#pragma once
#include "Gui.h"
#include "World/Generic/Blocks/BlockTypeList.h"
#include <glm/gtx/transform.hpp>

class SelectedBlockGui final : public Gui
{
    EBlockType CurrentBlockType;

public:
    void SwitchBlockType(EBlockType blockType);
    SelectedBlockGui();
    void Render() override;
    void Rebuild() override;
};
