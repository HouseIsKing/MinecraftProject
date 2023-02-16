#pragma once
#include "Gui.h"
#include "Util/EngineDefaults.h"
#include "Util/States/PlayerState.h"

class SelectedBlockGui final : public Gui
{
    const PlayerState& State;

public:
    explicit SelectedBlockGui(const PlayerState& player);
    void Render() override;
    void Rebuild() override;
};
