#include "SelectedBlockGui.h"

#include <GLFW/glfw3.h>

#include "Util/Transform.h"
#include "World/SinglePlayerWorld.h"

void SelectedBlockGui::SwitchBlockType(const EBlockType blockType)
{
    CurrentBlockType = blockType;
    Rebuild();
}

SelectedBlockGui::SelectedBlockGui() : CurrentBlockType(EBlockType::Cobblestone)
{
    Rebuild();
}

void SelectedBlockGui::Render()
{
    if (!Active)
    {
        return;
    }
    if (Tessellation.HasInitialized())
    {
        Tessellation.FreeMemory();
    }
    Tessellation.Draw();
}

void SelectedBlockGui::Rebuild()
{
    if (Tessellation.HasInitialized())
    {
        Tessellation.Reset();
    }
    int width = 0;
    int height = 0;
    glfwGetWindowSize(GetWorld()->GetWindow(), &width, &height);
    const auto fWidth = static_cast<float>(width);
    const auto fHeight = static_cast<float>(height);
    mat4x4 orthoMatrix = glm::ortho(0.0F, fWidth, fHeight, 0.0F, -100.0F, 100.0F);
    orthoMatrix = translate(orthoMatrix, vec3(fWidth - 48.0F, 48.0F, 0.0F));
    orthoMatrix = scale(orthoMatrix, vec3(48.0F, 48.0F, 48.0F));
    orthoMatrix = rotate(orthoMatrix, radians(30.0F), vec3(1.0F, 0.0F, 0.0F));
    orthoMatrix = rotate(orthoMatrix, radians(45.0F), vec3(0.0F, 1.0F, 0.0F));
    orthoMatrix = translate(orthoMatrix, vec3(1.5F, -0.5F, -0.5F));
    const Block* block = BlockTypeList::GetBlockTypeData(CurrentBlockType);
    block->GenerateTessellationData(Tessellation, BlockFaces::North, orthoMatrix);
    block->GenerateTessellationData(Tessellation, BlockFaces::East, orthoMatrix);
    block->GenerateTessellationData(Tessellation, BlockFaces::South, orthoMatrix);
    block->GenerateTessellationData(Tessellation, BlockFaces::West, orthoMatrix);
    block->GenerateTessellationData(Tessellation, BlockFaces::Top, orthoMatrix);
    block->GenerateTessellationData(Tessellation, BlockFaces::Bottom, orthoMatrix);
}
