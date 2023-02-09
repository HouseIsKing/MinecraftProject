#pragma once
#include "Gui.h"
#include "Util/EngineDefaults.h"

class SelectedBlockGui final : public Gui
{
    EBlockType CurrentBlockType;

public:
    void SwitchBlockType(EBlockType blockType);
    SelectedBlockGui();
    void Render() override;
    void Rebuild() override;
};
