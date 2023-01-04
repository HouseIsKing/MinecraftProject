#include "CrosshairGui.h"
#include "Util/EngineDefaults.h"
#include "World/SinglePlayerWorld.h"

CrosshairGui::CrosshairGui()
{
    Rebuild();
}

void CrosshairGui::Render()
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

void CrosshairGui::Rebuild()
{
    if (Tessellation.HasInitialized())
    {
        Tessellation.Reset();
    }
    const uint16_t index = EngineDefaults::RegisterTexture(Texture::LoadTexture("Textures/HighlightTexture.png"));
    int width = 0;
    int height = 0;
    glfwGetWindowSize(GetWorld()->GetWindow(), &width, &height);
    const float fWidth = static_cast<float>(width) / 2.0F;
    const float fHeight = static_cast<float>(height) / 2.0F;
    uint16_t index1 = Tessellation.AddVertex(GuiVertex(-8.0F / fWidth, -0.0F / fHeight, 0.0F, 0.0F, 0.0F, 1.0F, 1.0F, 1.0F, 1.0F, index));
    uint16_t index2 = Tessellation.AddVertex(GuiVertex(9.0F / fWidth, -0.0F / fHeight, 0.0F, 1.0F, 0.0F, 1.0F, 1.0F, 1.0F, 1.0F, index));
    uint16_t index3 = Tessellation.AddVertex(GuiVertex(-8.0F / fWidth, 1.0F / fHeight, 0.0F, 0.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, index));
    uint16_t index4 = Tessellation.AddVertex(GuiVertex(9.0F / fWidth, 1.0F / fHeight, 0.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, index));
    Tessellation.AddTriangle(index1);
    Tessellation.AddTriangle(index2);
    Tessellation.AddTriangle(index3);
    Tessellation.AddTriangle(index4);
    Tessellation.AddTriangle(index3);
    Tessellation.AddTriangle(index2);
    index1 = Tessellation.AddVertex(GuiVertex(-0.0F / fWidth, -8.0F / fHeight, 0.0F, 0.0F, 0.0F, 1.0F, 1.0F, 1.0F, 1.0F, index));
    index2 = Tessellation.AddVertex(GuiVertex(1.0F / fWidth, -8.0F / fHeight, 0.0F, 1.0F, 0.0F, 1.0F, 1.0F, 1.0F, 1.0F, index));
    index3 = Tessellation.AddVertex(GuiVertex(-0.0F / fWidth, 9.0F / fHeight, 0.0F, 0.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, index));
    index4 = Tessellation.AddVertex(GuiVertex(1.0F / fWidth, 9.0F / fHeight, 0.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, index));
    Tessellation.AddTriangle(index1);
    Tessellation.AddTriangle(index2);
    Tessellation.AddTriangle(index3);
    Tessellation.AddTriangle(index4);
    Tessellation.AddTriangle(index3);
    Tessellation.AddTriangle(index2);
}
