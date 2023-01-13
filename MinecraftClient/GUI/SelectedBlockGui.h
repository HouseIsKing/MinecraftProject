#pragma once
#include "Gui.h"
#include "World/Blocks/BlockTypeList.h"

class SelectedBlockGui final : public Gui
{
    EBlockType CurrentBlockType;

public:
    void SwitchBlockType(EBlockType blockType);
    SelectedBlockGui();
    void Render() override;
    void Rebuild() override;
};
