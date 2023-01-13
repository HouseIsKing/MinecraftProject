#include "SinglePlayerWorld.h"
#include "Entities/Zombie.h"
#include "GUI/CrosshairGui.h"
#include "GUI/PerformanceGui.h"
#include "Util/EngineDefaults.h"
#include "Util/PerlinNoise.h"
#include <filesystem>
#include <iostream>


void SinglePlayerWorld::SaveWorld()
{
    CustomFileManager fileManager{"level.dat", "w"};
    fileManager << Chunks.size();
    for (auto& val : Chunks | std::views::values)
    {
        fileManager << val;
    }
}

void SinglePlayerWorld::LoadWorld()
{
    CustomFileManager fileManager{"level.dat", "r"};
    size_t size = 0;
    fileManager >> size;
    for (size_t i = 0; i < size; i++)
    {
        ChunkCoords coords;
        fileManager >> coords;
        int x = coords.GetX() * Chunk::CHUNK_WIDTH;
        int y = coords.GetY() * Chunk::CHUNK_HEIGHT;
        int z = coords.GetZ() * Chunk::CHUNK_DEPTH;
        fileManager >> Chunks.emplace(std::piecewise_construct, std::forward_as_tuple(x, y, z), std::forward_as_tuple(x, y, z)).first->second;
    }
}

SinglePlayerWorld::SinglePlayerWorld(const uint16_t width, const uint16_t height, const uint16_t depth, GLFWwindow* window) : Player(nullptr), WorldTime(0), LevelWidth(width), LevelHeight(height), LevelDepth(depth), FogsBuffer(0), TheAppWindow(window), LastTimeFrame(0.0F), DeltaTime(0.0F), Frames(0), Fps(0)
{
    Entity::SetWorld(this);
    Chunk::SetWorld(this);
    Gui::SetWorld(this);
    for (uint16_t i = 0xFFFF; i != 0; i--)
    {
        EntityAvailableIDs.push(i);
    }
    EntityAvailableIDs.push(0);
    Guis.emplace_back(new CrosshairGui());
    Guis[0]->Active = true;
    Guis.emplace_back(new SelectedBlockGui());
    Guis[1]->Active = true;
    Guis.emplace_back(new PerformanceGui());
    Guis[2]->Active = true;
    Player = new PlayerController(EngineDefaults::GetNext(width), static_cast<float>(height + 3), EngineDefaults::GetNext(depth));
    Init();
    for (uint16_t i = 1; i <= 10; i++)
    {
        new Zombie(EngineDefaults::GetNext(width), static_cast<float>(LevelHeight + 3), EngineDefaults::GetNext(depth));
    }
    EngineDefaults::BuildTextureUbo();
}

uint16_t SinglePlayerWorld::RegisterEntity(Entity* entity)
{
    uint16_t index = EntityAvailableIDs.top();
    EntityAvailableIDs.pop();
    Entities.emplace(index, entity);
    return index;
}

void SinglePlayerWorld::RemoveEntity(const uint16_t id)
{
    EntitiesToRemove.push_back(id);
}

void SinglePlayerWorld::Tick()
{
    WorldTime++;
    Shader::SetUnsignedInt(EngineDefaults::GetShader()->GetUniformInt("worldTime"), static_cast<GLuint>(WorldTime % 24000L));
    for (uint16_t val : EntitiesToRemove)
    {
        Entities.erase(val);
        EntityAvailableIDs.push(val);
    }
    EntitiesToRemove.clear();
    for (const auto& val : Entities | std::views::values)
    {
        val->DoTick();
    }
    const int numTilesToTick = LevelWidth * LevelHeight * LevelDepth / 400;
    for (int i = 0; i < numTilesToTick; i++)
    {
        const int x = EngineDefaults::GetNext(LevelWidth);
        const int y = EngineDefaults::GetNext(LevelHeight);
        const int z = EngineDefaults::GetNext(LevelDepth);
        GetBlockAt(x, y, z)->Tick(this, x, y, z);
    }
}

void SinglePlayerWorld::HandleWindowResize(const int height, const int width) const
{
    glViewport(0, 0, width, height);
    CameraController::OnResizeWindow(width, height);
    RebuildGui();
}

