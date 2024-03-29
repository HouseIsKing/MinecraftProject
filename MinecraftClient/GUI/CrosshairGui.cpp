#include "CrosshairGui.h"
#include "Util/EngineDefaultsClient.h"
#include "World/Generic/ClientWorld.h"

CrosshairGui::CrosshairGui() : TextureIndex(EngineDefaultsClient::RegisterTexture(Texture::LoadTexture("Textures/HighlightTexture.png")))
{
    Rebuild();
}

void CrosshairGui::Render()
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

void CrosshairGui::Rebuild()
{
    if (this->Tessellation.HasInitialized())
    {
        this->Tessellation.Reset();
    }
    int width = 0;
    int height = 0;
    glfwGetWindowSize(ClientWorld::GetWorld()->GetWindow(), &width, &height);
    const float fWidth = static_cast<float>(width) * 240.0F / 2.0F / static_cast<float>(height);
    constexpr float fHeight = 240.0F / 2.0F;
    uint16_t index1 = this->Tessellation.AddVertex(GuiVertex(-4.0F / fWidth, -0.0F / fHeight, 0.0F, 0.0F, 0.0F, 1.0F, 1.0F, 1.0F, 1.0F, TextureIndex));
    uint16_t index2 = this->Tessellation.AddVertex(GuiVertex(5.0F / fWidth, -0.0F / fHeight, 0.0F, 1.0F, 0.0F, 1.0F, 1.0F, 1.0F, 1.0F, TextureIndex));
    uint16_t index3 = this->Tessellation.AddVertex(GuiVertex(-4.0F / fWidth, 1.0F / fHeight, 0.0F, 0.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, TextureIndex));
    uint16_t index4 = this->Tessellation.AddVertex(GuiVertex(5.0F / fWidth, 1.0F / fHeight, 0.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, TextureIndex));
    this->Tessellation.AddTriangle(index1);
    this->Tessellation.AddTriangle(index2);
    this->Tessellation.AddTriangle(index3);
    this->Tessellation.AddTriangle(index4);
    this->Tessellation.AddTriangle(index3);
    this->Tessellation.AddTriangle(index2);
    index1 = this->Tessellation.AddVertex(GuiVertex(-0.0F / fWidth, -4.0F / fHeight, 0.0F, 0.0F, 0.0F, 1.0F, 1.0F, 1.0F, 1.0F, TextureIndex));
    index2 = this->Tessellation.AddVertex(GuiVertex(1.0F / fWidth, -4.0F / fHeight, 0.0F, 1.0F, 0.0F, 1.0F, 1.0F, 1.0F, 1.0F, TextureIndex));
    index3 = this->Tessellation.AddVertex(GuiVertex(-0.0F / fWidth, 5.0F / fHeight, 0.0F, 0.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, TextureIndex));
    index4 = this->Tessellation.AddVertex(GuiVertex(1.0F / fWidth, 5.0F / fHeight, 0.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, TextureIndex));
    this->Tessellation.AddTriangle(index1);
    this->Tessellation.AddTriangle(index2);
    this->Tessellation.AddTriangle(index3);
    this->Tessellation.AddTriangle(index4);
    this->Tessellation.AddTriangle(index3);
    this->Tessellation.AddTriangle(index2);
}
