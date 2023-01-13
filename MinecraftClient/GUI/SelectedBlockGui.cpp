#include "SelectedBlockGui.h"
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
    const float fWidth = static_cast<float>(width) * 240.0F / static_cast<float>(height);
    constexpr auto fHeight = 240.0F;
    mat4x4 orthoMatrix = glm::ortho(0.0F, fWidth, fHeight, 0.0F, -100.0F, 100.0F);
    orthoMatrix = translate(orthoMatrix, vec3(fWidth - 16.0F, 16.0F, 0.0F));
    orthoMatrix = scale(orthoMatrix, vec3(16.0F, 16.0F, 16.0F));
    orthoMatrix = rotate(orthoMatrix, radians(30.0F), vec3(1.0F, 0.0F, 0.0F));
    orthoMatrix = rotate(orthoMatrix, radians(45.0F), vec3(0.0F, 1.0F, 0.0F));
    orthoMatrix = translate(orthoMatrix, vec3(-1.5F, 0.5F, -0.5F));
    orthoMatrix = scale(orthoMatrix, vec3(-1.0F, -1.0F, 1.0F));
    const Block* block = BlockTypeList::GetBlockTypeData(CurrentBlockType);
    if (block->GetDrawType() == DrawType::Default)
    {
        block->GenerateTessellationData(Tessellation, BlockFaces::South, orthoMatrix);
        block->GenerateTessellationData(Tessellation, BlockFaces::West, orthoMatrix);
        block->GenerateTessellationData(Tessellation, BlockFaces::Top, orthoMatrix);
    }
    if (block->GetDrawType() == DrawType::Sapling)
    {
        block->GenerateTessellationData(Tessellation, BlockFaces::South, orthoMatrix);
    }
}
