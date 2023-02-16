#include "ClientWorld.h"

#include "Blocks/BlockRendererList.h"
#include "Entities/Generic/CameraController.h"
#include "GUI/CrosshairGui.h"
#include "GUI/PerformanceGui.h"
#include "GUI/SelectedBlockGui.h"
#include "Util/EngineDefaultsClient.h"

ClientWorld* ClientWorld::TheWorld = nullptr;

void ClientWorld::AddGui(Gui* gui)
{
    Guis.emplace_back(gui);
}

void ClientWorld::HandleKeyCallback(const int key, const int action)
{
    switch (key)
    {
    case GLFW_KEY_W:
        if (action == GLFW_PRESS)
        {
            PlayerInput.Input.SetKey(EKeySet::Up, true);
        }
        else if (action == GLFW_RELEASE)
        {
            PlayerInput.Input.SetKey(EKeySet::Up, false);
        }
        break;
    case GLFW_KEY_S:
        if (action == GLFW_PRESS)
        {
            PlayerInput.Input.SetKey(EKeySet::Down, true);
        }
        else if (action == GLFW_RELEASE)
        {
            PlayerInput.Input.SetKey(EKeySet::Down, false);
        }
        break;
    case GLFW_KEY_A:
        if (action == GLFW_PRESS)
        {
            PlayerInput.Input.SetKey(EKeySet::Left, true);
        }
        else if (action == GLFW_RELEASE)
        {
            PlayerInput.Input.SetKey(EKeySet::Left, false);
        }
        break;
    case GLFW_KEY_D:
        if (action == GLFW_PRESS)
        {
            PlayerInput.Input.SetKey(EKeySet::Right, true);
        }
        else if (action == GLFW_RELEASE)
        {
            PlayerInput.Input.SetKey(EKeySet::Right, false);
        }
        break;
    case GLFW_KEY_SPACE:
        if (action == GLFW_PRESS)
        {
            PlayerInput.Input.SetKey(EKeySet::Jump, true);
        }
        else if (action == GLFW_RELEASE)
        {
            PlayerInput.Input.SetKey(EKeySet::Jump, false);
        }
        break;
    case GLFW_KEY_1:
        if (action == GLFW_PRESS)
        {
            PlayerInput.Input.SetKey(EKeySet::One, true);
        }
        else if (action == GLFW_RELEASE)
        {
            PlayerInput.Input.SetKey(EKeySet::One, false);
        }
        break;
    case GLFW_KEY_2:
        if (action == GLFW_PRESS)
        {
            PlayerInput.Input.SetKey(EKeySet::Two, true);
        }
        else if (action == GLFW_RELEASE)
        {
            PlayerInput.Input.SetKey(EKeySet::Two, false);
        }
        break;
    case GLFW_KEY_3:
        if (action == GLFW_PRESS)
        {
            PlayerInput.Input.SetKey(EKeySet::Three, true);
        }
        else if (action == GLFW_RELEASE)
        {
            PlayerInput.Input.SetKey(EKeySet::Three, false);
        }
        break;
    case GLFW_KEY_4:
        if (action == GLFW_PRESS)
        {
            PlayerInput.Input.SetKey(EKeySet::Four, true);
        }
        else if (action == GLFW_RELEASE)
        {
            PlayerInput.Input.SetKey(EKeySet::Four, false);
        }
        break;
    case GLFW_KEY_5:
        if (action == GLFW_PRESS)
        {
            PlayerInput.Input.SetKey(EKeySet::Five, true);
        }
        else if (action == GLFW_RELEASE)
        {
            PlayerInput.Input.SetKey(EKeySet::Five, false);
        }
        break;
    case GLFW_KEY_R:
        if (action == GLFW_PRESS)
        {
            PlayerInput.Input.SetKey(EKeySet::Reset, true);
        }
        else if (action == GLFW_RELEASE)
        {
            PlayerInput.Input.SetKey(EKeySet::Reset, false);
        }
        break;
    case GLFW_KEY_G:
        if (action == GLFW_PRESS)
        {
            PlayerInput.Input.SetKey(EKeySet::SpawnZombie, true);
        }
        else if (action == GLFW_RELEASE)
        {
            PlayerInput.Input.SetKey(EKeySet::SpawnZombie, false);
        }
        break;
    default:
        break;
    }
}

void ClientWorld::HandleCursorPosCallback(const float xPos, const float yPos)
{
    PlayerInput.Input.MouseX += (xPos - PlayerInput.PrevMouseX) * EngineDefaultsClient::MOUSE_SENSITIVITY;
    PlayerInput.Input.MouseY += (yPos - PlayerInput.PrevMouseY) * EngineDefaultsClient::MOUSE_SENSITIVITY;
    PlayerInput.PrevMouseX = xPos;
    PlayerInput.PrevMouseY = yPos;
}

