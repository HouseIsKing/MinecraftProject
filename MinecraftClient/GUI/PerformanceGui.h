#pragma once
#include "Gui.h"
#include "Util/EngineDefaults.h"

template <typename WorldType>
class PerformanceGui final : public Gui<WorldType>
{
    const uint16_t FontTexture;
    constexpr static uint16_t FONT_WIDTH = 8;
    constexpr static uint16_t FONT_HEIGHT = 8;
    const std::string Version = "0.09a ";

    void DrawStringAt(const std::string& text, float x, float y, float z, float r, float g, float b, float a);

public:
    PerformanceGui();
    void Rebuild() override;
    void Render() override;
};


template <typename WorldType>
void PerformanceGui<WorldType>::DrawStringAt(const std::string& text, float x, float y, const float z, const float r, const float g, const float b, const float a)
{
    int width = 0;
    int height = 0;
    glfwGetWindowSize(Gui<WorldType>::GetWorld()->GetWindow(), &width, &height);
    const float fWidth = static_cast<float>(width) * 240.0F / 2.0F / static_cast<float>(height);
    constexpr float fHeight = 240.0F / 2.0F;
    x = -fWidth + x;
    y = fHeight - y - 8.0F;
    float offsetX = 0.0F;
    //float offsetY = 0.0F;
    for (const char c : text)
    {
        const float u = static_cast<float>(c % 16) / 16.0F;
        const float v = 0.939F - static_cast<float>(c >> 4) / 16.0F;
        const uint16_t index1 = this->Tessellation.AddVertex(GuiVertex((x + offsetX) / fWidth, y / fHeight, z, u, v, r, g, b, a, FontTexture));
        const uint16_t index2 = this->Tessellation.AddVertex(GuiVertex((x + offsetX + 8.0F) / fWidth, y / fHeight, z, u + 0.061F, v, r, g, b, a, FontTexture));
        const uint16_t index3 = this->Tessellation.AddVertex(GuiVertex((x + offsetX + 8.0F) / fWidth, (y + 8.0F) / fHeight, z, u + 0.061F, v + 0.061F, r, g, b, a, FontTexture));
        const uint16_t index4 = this->Tessellation.AddVertex(GuiVertex((x + offsetX) / fWidth, (y + 8.0F) / fHeight, z, u, v + 0.061F, r, g, b, a, FontTexture));
        this->Tessellation.AddTriangle(index1);
        this->Tessellation.AddTriangle(index2);
        this->Tessellation.AddTriangle(index3);
        this->Tessellation.AddTriangle(index4);
        this->Tessellation.AddTriangle(index1);
        this->Tessellation.AddTriangle(index3);
        offsetX += static_cast<float>(EngineDefaults::GetFontManager().GetCharacterWidth(c));
    }
}

template <typename WorldType>
PerformanceGui<WorldType>::PerformanceGui() : FontTexture(EngineDefaults::RegisterTexture(Texture::LoadTexture("Textures/Font/default.png")))
{
}

template <typename WorldType>
void PerformanceGui<WorldType>::Rebuild()
{
    if (this->Tessellation.HasInitialized())
    {
        this->Tessellation.Reset();
    }
    const std::string text = std::to_string(Gui<WorldType>::GetWorld()->GetFps()) + " FPS";
    DrawStringAt(Version, 3.0F, 3.0F, 0.0F, 0.25F, 0.25F, 0.25F, 1.0F);
    DrawStringAt(Version, 2.0F, 2.0F, 0.0F, 1.0F, 1.0F, 1.0F, 1.0F);
    DrawStringAt(text, 3.0F, 13.0F, 0.0F, 0.25F, 0.25F, 0.25F, 1.0F);
    DrawStringAt(text, 2.0F, 12.0F, 0.0F, 1.0F, 1.0F, 1.0F, 1.0F);
}

template <typename WorldType>
void PerformanceGui<WorldType>::Render()
{
    Rebuild();
    this->Tessellation.Draw();
}

