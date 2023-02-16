#include "SinglePlayerWorld.h"
#include "Util/CustomFileManager.h"
#include "Util/EngineDefaults.h"
#include "Util/PerlinNoise.h"
#include <filesystem>
#include <iostream>
#include <memory>

#include "Entities/Generic/Zombie.h"

SinglePlayerWorld::SinglePlayerWorld(const uint16_t width, const uint16_t height, const uint16_t depth, GLFWwindow* window) : ClientWorld(window, height, width, depth)
{
    PlayerId = 0;
    PlayerState state{};
    state.EntityId = PlayerId;
    state.EntityTransform.Position = glm::vec3(TickRandomEngine.GetNextFloat() * 256.0F, 67.0F, TickRandomEngine.GetNextFloat() * 256.0F);
    State.AddEntity(&state);
    ClientWorld::EntityAdded(PlayerId);
    //PlayerRenderers.emplace(std::piecewise_construct, std::forward_as_tuple(PlayerId), std::forward_as_tuple(player.GetState(), player.GetOldState()));
    LoadWorld();
    LastTimeFrame = static_cast<float>(glfwGetTime());
    //PlayerId = (new PlayerController(RandomEngineState.GetNext(width), static_cast<float>(height + 3), RandomEngineState.GetNext(depth)))->GetEntityId();
    /*for (uint16_t i = 1; i <= 10; i++)
    {
        new Zombie(RandomEngineState.GetNext(width), static_cast<float>(LevelHeight + 3), RandomEngineState.GetNext(depth));
    }*/
}

void SinglePlayerWorld::NewTick()
{
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
