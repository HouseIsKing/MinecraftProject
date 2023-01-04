//
// Created by amit on 4/21/2022.
//

#include "Chunk.h"
#include "../Util/EngineDefaults.h"
#include "SinglePlayerWorld.h"

SinglePlayerWorld* Chunk::World = nullptr;

void Chunk::SetWorld(SinglePlayerWorld* newWorld)
{
    World = newWorld;
}

SinglePlayerWorld* Chunk::GetWorld()
{
    return World;
}

Chunk::Chunk(const int x, const int y, const int z) : ChunkPosition(x, y, z)
{
    GetWorld()->AddChunkAsDirty(this);
    for (auto& block : Blocks)
    {
        block = EBlockType::Air;
    }
}

void Chunk::DrawBlock(const EBlockType blockType, const int x, const int y, const int z)
{
    const Block* myBlock = BlockTypeList::GetBlockTypeData(blockType);
    const int finalX = x + ChunkPosition.GetX() * CHUNK_WIDTH;
    const int finalY = y + ChunkPosition.GetY() * CHUNK_HEIGHT;
    const int finalZ = z + ChunkPosition.GetZ() * CHUNK_DEPTH;
    if (IsCoordsInsideChunk(finalX, finalY + 1, finalZ))
    {
        const int brightness = World->GetBrightnessAt(finalX, finalY + 1, finalZ);
        if (Blocks[static_cast<size_t>(EngineDefaults::GetChunkLocalIndex(finalX, finalY + 1, finalZ))] == EBlockType::Air)
        {
            myBlock->GenerateTessellationData(Tessellation, BlockFaces::Top, static_cast<float>(finalX), static_cast<float>(finalY), static_cast<float>(finalZ), brightness);
        }
    }
    else if (!GetWorld()->IsBlockExists(finalX, finalY + 1, finalZ))
    {
        const int brightness = World->GetBrightnessAt(finalX, finalY + 1, finalZ);
        myBlock->GenerateTessellationData(Tessellation, BlockFaces::Top, static_cast<float>(finalX), static_cast<float>(finalY), static_cast<float>(finalZ), brightness);
    }
    if (IsCoordsInsideChunk(finalX, finalY - 1, finalZ))
    {
        const int brightness = World->GetBrightnessAt(finalX, finalY - 1, finalZ);
        if (Blocks[static_cast<size_t>(EngineDefaults::GetChunkLocalIndex(finalX, finalY - 1, finalZ))] == EBlockType::Air)
        {
            myBlock->GenerateTessellationData(Tessellation, BlockFaces::Bottom, static_cast<float>(finalX), static_cast<float>(finalY), static_cast<float>(finalZ), brightness);
        }
    }
    else if (!GetWorld()->IsBlockExists(finalX, finalY - 1, finalZ))
    {
        const int brightness = World->GetBrightnessAt(finalX, finalY - 1, finalZ);
        myBlock->GenerateTessellationData(Tessellation, BlockFaces::Bottom, static_cast<float>(finalX), static_cast<float>(finalY), static_cast<float>(finalZ), brightness);
    }
    if (IsCoordsInsideChunk(finalX, finalY, finalZ + 1))
    {
        const int brightness = World->GetBrightnessAt(finalX, finalY, finalZ + 1);
        if (Blocks[static_cast<size_t>(EngineDefaults::GetChunkLocalIndex(finalX, finalY, finalZ + 1))] == EBlockType::Air)
        {
            myBlock->GenerateTessellationData(Tessellation, BlockFaces::North, static_cast<float>(finalX), static_cast<float>(finalY), static_cast<float>(finalZ), brightness);
        }
    }
    else if (!GetWorld()->IsBlockExists(finalX, finalY, finalZ + 1))
    {
        const int brightness = World->GetBrightnessAt(finalX, finalY, finalZ + 1);
        myBlock->GenerateTessellationData(Tessellation, BlockFaces::North, static_cast<float>(finalX), static_cast<float>(finalY), static_cast<float>(finalZ), brightness);
    }
    if (IsCoordsInsideChunk(finalX, finalY, finalZ - 1))
    {
        const int brightness = World->GetBrightnessAt(finalX, finalY, finalZ - 1);
        if (Blocks[static_cast<size_t>(EngineDefaults::GetChunkLocalIndex(finalX, finalY, finalZ - 1))] == EBlockType::Air)
        {
            myBlock->GenerateTessellationData(Tessellation, BlockFaces::South, static_cast<float>(finalX), static_cast<float>(finalY), static_cast<float>(finalZ), brightness);
        }
    }
    else if (!GetWorld()->IsBlockExists(finalX, finalY, finalZ - 1))
    {
        const int brightness = World->GetBrightnessAt(finalX, finalY, finalZ - 1);
        myBlock->GenerateTessellationData(Tessellation, BlockFaces::South, static_cast<float>(finalX), static_cast<float>(finalY), static_cast<float>(finalZ), brightness);
    }
    if (IsCoordsInsideChunk(finalX + 1, finalY, finalZ))
    {
        const int brightness = World->GetBrightnessAt(finalX + 1, finalY, finalZ);
        if (Blocks[static_cast<size_t>(EngineDefaults::GetChunkLocalIndex(finalX + 1, finalY, finalZ))] == EBlockType::Air)
        {
            myBlock->GenerateTessellationData(Tessellation, BlockFaces::East, static_cast<float>(finalX), static_cast<float>(finalY), static_cast<float>(finalZ), brightness);
        }
    }
    else if (!GetWorld()->IsBlockExists(finalX + 1, finalY, finalZ))
    {
        const int brightness = World->GetBrightnessAt(finalX + 1, finalY, finalZ);
        myBlock->GenerateTessellationData(Tessellation, BlockFaces::East, static_cast<float>(finalX), static_cast<float>(finalY), static_cast<float>(finalZ), brightness);
    }
    if (IsCoordsInsideChunk(finalX - 1, finalY, finalZ))
    {
        const int brightness = World->GetBrightnessAt(finalX - 1, finalY, finalZ);
        if (Blocks[static_cast<size_t>(EngineDefaults::GetChunkLocalIndex(finalX - 1, finalY, finalZ))] == EBlockType::Air)
        {
            myBlock->GenerateTessellationData(Tessellation, BlockFaces::West, static_cast<float>(finalX), static_cast<float>(finalY), static_cast<float>(finalZ), brightness);
        }
    }
    else if (!GetWorld()->IsBlockExists(finalX - 1, finalY, finalZ))
    {
        const int brightness = World->GetBrightnessAt(finalX - 1, finalY, finalZ);
        myBlock->GenerateTessellationData(Tessellation, BlockFaces::West, static_cast<float>(finalX), static_cast<float>(finalY), static_cast<float>(finalZ), brightness);
    }
}

