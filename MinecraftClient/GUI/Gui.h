#pragma once
#include "Util/GuiTessellation.h"

template <typename T>
class Gui
{
    static T* TheWorld;

protected:
    GuiTessellation Tessellation;
    Gui() = default;
    static T* GetWorld();

public:
    bool Active = false;
    static void SetWorld(T* newWorld);
    virtual ~Gui() = default;
    Gui(const Gui&) = delete;
    Gui& operator=(const Gui&) = delete;
    Gui(Gui&&) = delete;
    Gui& operator=(Gui&&) = delete;
    virtual void Render() = 0;
    virtual void Rebuild() = 0;
};

template <typename WorldType>
WorldType* Gui<WorldType>::TheWorld = nullptr;

template <typename WorldType>
WorldType* Gui<WorldType>::GetWorld()
{
    return TheWorld;
}

template <typename WorldType>
void Gui<WorldType>::SetWorld(WorldType* newWorld)
{
    TheWorld = newWorld;
}

