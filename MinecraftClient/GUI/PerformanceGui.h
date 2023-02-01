#pragma once
#include "Gui.h"
#include "Util/EngineDefaults.h"

class PerformanceGui final : public Gui
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
