#pragma once
#include "Util/GuiTessellation.h"

class SinglePlayerWorld;

class Gui
{
    static SinglePlayerWorld* World;
protected:
    GuiTessellation Tessellation;
    Gui() = default;
    static SinglePlayerWorld* GetWorld();
public:
    bool Active = false;
    static void SetWorld(SinglePlayerWorld* newWorld);
    virtual ~Gui() = default;
    Gui(const Gui&) = delete;
    Gui& operator=(const Gui&) = delete;
    Gui(Gui&&) = delete;
    Gui& operator=(Gui&&) = delete;
    virtual void Render() = 0;
    virtual void Rebuild() = 0;
};