void SinglePlayerWorld::Init()
{
    //Shader::SetVec3(EngineDefaults::GetShader()->GetUniformInt("directionalLightDirection"), -1.0F, -1.0F, -1.0F);
    Shader::SetUnsignedInt(EngineDefaults::GetShader()->GetUniformInt("worldTime"), static_cast<GLuint>(WorldTime % 24000L));
    InitFog();
    BlockTypeList::InitBlockTypes();
    if (std::filesystem::exists("level.dat"))
    {
        LoadWorld();
    }
    else
    {
        GenerateLevel();
    }
    RecalculateLightLevels();
}

void SinglePlayerWorld::InitFog()
{
    const std::array fogs{14.0F / 255.0F, 11.0F / 255.0F, 10.0F / 255.0F, 1.0F, 0.01F, 0.0F, 0.0F, 0.0F, 254.0F / 255.0F, 251.0F / 255.0F, 250.0F / 255.0F, 1.0F, 0.001F, 0.0F, 0.0F, 0.0F};
    glGenBuffers(1, &FogsBuffer);
    glBindBuffer(GL_UNIFORM_BUFFER, FogsBuffer);
    glBufferData(GL_UNIFORM_BUFFER, static_cast<GLintptr>(fogs.size() * sizeof(float)), fogs.data(), GL_STATIC_COPY);
    glBindBufferRange(GL_UNIFORM_BUFFER, 1, FogsBuffer, 0, static_cast<GLintptr>(fogs.size() * sizeof(float)));
    glUniformBlockBinding(EngineDefaults::GetShader()->Program, EngineDefaults::GetShader()->GetUniformBlockIndex("fogBlock"), 1);
}

void SinglePlayerWorld::RecalculateLightLevels()
{
    LightLevels.clear();
    LightLevels.reserve(static_cast<size_t>(LevelWidth * LevelDepth));
    while (LightLevels.size() < static_cast<size_t>(LevelWidth * LevelDepth))
    {
        LightLevels.push_back(0);
    }
    for (int x = 0; x < LevelWidth; x++)
    {
        for (int z = 0; z < LevelDepth; z++)
        {
            RecalculateLightLevels(x, z);
        }
    }
}

int SinglePlayerWorld::RecalculateLightLevels(const int x, const int z)
{
    const int previousLightLevel = LightLevels[static_cast<size_t>(x * LevelDepth + z)];
    for (int y = LevelHeight - 1; y >= 0; y--)
    {
        if (GetBlockAt(x, y, z)->IsBlockingLight() || y == 0)
        {
            LightLevels.at(static_cast<size_t>(x * LevelDepth + z)) = static_cast<uint8_t>(y);
            return y - previousLightLevel;
        }
    }
    return 0 - previousLightLevel;
}

void SinglePlayerWorld::UpdateChunksNear(const int x, const int y, const int z)
{
    Chunk* chunk = GetChunkAt(x, y + 1, z);
    if (chunk != nullptr)
    {
        AddChunkAsDirty(chunk);
    }
    chunk = GetChunkAt(x, y - 1, z);
    if (chunk != nullptr)
    {
        AddChunkAsDirty(chunk);
    }
    chunk = GetChunkAt(x, y, z + 1);
    if (chunk != nullptr)
    {
        AddChunkAsDirty(chunk);
    }
    chunk = GetChunkAt(x, y, z - 1);
    if (chunk != nullptr)
    {
        AddChunkAsDirty(chunk);
    }
    chunk = GetChunkAt(x + 1, y, z);
    if (chunk != nullptr)
    {
        AddChunkAsDirty(chunk);
    }
    chunk = GetChunkAt(x - 1, y, z);
    if (chunk != nullptr)
    {
        AddChunkAsDirty(chunk);
    }
}

void SinglePlayerWorld::DrawGui() const
{
    Shader::SetUnsignedInt(EngineDefaults::GetShader(1)->GetUniformInt("worldTime"), static_cast<GLuint>(WorldTime % 24000L));
    for (const auto& gui : Guis)
    {
        gui->Render();
    }
}

void SinglePlayerWorld::GenerateChunks(const uint16_t amountX, const uint16_t amountY, const uint16_t amountZ)
{
    Chunks.reserve(static_cast<uint64_t>(amountX) * static_cast<uint64_t>(amountY) * static_cast<uint64_t>(amountZ));
    for (int x = 0; x < amountX; x++)
    {
        for (int y = 0; y < amountY; y++)
        {
            for (int z = 0; z < amountZ; z++)
            {
                Chunks.emplace(std::piecewise_construct, std::forward_as_tuple(x * Chunk::CHUNK_WIDTH, y * Chunk::CHUNK_HEIGHT, z * Chunk::CHUNK_DEPTH), std::forward_as_tuple(x * Chunk::CHUNK_WIDTH, y * Chunk::CHUNK_HEIGHT, z * Chunk::CHUNK_DEPTH));
            }
        }
    }
}