void ClientWorld::HandleMouseButtonCallback(const int button, const int action)
{
    switch (button)
    {
    case GLFW_MOUSE_BUTTON_LEFT:
        if (action == GLFW_PRESS)
        {
            PlayerInput.Input.SetKey(EKeySet::LeftMouseButton, true);
        }
        else if (action == GLFW_RELEASE)
        {
            PlayerInput.Input.SetKey(EKeySet::LeftMouseButton, false);
        }
        break;
    case GLFW_MOUSE_BUTTON_RIGHT:
        if (action == GLFW_PRESS)
        {
            PlayerInput.Input.SetKey(EKeySet::RightMouseButton, true);
        }
        else if (action == GLFW_RELEASE)
        {
            PlayerInput.Input.SetKey(EKeySet::RightMouseButton, false);
        }
        break;
    default:
        break;
    }
}

ClientWorld* ClientWorld::GetWorld()
{
    return TheWorld;
}

void ClientWorld::Init()
{
    Shader::SetUnsignedInt(EngineDefaultsClient::GetShader()->GetUniformInt("worldTime"), static_cast<GLuint>(State.GetState().WorldTime % 24000L));
    InitFog();
    BlockRendererList::InitBlockTypes();
    EngineDefaultsClient::InitTextures();
    LastTimeFrame = static_cast<float>(glfwGetTime());
}

void ClientWorld::InitFog()
{
    const std::array fogs{14.0F / 255.0F, 11.0F / 255.0F, 10.0F / 255.0F, 1.0F, 0.01F, 0.0F, 0.0F, 0.0F, 254.0F / 255.0F, 251.0F / 255.0F, 250.0F / 255.0F, 1.0F, 0.001F, 0.0F, 0.0F, 0.0F};
    glGenBuffers(1, &FogsBuffer);
    glBindBuffer(GL_UNIFORM_BUFFER, FogsBuffer);
    glBufferData(GL_UNIFORM_BUFFER, static_cast<GLintptr>(fogs.size() * sizeof(float)), fogs.data(), GL_STATIC_COPY);
    glBindBufferRange(GL_UNIFORM_BUFFER, 1, FogsBuffer, 0, static_cast<GLintptr>(fogs.size() * sizeof(float)));
    glUniformBlockBinding(EngineDefaultsClient::GetShader()->Program, EngineDefaultsClient::GetShader()->GetUniformBlockIndex("fogBlock"), 1);
}

void ClientWorld::DrawGui() const
{
    Shader::SetUnsignedInt(EngineDefaultsClient::GetShader(1)->GetUniformInt("worldTime"), static_cast<GLuint>(State.GetState().WorldTime % 24000L));
    for (const auto& gui : Guis)
    {
        gui->Render();
    }
}

void ClientWorld::RebuildGui() const
{
    for (const auto& gui : Guis)
    {
        gui->Rebuild();
    }
}

