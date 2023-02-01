#pragma once
#include "Gui.h"
#include "Util/EngineDefaults.h"

class CrosshairGui final : public Gui
{
    const uint16_t TextureIndex;

public:
    CrosshairGui();
    void Render() override;
    void Rebuild() override;
};
