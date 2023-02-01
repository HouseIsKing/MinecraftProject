#include "World.h"

uint16_t World::RegisterEntity(Entity* entity)
{
    const uint16_t index = EntityAvailableIDs.top();
    EntitiesAdded.emplace_back(entity);
    EntityAvailableIDs.pop();
    return index;
}

void World::RemoveEntity(const uint16_t id)
{
    EntitiesToRemove.push_back(id);
}

void World::HandleKeyCallback(int key, int action) const
{
    if (Entities.contains(PlayerId))
    {
        reinterpret_cast<PlayerController*>(Entities.at(PlayerId).get())->KeyboardButtonPressed(key, action);
    }
}

void World::HandleCursorPosCallback(float xPos, float yPos) const
{
    if (Entities.contains(PlayerId))
    {
        reinterpret_cast<PlayerController*>(Entities.at(PlayerId).get())->UpdateMouseMove(xPos, yPos);
    }
}

void World::HandleMouseButtonCallback(int button, int action) const
{
    if (Entities.contains(PlayerId))
    {
        reinterpret_cast<PlayerController*>(Entities.at(PlayerId).get())->MouseButtonPressed(button, action);
    }
}

void World::Init()
{
    Shader::SetUnsignedInt(EngineDefaults::GetShader()->GetUniformInt("worldTime"), static_cast<GLuint>(WorldTime % 24000L));
    InitFog();
    EngineDefaults::InitTextures();
    LastTimeFrame = static_cast<float>(glfwGetTime());
}

void World::Tick()
{
    WorldTime++;
    //Send Input to server
    for (auto* val : EntitiesAdded)
    {
        Entities.emplace(val->GetEntityId(), val);
    }
    EntitiesAdded.clear();
    for (uint16_t val : EntitiesToRemove)
    {
        Entities.erase(val);
        EntityAvailableIDs.push(val);
    }
    EntitiesToRemove.clear();
    for (const auto& val : Entities | std::views::values)
    {
        val->Tick();
    }
    const int numTilesToTick = LevelWidth * LevelHeight * LevelDepth / 400;
    for (int i = 0; i < numTilesToTick; i++)
    {
        const int x = RandomEngineState.GetNext(LevelWidth);
        const int y = RandomEngineState.GetNext(LevelHeight);
        const int z = RandomEngineState.GetNext(LevelDepth);
        GetBlockAt(x, y, z)->Tick(this, x, y, z);
    }
}

void World::InitFog()
{
    const std::array fogs{14.0F / 255.0F, 11.0F / 255.0F, 10.0F / 255.0F, 1.0F, 0.01F, 0.0F, 0.0F, 0.0F, 254.0F / 255.0F, 251.0F / 255.0F, 250.0F / 255.0F, 1.0F, 0.001F, 0.0F, 0.0F, 0.0F};
    glGenBuffers(1, &FogsBuffer);
    glBindBuffer(GL_UNIFORM_BUFFER, FogsBuffer);
    glBufferData(GL_UNIFORM_BUFFER, static_cast<GLintptr>(fogs.size() * sizeof(float)), fogs.data(), GL_STATIC_COPY);
    glBindBufferRange(GL_UNIFORM_BUFFER, 1, FogsBuffer, 0, static_cast<GLintptr>(fogs.size() * sizeof(float)));
    glUniformBlockBinding(EngineDefaults::GetShader()->Program, EngineDefaults::GetShader()->GetUniformBlockIndex("fogBlock"), 1);
}

void World::DrawGui() const
{
    Shader::SetUnsignedInt(EngineDefaults::GetShader(1)->GetUniformInt("worldTime"), static_cast<GLuint>(WorldTime % 24000L));
    for (const auto& gui : Guis)
    {
        gui->Render();
    }
}

void World::RebuildGui() const
{
    for (const auto& gui : Guis)
    {
        gui->Rebuild();
    }
}