void ClientWorld::DrawWorld(const float partialTick)
{
    Shader::SetUnsignedInt(EngineDefaultsClient::GetShader(0)->GetUniformInt("worldTime"), static_cast<GLuint>(State.GetState().WorldTime % 24000L));
    glEnable(GL_CULL_FACE);
    if (LocalPlayerRenderer)
    {
        LocalPlayerRenderer->Render(partialTick);
        const Frustum frustum = LocalPlayerRenderer->GetCameraFrustum();
        std::ranges::sort(DirtyChunks, DirtyChunkComparator{frustum});
        for (int i = 0; i < MaxChunkRebuilt && !DirtyChunks.empty(); i++)
        {
            DirtyChunks[0]->GenerateTessellationData();
            DirtyChunksDuplicatorCheck.erase(DirtyChunks[0]);
            DirtyChunks.erase(DirtyChunks.begin());
        }
        for (auto& chunk : ChunkRenderers | std::views::values)
        {
            if (chunk.ChunkInFrustum(frustum))
            {
                chunk.Draw();
            }
        }
        for (auto& blockParticle : BlockParticleEntityRenderers)
        {
            if (frustum.CubeInFrustum(State.GetState().BlockParticleEntities.at(blockParticle.first)->GetBoundingBox()))
            {
                blockParticle.second.Render(partialTick);
            }
        }
        LocalPlayerRenderer->RenderSelectionHighlight();
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

ClientWorld::ClientWorld(GLFWwindow* window, const uint16_t levelHeight, const uint16_t levelWidth, const uint16_t levelDepth) : World(levelWidth, levelHeight, levelDepth), PlayerId(0), TheAppWindow(window)
{
    TheWorld = this;
    Guis.emplace_back(new CrosshairGui());
    Guis[0]->Active = true;
    Guis.emplace_back(new PerformanceGui());
    Guis[1]->Active = true;
    Init();
}

GLFWwindow* ClientWorld::GetWindow() const
{
    return TheAppWindow;
}

int ClientWorld::GetFps() const
{
    return Fps;
}

const Player* ClientWorld::GetPlayer() const
{
    return State.GetState().Players.at(PlayerId);
}

void ClientWorld::ChunkChanged(const ChunkCoords& coords)
{
    if (ChunkRenderer* chunk = &ChunkRenderers.at(coords); DirtyChunksDuplicatorCheck.insert(chunk).second)
    {
        chunk->GotDirty();
        DirtyChunks.push_back(chunk);
    }
}

void ClientWorld::ChunkAdded(const ChunkCoords& coords)
{
    ChunkRenderers.emplace(coords, State.GetChunk(coords)->GetState());
    ChunkChanged(coords);
}

void ClientWorld::ChunkRemoved(const ChunkCoords& coords)
{
    //ChunkRenderers.erase(coords);
}

void ClientWorld::EntityAdded(uint16_t entityId)
{
    if (entityId == PlayerId)
    {
        const Entity<PlayerStateWrapper, PlayerState>* player = State.GetEntity<PlayerStateWrapper, PlayerState>(entityId);
        LocalPlayerRenderer = std::make_unique<ClientPlayerRenderer>(player->GetState(), player->GetOldState());
    }
    else
    {
        switch (State.GetEntityIdToType().at(entityId))
        {
        case EEntityType::Player:
            {
                const Entity<PlayerStateWrapper, PlayerState>* player = State.GetEntity<PlayerStateWrapper, PlayerState>(entityId);
                PlayerRenderers.emplace(std::piecewise_construct, std::forward_as_tuple(entityId), std::forward_as_tuple(player->GetState(), player->GetOldState()));
                break;
            }
        case EEntityType::BlockBreakParticle:
            {
                const Entity<BlockParticleEntityStateWrapper, BlockParticleEntityState>* blockParticle = State.GetEntity<BlockParticleEntityStateWrapper, BlockParticleEntityState>(entityId);
                BlockParticleEntityRenderers.emplace(std::piecewise_construct, std::forward_as_tuple(entityId), std::forward_as_tuple(blockParticle->GetState(), blockParticle->GetOldState()));
                break;
            }
        case EEntityType::Zombie:
            break;
        }
    }
}

void ClientWorld::EntityChanged(uint16_t entityId)
{
    if (entityId == PlayerId)
    {
        LocalPlayerRenderer->Changed();
    }
    else
    {
        switch (State.GetEntityIdToType().at(entityId))
        {
        case EEntityType::Player:
            PlayerRenderers.at(entityId).Changed();
            break;
        case EEntityType::BlockBreakParticle:
            BlockParticleEntityRenderers.at(entityId).Changed();
            break;
        case EEntityType::Zombie:
            break;
        }
    }
}

void ClientWorld::EntityRemoved(uint16_t entityId)
{
    switch (State.GetEntityIdToType().at(entityId))
    {
    case EEntityType::Player:
        PlayerRenderers.erase(entityId);
        break;
    case EEntityType::BlockBreakParticle:
        BlockParticleEntityRenderers.erase(entityId);
        break;
    case EEntityType::Zombie:
        break;
    }
}


void ClientWorld::HandleWindowResize(const int height, const int width) const
{
    glViewport(0, 0, width, height);
    CameraController::OnResizeWindow(width, height);
    RebuildGui();
}

void ClientWorld::NewTick()
{
    for (auto it = State.GetPlayersIterator(); it != State.GetState().Players.end(); ++it)
    {
        it->second->NewTick();
    }
    dynamic_cast<Player*>(State.GetEntity<PlayerStateWrapper, PlayerState>(PlayerId))->SetClientInput(State.GetState().WorldTime + 1, PlayerInput.Input);
    Tick();
    PlayerInput.Input.MouseX = 0;
    PlayerInput.Input.MouseY = 0;
}

void ClientWorld::Run()
{
    int i;
    for (i = 0; i < static_cast<int>(TickTimer / EngineDefaults::TICK_RATE); i++)
    {
        NewTick();
    }
    TickTimer -= static_cast<float>(i) * EngineDefaults::TICK_RATE;
    DrawWorld(TickTimer / EngineDefaults::TICK_RATE);
}
