#pragma once
#include "Gui.h"

class PerformanceGui final : public Gui
{
    const uint16_t FontTexture;
    constexpr static uint16_t FONT_WIDTH = 8;
    constexpr static uint16_t FONT_HEIGHT = 8;
    const string Version = "0.02a ";

    void DrawStringAt(const string& text, float x, float y, float z, float r, float g, float b, float a);

public:
    PerformanceGui();
    void Rebuild() override;
    void Render() override;
};