void SinglePlayerWorld::GenerateLevel()
{
    const auto amountX = static_cast<uint16_t>(std::ceil(static_cast<float>(LevelWidth) / static_cast<float>(Chunk::CHUNK_WIDTH)));
    const auto amountY = static_cast<uint16_t>(std::ceil(static_cast<float>(LevelHeight) / static_cast<float>(Chunk::CHUNK_HEIGHT)));
    const auto amountZ = static_cast<uint16_t>(std::ceil(static_cast<float>(LevelDepth) / static_cast<float>(Chunk::CHUNK_DEPTH)));
    GenerateChunks(amountX, amountY, amountZ);
    const std::vector<int> firstHeightMap = PerlinNoise(0).Generate(LevelWidth, LevelDepth);
    const std::vector<int> secondHeightMap = PerlinNoise(0).Generate(LevelWidth, LevelDepth);
    const std::vector<int> cliffMap = PerlinNoise(1).Generate(LevelWidth, LevelDepth);
    const std::vector<int> rockMap = PerlinNoise(1).Generate(LevelWidth, LevelDepth);

    for (int x = 0; x < LevelWidth; x++)
    {
        for (int y = 0; y < LevelHeight; y++)
        {
            for (int z = 0; z < LevelDepth; z++)
            {
                int firstHeightValue = firstHeightMap[static_cast<size_t>(x + z * LevelWidth)];
                int secondHeightValue = secondHeightMap[static_cast<size_t>(x + z * LevelWidth)];

                if (cliffMap[static_cast<size_t>(x + z * LevelWidth)] < 128)
                {
                    secondHeightValue = firstHeightValue;
                }

                const int maxLevelHeight = std::max(secondHeightValue, firstHeightValue) / 8 + LevelHeight / 3;
                int maxRockHeight = rockMap[static_cast<size_t>(x + z * LevelWidth)] / 8 + LevelHeight / 3;

                // Keep it below the max height of the level
                if (maxRockHeight > maxLevelHeight - 2)
                {
                    maxRockHeight = maxLevelHeight - 2;
                }

                auto blockType = EBlockType::Air;
                // Grass layer
                if (y == maxLevelHeight)
                {
                    blockType = EBlockType::Grass;
                }

                // Dirt layer
                if (y < maxLevelHeight)
                {
                    blockType = EBlockType::Dirt;
                }

                // Rock layer
                if (y <= maxRockHeight)
                {
                    blockType = EBlockType::Stone;
                }

                GetChunkAt(x, y, z)->SetBlockTypeAt(x, y, z, blockType);
            }
        }
    }
    const int count = LevelWidth * LevelHeight * LevelDepth / 256 / 64;
    for (int i = 0; i < count; i++)
    {
        float x = EngineDefaults::GetNextFloat() * static_cast<float>(LevelWidth);
        float y = EngineDefaults::GetNextFloat() * static_cast<float>(LevelHeight);
        float z = EngineDefaults::GetNextFloat() * static_cast<float>(LevelDepth);
        const int length = static_cast<int>(EngineDefaults::GetNextFloat() + EngineDefaults::GetNextFloat() * 150.0F);
        float dir1 = EngineDefaults::GetNextFloat() * glm::pi<float>() * 2.0F;
        float dir1Change = 0.0F;
        float dir2 = EngineDefaults::GetNextFloat() * glm::pi<float>() * 2.0F;
        float dir2Change = 0.0F;
        for (int l = 0; l < length; l++)
        {
            x += sin(dir1) * cos(dir2);
            z += cos(dir1) * cos(dir2);
            y += sin(dir2);
            dir1 += dir1Change * 0.2F;
            dir1Change *= 0.9F;
            dir1Change += EngineDefaults::GetNextFloat() - EngineDefaults::GetNextFloat();
            dir2 += dir2Change * 0.5F;
            dir2 *= 0.5F;
            dir2Change *= 0.9F;
            dir2Change += EngineDefaults::GetNextFloat() - EngineDefaults::GetNextFloat();
            const float size = sin(static_cast<float>(l) * glm::pi<float>() / static_cast<float>(length)) * 2.5F + 1.0F;
            for (int xx = static_cast<int>(x - size); xx <= static_cast<int>(x + size); xx++)
            {
                for (int yy = static_cast<int>(y - size); yy <= static_cast<int>(y + size); yy++)
                {
                    for (int zz = static_cast<int>(z - size); zz <= static_cast<int>(z + size); zz++)
                    {
                        const float deltaX = static_cast<float>(xx) - x;
                        const float deltaY = static_cast<float>(yy) - y;
                        const float deltaZ = static_cast<float>(zz) - z;
                        if (const float distance = deltaX * deltaX + deltaY * deltaY * 2.0F + deltaZ * deltaZ; distance < size * size && xx >= 1 && yy >= 1 && zz >= 1 && xx < LevelWidth - 1 && yy < LevelHeight - 1 && zz < LevelDepth - 1)
                        {
                            if (Chunk* chunk = GetChunkAt(xx, yy, zz); chunk->GetBlockTypeAt(xx, yy, zz) == EBlockType::Stone)
                            {
                                chunk->SetBlockTypeAt(xx, yy, zz, EBlockType::Air);
                            }
                        }
                    }
                }
            }
        }
    }
}