float Chunk::GetDistanceFromPlayer() const
{
    PlayerController* player = GetWorld()->GetPlayer();
    if (player == nullptr)
        return 0.0F;
    const vec3 temp = player->GetTransform().GetPosition() - vec3(static_cast<float>(ChunkPosition.GetX() * CHUNK_WIDTH) + CHUNK_WIDTH / 2.0F, static_cast<float>(ChunkPosition.GetY() * CHUNK_HEIGHT) + CHUNK_HEIGHT / 2.0F, static_cast<float>(ChunkPosition.GetZ() * CHUNK_DEPTH) + CHUNK_DEPTH / 2.0F);
    return dot(temp, temp);
}

const Block* Chunk::GetBlockAt(const int x, const int y, const int z) const
{
    return BlockTypeList::GetBlockTypeData(GetBlockTypeAt(x, y, z));
}

EBlockType Chunk::GetBlockTypeAt(const int x, const int y, const int z) const
{
    return Blocks[static_cast<size_t>(EngineDefaults::GetChunkLocalIndex(x, y, z))];
}

bool Chunk::IsCoordsInsideChunk(const int x, const int y, const int z) const
{
    return x >= ChunkPosition.GetX() && y >= ChunkPosition.GetY() && z >= ChunkPosition.GetZ() && x < ChunkPosition.GetX() + CHUNK_WIDTH && y < ChunkPosition.GetY() + CHUNK_HEIGHT && z < ChunkPosition.GetZ() + CHUNK_DEPTH;
}

void Chunk::ResetDraw()
{
    Tessellation.Reset();
}

void Chunk::Draw()
{
    Tessellation.Draw();
    if (Tessellation.HasInitialized())
    {
        Tessellation.FreeMemory();
    }
}

void Chunk::GenerateTessellationData()
{
    if (Tessellation.HasInitialized())
    {
        ResetDraw();
    }
    for (int x = 0; x < CHUNK_WIDTH; x++)
    {
        for (int y = 0; y < CHUNK_HEIGHT; y++)
        {
            for (int z = 0; z < CHUNK_DEPTH; z++)
            {
                if (const EBlockType type = Blocks[static_cast<size_t>(EngineDefaults::GetChunkLocalIndex(x, y, z))]; type != EBlockType::Air)
                {
                    DrawBlock(type, x, y, z);
                }
            }
        }
    }
}

void Chunk::SetBlockTypeAt(const int x, const int y, const int z, const EBlockType block)
{
    const auto index = static_cast<size_t>(EngineDefaults::GetChunkLocalIndex(x, y, z));
    const EBlockType check = Blocks[index];
    Blocks[index] = block;
    if (check != block)
    {
        GetWorld()->AddChunkAsDirty(this);
    }
}

bool Chunk::operator<(const Chunk& other) const
{
    return GetDistanceFromPlayer() < other.GetDistanceFromPlayer();
}

CustomFileManager& operator<<(CustomFileManager& fileManager, const Chunk& chunk)
{
    fileManager << chunk.ChunkPosition;
    for (const auto& block : chunk.Blocks)
    {
        fileManager << block;
    }
    return fileManager;
}

CustomFileManager& operator>>(CustomFileManager& fileManager, Chunk& chunk)
{
    for (auto& block : chunk.Blocks)
    {
        fileManager >> block;
    }
    return fileManager;
}
