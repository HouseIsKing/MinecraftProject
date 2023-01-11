#include "CrosshairGui.h"
#include "Util/EngineDefaults.h"
#include "World/SinglePlayerWorld.h"

CrosshairGui::CrosshairGui() : TextureIndex(EngineDefaults::RegisterTexture(Texture::LoadTexture("Textures/HighlightTexture.png")))
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
    int width = 0;
    int height = 0;
    glfwGetWindowSize(GetWorld()->GetWindow(), &width, &height);
    const float fWidth = static_cast<float>(width) * 240.0F / 2.0F / static_cast<float>(height);
    constexpr float fHeight = 240.0F / 2.0F;
    uint16_t index1 = Tessellation.AddVertex(GuiVertex(-4.0F / fWidth, -0.0F / fHeight, 0.0F, 0.0F, 0.0F, 1.0F, 1.0F, 1.0F, 1.0F, TextureIndex));
    uint16_t index2 = Tessellation.AddVertex(GuiVertex(5.0F / fWidth, -0.0F / fHeight, 0.0F, 1.0F, 0.0F, 1.0F, 1.0F, 1.0F, 1.0F, TextureIndex));
    uint16_t index3 = Tessellation.AddVertex(GuiVertex(-4.0F / fWidth, 1.0F / fHeight, 0.0F, 0.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, TextureIndex));
    uint16_t index4 = Tessellation.AddVertex(GuiVertex(5.0F / fWidth, 1.0F / fHeight, 0.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, TextureIndex));
    Tessellation.AddTriangle(index1);
    Tessellation.AddTriangle(index2);
    Tessellation.AddTriangle(index3);
    Tessellation.AddTriangle(index4);
    Tessellation.AddTriangle(index3);
    Tessellation.AddTriangle(index2);
    index1 = Tessellation.AddVertex(GuiVertex(-0.0F / fWidth, -4.0F / fHeight, 0.0F, 0.0F, 0.0F, 1.0F, 1.0F, 1.0F, 1.0F, TextureIndex));
    index2 = Tessellation.AddVertex(GuiVertex(1.0F / fWidth, -4.0F / fHeight, 0.0F, 1.0F, 0.0F, 1.0F, 1.0F, 1.0F, 1.0F, TextureIndex));
    index3 = Tessellation.AddVertex(GuiVertex(-0.0F / fWidth, 5.0F / fHeight, 0.0F, 0.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, TextureIndex));
    index4 = Tessellation.AddVertex(GuiVertex(1.0F / fWidth, 5.0F / fHeight, 0.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, TextureIndex));
    Tessellation.AddTriangle(index1);
    Tessellation.AddTriangle(index2);
    Tessellation.AddTriangle(index3);
    Tessellation.AddTriangle(index4);
    Tessellation.AddTriangle(index3);
    Tessellation.AddTriangle(index2);
}
