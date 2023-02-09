#include "ClientWorld.h"

#include "Blocks/BlockRendererList.h"
#include "GUI/CrosshairGui.h"
#include "GUI/PerformanceGui.h"
#include "GUI/SelectedBlockGui.h"
#include "Util/EngineDefaultsClient.h"

ClientWorld* ClientWorld::TheWorld = nullptr;

void ClientWorld::HandleKeyCallback(const int key, const int action)
{
    switch (key)
    {
    case GLFW_KEY_W:
        if (action == GLFW_PRESS)
        {
            PlayerInput.Input.ForwardAxis++;
        }
        else if (action == GLFW_RELEASE)
        {
            PlayerInput.Input.ForwardAxis--;
        }
        break;
    case GLFW_KEY_S:
        if (action == GLFW_PRESS)
        {
            PlayerInput.Input.ForwardAxis--;
        }
        else if (action == GLFW_RELEASE)
        {
            PlayerInput.Input.ForwardAxis++;
        }
        break;
    case GLFW_KEY_A:
        if (action == GLFW_PRESS)
        {
            PlayerInput.Input.RightAxis--;
        }
        else if (action == GLFW_RELEASE)
        {
            PlayerInput.Input.RightAxis++;
        }
        break;
    case GLFW_KEY_D:
        if (action == GLFW_PRESS)
        {
            PlayerInput.Input.RightAxis++;
        }
        else if (action == GLFW_RELEASE)
        {
            PlayerInput.Input.RightAxis--;
        }
        break;
    case GLFW_KEY_SPACE:
        if (action == GLFW_PRESS)
        {
            PlayerInput.Input.JumpPressed = true;
        }
        else if (action == GLFW_RELEASE)
        {
            PlayerInput.Input.JumpPressed = false;
        }
        break;
    case GLFW_KEY_1:
        if (action == GLFW_PRESS)
        {
            PlayerInput.Input.OnePressed = true;
        }
        else if (action == GLFW_RELEASE)
        {
            PlayerInput.Input.OnePressed = false;
        }
        break;
    case GLFW_KEY_2:
        if (action == GLFW_PRESS)
        {
            PlayerInput.Input.TwoPressed = true;
        }
        else if (action == GLFW_RELEASE)
        {
            PlayerInput.Input.TwoPressed = false;
        }
        break;
    case GLFW_KEY_3:
        if (action == GLFW_PRESS)
        {
            PlayerInput.Input.ThreePressed = true;
        }
        else if (action == GLFW_RELEASE)
        {
            PlayerInput.Input.ThreePressed = false;
        }
        break;
    case GLFW_KEY_4:
        if (action == GLFW_PRESS)
        {
            PlayerInput.Input.FourPressed = true;
        }
        else if (action == GLFW_RELEASE)
        {
            PlayerInput.Input.FourPressed = false;
        }
        break;
    case GLFW_KEY_5:
        if (action == GLFW_PRESS)
        {
            PlayerInput.Input.FivePressed = true;
        }
        else if (action == GLFW_RELEASE)
        {
            PlayerInput.Input.FivePressed = false;
        }
        break;
    case GLFW_KEY_R:
        if (action == GLFW_PRESS)
        {
            PlayerInput.Input.ResetPressed = true;
        }
        else if (action == GLFW_RELEASE)
        {
            PlayerInput.Input.ResetPressed = false;
        }
        break;
    case GLFW_KEY_G:
        if (action == GLFW_PRESS)
        {
            PlayerInput.Input.SpawnZombiePressed = true;
        }
        else if (action == GLFW_RELEASE)
        {
            PlayerInput.Input.SpawnZombiePressed = false;
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
            PlayerInput.Input.LeftMouseButtonPressed = true;
        }
        else if (action == GLFW_RELEASE)
        {
            PlayerInput.Input.LeftMouseButtonPressed = false;
        }
        break;
    case GLFW_MOUSE_BUTTON_RIGHT:
        if (action == GLFW_PRESS)
        {
            PlayerInput.Input.RightMouseButtonPressed = true;
        }
        else if (action == GLFW_RELEASE)
        {
            PlayerInput.Input.RightMouseButtonPressed = false;
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
        /*for (const std::unique_ptr<Entity>& entity : Entities | std::views::values)
        {
            if (entity->GetEntityId() != PlayerId && frustum.CubeInFrustum(entity->GetBoundingBox()))
            {
                entity->Render(partialTick);
            }
        }*/
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
        //player->DisplaySelectionHighlight();
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
    Guis.emplace_back(new SelectedBlockGui());
    Guis[1]->Active = true;
    Guis.emplace_back(new PerformanceGui());
    Guis[2]->Active = true;
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

const Player& ClientWorld::GetPlayer() const
{
    return State.GetState().Players.at(PlayerId);
}


void ClientWorld::HandleWindowResize(const int height, const int width) const
{
    glViewport(0, 0, width, height);
    CameraController::OnResizeWindow(width, height);
    RebuildGui();
}

void ClientWorld::AddChunkAsDirty(ChunkRenderer* chunk)
{
    if (DirtyChunksDuplicatorCheck.insert(chunk).second)
    {
        chunk->GotDirty();
        DirtyChunks.push_back(chunk);
    }
}

void ClientWorld::NewTick()
{
    State.GetPlayer(PlayerId).SetClientInput(State.GetState().WorldTime + 1, PlayerInput.Input);
    Tick();
    PlayerInput.Input.MouseX = 0;
    PlayerInput.Input.MouseY = 0;
}

void ClientWorld::UpdateChunksNear(const int x, const int y, const int z)
{
    const Chunk* chunk = GetChunkAt(x, y + 1, z);
    if (chunk != nullptr)
    {
        AddChunkAsDirty(&ChunkRenderers.at(chunk->GetState().ChunkPosition));
    }
    chunk = GetChunkAt(x, y - 1, z);
    if (chunk != nullptr)
    {
        AddChunkAsDirty(&ChunkRenderers.at(chunk->GetState().ChunkPosition));
    }
    chunk = GetChunkAt(x, y, z + 1);
    if (chunk != nullptr)
    {
        AddChunkAsDirty(&ChunkRenderers.at(chunk->GetState().ChunkPosition));
    }
    chunk = GetChunkAt(x, y, z - 1);
    if (chunk != nullptr)
    {
        AddChunkAsDirty(&ChunkRenderers.at(chunk->GetState().ChunkPosition));
    }
    chunk = GetChunkAt(x + 1, y, z);
    if (chunk != nullptr)
    {
        AddChunkAsDirty(&ChunkRenderers.at(chunk->GetState().ChunkPosition));
    }
    chunk = GetChunkAt(x - 1, y, z);
    if (chunk != nullptr)
    {
        AddChunkAsDirty(&ChunkRenderers.at(chunk->GetState().ChunkPosition));
    }
}
