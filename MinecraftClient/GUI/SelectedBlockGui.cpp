#include "SelectedBlockGui.h"
#include <GLFW/glfw3.h>

#include "World/Blocks/BlockTypeList.h"
#include "World/Generic/ClientWorld.h"
#include "World/Generic/Blocks/BlockRendererList.h"

SelectedBlockGui::SelectedBlockGui(const PlayerState& player) : State(player)
{
}

void SelectedBlockGui::Render()
{
    if (!this->Active)
    {
        return;
    }
    if (this->Tessellation.HasInitialized())
    {
        this->Tessellation.FreeMemory();
    }
    this->Tessellation.Draw();
}

void SelectedBlockGui::Rebuild()
{
    if (this->Tessellation.HasInitialized())
    {
        this->Tessellation.Reset();
    }
    int width = 0;
    int height = 0;
    glfwGetWindowSize(ClientWorld::GetWorld()->GetWindow(), &width, &height);
    const float fWidth = static_cast<float>(width) * 240.0F / static_cast<float>(height);
    constexpr auto fHeight = 240.0F;
    glm::mat4x4 orthoMatrix = glm::ortho(0.0F, fWidth, fHeight, 0.0F, -100.0F, 100.0F);
    orthoMatrix = translate(orthoMatrix, glm::vec3(fWidth - 16.0F, 16.0F, 0.0F));
    orthoMatrix = scale(orthoMatrix, glm::vec3(16.0F, 16.0F, 16.0F));
    orthoMatrix = rotate(orthoMatrix, glm::radians(30.0F), glm::vec3(1.0F, 0.0F, 0.0F));
    orthoMatrix = rotate(orthoMatrix, glm::radians(45.0F), glm::vec3(0.0F, 1.0F, 0.0F));
    orthoMatrix = translate(orthoMatrix, glm::vec3(-1.5F, 0.5F, -0.5F));
    orthoMatrix = scale(orthoMatrix, glm::vec3(-1.0F, -1.0F, 1.0F));
    const Block* block = BlockTypeList::GetBlockTypeData(State.CurrentSelectedBlock);
    const BlockRenderer* blockRenderer = BlockRendererList::GetBlockRenderer(State.CurrentSelectedBlock);
    if (blockRenderer->GetDrawType() == DrawType::Default)
    {
        blockRenderer->GenerateTessellationData(block, Tessellation, BlockFaces::South, orthoMatrix);
        blockRenderer->GenerateTessellationData(block, Tessellation, BlockFaces::West, orthoMatrix);
        blockRenderer->GenerateTessellationData(block, Tessellation, BlockFaces::Top, orthoMatrix);
    }
    if (blockRenderer->GetDrawType() == DrawType::Sapling)
    {
        blockRenderer->GenerateTessellationData(block, Tessellation, BlockFaces::South, orthoMatrix);
    }
}