const Block* SinglePlayerWorld::GetBlockAt(const int x, const int y, const int z)
{
    return BlockTypeList::GetBlockTypeData(GetBlockTypeAt(x, y, z));
}

EBlockType SinglePlayerWorld::GetBlockTypeAt(const int x, const int y, const int z)
{
    const Chunk* chunk = GetChunkAt(x, y, z);
    if (chunk == nullptr)
    {
        return EBlockType::Air;
    }
    return chunk->GetBlockTypeAt(x, y, z);
}

Chunk* SinglePlayerWorld::GetChunkAt(const int x, const int y, const int z)
{
    const auto pos = ChunkCoords(x, y, z);
    if (!Chunks.contains(pos))
    {
        return nullptr;
    }
    return &Chunks.at(pos);
}

/**
 * \brief Rounds the float vector to integer
 * \param pos position to round
 * \return returns brightness at the rounded position
 */
int SinglePlayerWorld::GetBrightnessAt(const glm::vec3 pos) const
{
    return GetBrightnessAt(static_cast<int>(pos.x), static_cast<int>(pos.y), static_cast<int>(pos.z));
}

int SinglePlayerWorld::GetBrightnessAt(const int x, const int y, const int z) const
{
    if (x < 0 || x >= LevelWidth || y < 0 || y >= LevelHeight || z < 0 || z >= LevelDepth)
    {
        return 1;
    }
    if (const uint8_t lightLevel = LightLevels.at(static_cast<size_t>(x * LevelDepth + z)); y > lightLevel)
    {
        return 1;
    }
    return 0;
}

bool SinglePlayerWorld::IsBlockSolid(const int x, const int y, const int z)
{
    return GetBlockAt(x, y, z)->IsSolidBlock();
}

bool SinglePlayerWorld::IsBlockExists(const int x, const int y, const int z)
{
    return GetBlockTypeAt(x, y, z) != EBlockType::Air;
}

void SinglePlayerWorld::PlaceBlockAt(const int x, const int y, const int z, const EBlockType blockType)
{
    Chunk* chunk = GetChunkAt(x, y, z);
    if (chunk == nullptr)
    {
        return;
    }
    const Block* previousBlock = chunk->GetBlockAt(x, y, z);
    const Block* block = BlockTypeList::GetBlockTypeData(blockType);
    chunk->SetBlockTypeAt(x, y, z, blockType);
    if (block->IsSolidBlock() != previousBlock->IsSolidBlock())
    {
        UpdateChunksNear(x, y, z);
    }
    if (block->IsBlockingLight() != previousBlock->IsBlockingLight())
    {
        const int lightLevelsChange = RecalculateLightLevels(x, z);
        for (int i = 0; i <= abs(lightLevelsChange); i++)
        {
            if (Chunk* chunkLight = GetChunkAt(x, y + i * (lightLevelsChange > 0 ? -1 : 1), z); chunkLight != nullptr)
            {
                AddChunkAsDirty(chunkLight);
            }
        }
    }
}

