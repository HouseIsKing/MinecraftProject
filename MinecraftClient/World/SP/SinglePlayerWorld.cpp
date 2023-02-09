#include "SinglePlayerWorld.h"
#include "Entities/Generic/ParticleEntity.h"
#include "Util/CustomFileManager.h"
#include "Util/EngineDefaults.h"
#include "Util/PerlinNoise.h"
#include <filesystem>
#include <iostream>
#include <memory>

#include "Entities/Generic/Zombie.h"

SinglePlayerWorld::SinglePlayerWorld(const uint16_t width, const uint16_t height, const uint16_t depth, GLFWwindow* window) : ClientWorld(window, height, width, depth)
{
    PlayerId = EntityAvailableIDs.top();
    EntityAvailableIDs.pop();
    State.AddPlayer(PlayerId, 0.0F, 68.0F, 0.0F);
    const Player& player = State.GetState().Players.at(PlayerId);
    LocalPlayerRenderer = std::make_unique<ClientPlayerRenderer>(player.GetState(), player.GetOldState());
    //PlayerRenderers.emplace(std::piecewise_construct, std::forward_as_tuple(PlayerId), std::forward_as_tuple(player.GetState(), player.GetOldState()));
    LoadWorld();
    for (const auto& pair : State.GetState().Chunks)
    {
        ChunkRenderers.emplace(pair.first, pair.second.GetState());
    }
    //PlayerId = (new PlayerController(RandomEngineState.GetNext(width), static_cast<float>(height + 3), RandomEngineState.GetNext(depth)))->GetEntityId();
    /*for (uint16_t i = 1; i <= 10; i++)
    {
        new Zombie(RandomEngineState.GetNext(width), static_cast<float>(LevelHeight + 3), RandomEngineState.GetNext(depth));
    }*/
}

void SinglePlayerWorld::Run()
{
    int i;
    for (i = 0; i < static_cast<int>(TickTimer / EngineDefaults::TICK_RATE); i++)
    {
        NewTick();
    }
    TickTimer -= static_cast<float>(i) * EngineDefaults::TICK_RATE;
    DrawWorld(TickTimer / EngineDefaults::TICK_RATE);
}

void SinglePlayerWorld::NewTick()
{
    State.ClearAllChanges();
    ClientWorld::NewTick();
}

void SinglePlayerWorld::HandlePacket(const PacketData* /*packet*/)
{
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