void World::DrawWorld(const float partialTick)
{
    glEnable(GL_CULL_FACE);
    if (Entities.contains(PlayerId))
    {
        Entities[PlayerId]->Render(partialTick);
        auto* player = reinterpret_cast<PlayerController*>(Entities[PlayerId].get());
        const Frustum frustum = player->GetCameraFrustum();
        for (const std::unique_ptr<Entity>& entity : Entities | std::views::values)
        {
            if (entity->GetEntityId() != PlayerId && frustum.CubeInFrustum(entity->GetBoundingBox()))
            {
                entity->Render(partialTick);
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
        player->DisplaySelectionHighlight();
    }
    DrawGui();
    glDisable(GL_CULL_FACE);
    Frames++;
    const auto time = static_cast<float>(glfwGetTime());
    DeltaFrameTime += time - LastTimeFrame;
    TickTimer += time - LastTimeFrame;
    LastTimeFrame = time;
    if (DeltaFrameTime > 1.0F)
    {
        DeltaFrameTime -= 1.0F;
        Fps = Frames;
        Frames = 0;
    }
}

World::World(GLFWwindow* window, const uint16_t levelHeight, const uint16_t levelWidth, const uint16_t levelDepth) : PlayerId(0), TheAppWindow(window), LevelHeight(levelHeight), LevelWidth(levelWidth), LevelDepth(levelDepth)
{
    Entity::SetWorld(this);
    Chunk::SetWorld(this);
    Gui::SetWorld(this);
    Guis.emplace_back(new CrosshairGui());
    Guis[0]->Active = true;
    Guis.emplace_back(new SelectedBlockGui());
    Guis[1]->Active = true;
    Guis.emplace_back(new PerformanceGui());
    Guis[2]->Active = true;
}

GLFWwindow* World::GetWindow() const
{
    return TheAppWindow;
}

int World::GetFps() const
{
    return Fps;
}

int World::GetBrightnessAt(const glm::vec3 pos) const
{
    return GetBrightnessAt(static_cast<int>(pos.x), static_cast<int>(pos.y), static_cast<int>(pos.z));
}

int World::GetBrightnessAt(const int x, const int y, const int z) const
{
    if (const auto pos = glm::ivec2(x, z); LightLevels.contains(pos))
    {
        if (const uint8_t lightLevel = LightLevels.at(glm::ivec2(x, z)); y > lightLevel)
        {
            return 1;
        }
    }
    return 0;
}

Entity* World::GetEntity(uint16_t id) const
{
    if (Entities.contains(id))
    {
        return Entities.at(id).get();
    }
    return nullptr;
}

PlayerController* World::GetPlayer() const
{
    return reinterpret_cast<PlayerController*>(Entities.at(PlayerId).get());
}

uint64_t World::GetWorldTime() const
{
    return WorldTime;
}


void World::HandleWindowResize(const int height, const int width) const
{
    glViewport(0, 0, width, height);
    CameraController::OnResizeWindow(width, height);
    RebuildGui();
}

void World::AddChunkAsDirty(Chunk* chunk)
{
    if (DirtyChunksDuplicatorCheck.insert(chunk).second)
    {
        chunk->GotDirty();
        DirtyChunks.push_back(chunk);
    }
}

bool World::IsBlockExists(const int x, const int y, const int z)
{
    return GetBlockTypeAt(x, y, z) != EBlockType::Air;
}

const Block* World::GetBlockAt(const int x, const int y, const int z)
{
    return BlockTypeList::GetBlockTypeData(GetBlockTypeAt(x, y, z));
}

EBlockType World::GetBlockTypeAt(const int x, const int y, const int z)
{
    const Chunk* chunk = GetChunkAt(x, y, z);
    if (chunk == nullptr)
    {
        return EBlockType::Air;
    }
    return chunk->GetBlockTypeAt(x, y, z);
}

Chunk* World::GetChunkAt(const int x, const int y, const int z)
{
    if (const auto pos = ChunkCoords(x, y, z); Chunks.contains(pos))
    {
        return &Chunks.at(pos);
    }
    return nullptr;
}

bool World::IsBlockSolid(int x, int y, int z)
{
    return GetBlockAt(x, y, z)->IsSolidBlock();
}

std::vector<BoundingBox> World::GetBlockBoxesInBoundingBox(const BoundingBox& boundingBox)
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

void World::PlaceBlockAt(const int x, const int y, const int z, const EBlockType blockType)
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

void World::RemoveBlockAt(const int x, const int y, const int z)
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

void World::UpdateChunksNear(const int x, const int y, const int z)
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

int World::RecalculateLightLevels(const int x, const int z)
{
    const glm::ivec2 coords{x, z};
    const int previousLightLevel = LightLevels[coords];
    for (int y = LevelHeight - 1; y >= 0; y--)
    {
        if (GetBlockAt(x, y, z)->IsBlockingLight() || y == 0)
        {
            LightLevels[coords] = static_cast<uint8_t>(y);
            return y - previousLightLevel;
        }
    }
    return 0 - previousLightLevel;
}
