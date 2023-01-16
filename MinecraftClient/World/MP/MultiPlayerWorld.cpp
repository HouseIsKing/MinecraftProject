#include "MultiPlayerWorld.h"


MultiPlayerWorld::MultiPlayerWorld(GLFWwindow* window) : World(window)
{
    Init();
}

uint16_t MultiPlayerWorld::RegisterEntity(Entity<MultiPlayerWorld>* entity)
{
    /*uint16_t index = EntityAvailableIDs.top();
    EntityAvailableIDs.pop();
    Entities.emplace(index, entity);
    return index;*/
    return 0;
}

void MultiPlayerWorld::Init()
{
    World::Init();
    //Do more stuff here
}

void MultiPlayerWorld::UpdateChunksNear(const int x, const int y, const int z)
{
    Chunk<MultiPlayerWorld>* chunk = GetChunkAt(x, y + 1, z);
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

void MultiPlayerWorld::Run()
{
    if (TickTimer > 1.0F)
    {
        TickTimer = 1.0F;
    }
    DrawWorld(TickTimer);
    //handle messages
}

MultiPlayerWorld::~MultiPlayerWorld()
{
    glDeleteBuffers(1, &FogsBuffer);
}
