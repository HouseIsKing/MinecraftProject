#pragma once
#include "Gui.h"

class CrosshairGui final : public Gui
{
public:
    CrosshairGui();
    void Render() override;
    void Rebuild() override;
};
