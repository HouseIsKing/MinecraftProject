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

Chunk::Chunk(const int x, const int y, const int z) : ChunkPosition(x, y, z), Tessellation(new TessellationHelper(EngineDefaults::GetShader()))
{
    for (auto& block : Blocks)
    {
        block = EBlockType::Air;
    }
}

void Chunk::DrawBlock(const EBlockType blockType, const int x, const int y, const int z) const
{
    const Block* myBlock = BlockTypeList::GetBlockTypeData(blockType);
    const int finalX = x + ChunkPosition.GetX() * CHUNK_WIDTH;
    const int finalY = y + ChunkPosition.GetY() * CHUNK_HEIGHT;
    const int finalZ = z + ChunkPosition.GetZ() * CHUNK_DEPTH;
    if (IsCoordsInsideChunk(finalX, finalY + 1, finalZ))
    {
        if (Blocks[static_cast<size_t>(EngineDefaults::GetChunkLocalIndex(finalX, finalY + 1, finalZ))] == EBlockType::Air)
        {
            myBlock->GenerateTessellationData(*Tessellation, BlockFaces::Top, static_cast<float>(finalX), static_cast<float>(finalY), static_cast<float>(finalZ));
        }
    }
    else if (!GetWorld()->IsBlockExists(finalX, finalY + 1, finalZ))
    {
        myBlock->GenerateTessellationData(*Tessellation, BlockFaces::Top, static_cast<float>(finalX), static_cast<float>(finalY), static_cast<float>(finalZ));
    }
    if (IsCoordsInsideChunk(finalX, finalY - 1, finalZ))
    {
        if (Blocks[static_cast<size_t>(EngineDefaults::GetChunkLocalIndex(finalX, finalY - 1, finalZ))] == EBlockType::Air)
        {
            myBlock->GenerateTessellationData(*Tessellation, BlockFaces::Bottom, static_cast<float>(finalX), static_cast<float>(finalY), static_cast<float>(finalZ));
        }
    }
    else if (!GetWorld()->IsBlockExists(finalX, finalY - 1, finalZ))
    {
        myBlock->GenerateTessellationData(*Tessellation, BlockFaces::Bottom, static_cast<float>(finalX), static_cast<float>(finalY), static_cast<float>(finalZ));
    }
    if (IsCoordsInsideChunk(finalX, finalY, finalZ + 1))
    {
        if (Blocks[static_cast<size_t>(EngineDefaults::GetChunkLocalIndex(finalX, finalY, finalZ + 1))] == EBlockType::Air)
        {
            myBlock->GenerateTessellationData(*Tessellation, BlockFaces::North, static_cast<float>(finalX), static_cast<float>(finalY), static_cast<float>(finalZ));
        }
    }
    else if (!GetWorld()->IsBlockExists(finalX, finalY, finalZ + 1))
    {
        myBlock->GenerateTessellationData(*Tessellation, BlockFaces::North, static_cast<float>(finalX), static_cast<float>(finalY), static_cast<float>(finalZ));
    }
    if (IsCoordsInsideChunk(finalX, finalY, finalZ - 1))
    {
        if (Blocks[static_cast<size_t>(EngineDefaults::GetChunkLocalIndex(finalX, finalY, finalZ - 1))] == EBlockType::Air)
        {
            myBlock->GenerateTessellationData(*Tessellation, BlockFaces::South, static_cast<float>(finalX), static_cast<float>(finalY), static_cast<float>(finalZ));
        }
    }
    else if (!GetWorld()->IsBlockExists(finalX, finalY, finalZ - 1))
    {
        myBlock->GenerateTessellationData(*Tessellation, BlockFaces::South, static_cast<float>(finalX), static_cast<float>(finalY), static_cast<float>(finalZ));
    }
    if (IsCoordsInsideChunk(finalX + 1, finalY, finalZ))
    {
        if (Blocks[static_cast<size_t>(EngineDefaults::GetChunkLocalIndex(finalX + 1, finalY, finalZ))] == EBlockType::Air)
        {
            myBlock->GenerateTessellationData(*Tessellation, BlockFaces::East, static_cast<float>(finalX), static_cast<float>(finalY), static_cast<float>(finalZ));
        }
    }
    else if (!GetWorld()->IsBlockExists(finalX + 1, finalY, finalZ))
    {
        myBlock->GenerateTessellationData(*Tessellation, BlockFaces::East, static_cast<float>(finalX), static_cast<float>(finalY), static_cast<float>(finalZ));
    }
    if (IsCoordsInsideChunk(finalX - 1, finalY, finalZ))
    {
        if (Blocks[static_cast<size_t>(EngineDefaults::GetChunkLocalIndex(finalX - 1, finalY, finalZ))] == EBlockType::Air)
        {
            myBlock->GenerateTessellationData(*Tessellation, BlockFaces::West, static_cast<float>(finalX), static_cast<float>(finalY), static_cast<float>(finalZ));
        }
    }
    else if (!GetWorld()->IsBlockExists(finalX - 1, finalY, finalZ))
    {
        myBlock->GenerateTessellationData(*Tessellation, BlockFaces::West, static_cast<float>(finalX), static_cast<float>(finalY), static_cast<float>(finalZ));
    }
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
    Tessellation.reset();
}

void Chunk::Draw() const
{
    if (!Tessellation->HasInitialized())
    {
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
    Tessellation->Draw();
}

void Chunk::SetBlockTypeAt(const int x, const int y, const int z, const EBlockType block)
{
    Blocks[static_cast<size_t>(EngineDefaults::GetChunkLocalIndex(x, y, z))] = block;
    if (Tessellation->HasInitialized())
    {
        Tessellation->Reset();
    }
}
