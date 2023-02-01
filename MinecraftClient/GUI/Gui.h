#pragma once
#include "Util/GuiTessellation.h"

class World;

class Gui
{
protected:
    GuiTessellation Tessellation;
    Gui() = default;
    static World* TheWorld;

public:
    bool Active = false;
    static void SetWorld(World* newWorld);
    virtual ~Gui() = default;
    Gui(const Gui&) = delete;
    Gui& operator=(const Gui&) = delete;
    Gui(Gui&&) = delete;
    Gui& operator=(Gui&&) = delete;
    virtual void Render() = 0;
    virtual void Rebuild() = 0;
};