void SinglePlayerWorld::RemoveBlockAt(const int x, const int y, const int z)
{
    Chunk* chunk = GetChunkAt(x, y, z);
    if (chunk == nullptr)
    {
        return;
    }
    const Block* block = chunk->GetBlockAt(x, y, z);
    block->OnBreak(this, x, y, z);
    chunk->SetBlockTypeAt(x, y, z, EBlockType::Air);
    if (block->IsSolidBlock())
    {
        UpdateChunksNear(x, y, z);
    }
    if (block->IsBlockingLight())
    {
        const int lightLevelsChange = RecalculateLightLevels(x, z);
        for (int i = 0; i <= abs(lightLevelsChange); i++)
        {
            if (Chunk* chunkLight = GetChunkAt(x, y + i * (lightLevelsChange > 0 ? 1 : -1), z); chunkLight != nullptr)
            {
                AddChunkAsDirty(chunkLight);
            }
        }
    }
}

void SinglePlayerWorld::AddChunkAsDirty(Chunk* chunk)
{
    if (DirtyChunksDuplicatorCheck.insert(chunk).second)
    {
        chunk->GotDirty();
        DirtyChunks.push_back(chunk);
    }
}

Entity* SinglePlayerWorld::GetEntity(const uint16_t id) const
{
    return Entities.at(id).get();
}

PlayerController* SinglePlayerWorld::GetPlayer() const
{
    return Player;
}

void SinglePlayerWorld::DrawWorld(const float partialTick)
{
    const auto time = static_cast<float>(glfwGetTime());
    DeltaTime += time - LastTimeFrame;
    LastTimeFrame = time;
    if (DeltaTime > 1.0F)
    {
        DeltaTime -= 1.0F;
        Fps = Frames;
        Frames = 0;
    }
    glEnable(GL_CULL_FACE);
    Player->Render(partialTick);
    const Frustum frustum = Player->GetCameraFrustum();
    for (const auto& entity : Entities | std::views::values)
    {
        if (entity.get() != Player && frustum.CubeInFrustum(entity->GetBoundingBox()))
        {
            entity->DoRender(partialTick);
        }
    }
    std::ranges::sort(DirtyChunks, DirtyChunkComparator{frustum});
    for (int i = 0; i < MaxChunkRebuilt && !DirtyChunks.empty(); i++)
    {
        DirtyChunks[0]->GenerateTessellationData();
        DirtyChunksDuplicatorCheck.erase(DirtyChunks[0]);
        DirtyChunks.erase(DirtyChunks.begin());
    }
    for (auto& chunk : Chunks | std::views::values)
    {
        if (chunk.ChunkInFrustum(frustum))
        {
            chunk.Draw();
        }
    }
    Player->DisplaySelectionHighlight();
    DrawGui();
    glDisable(GL_CULL_FACE);
    Frames++;
}

void SinglePlayerWorld::RebuildGui() const
{
    for (const auto& gui : Guis)
    {
        gui->Rebuild();
    }
}

GLFWwindow* SinglePlayerWorld::GetWindow() const
{
    return TheAppWindow;
}

int SinglePlayerWorld::GetFps() const
{
    return Fps;
}

std::vector<BoundingBox> SinglePlayerWorld::GetBlockBoxesInBoundingBox(const BoundingBox& boundingBox)
{
    std::vector<BoundingBox> result{};
    for (int x = static_cast<int>(boundingBox.GetMinX()); static_cast<float>(x) <= boundingBox.GetMaxX(); x++)
    {
        for (int y = static_cast<int>(boundingBox.GetMinY()); static_cast<float>(y) <= boundingBox.GetMaxY(); y++)
        {
            for (int z = static_cast<int>(boundingBox.GetMinZ()); static_cast<float>(z) <= boundingBox.GetMaxZ(); z++)
            {
                if (const Block* block = GetBlockAt(x, y, z); block->IsSolidBlock())
                {
                    BoundingBox helper = block->GetBoundingBox();
                    helper.Move(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z));
                    result.push_back(helper);
                }
            }
        }
    }
    return result;
}

SinglePlayerWorld::~SinglePlayerWorld()
{
    try
    {
        SaveWorld();
    }
    catch (...)
    {
        std::cout << "Error while saving world" << std::endl;
    }
    glDeleteBuffers(1, &FogsBuffer);
}
