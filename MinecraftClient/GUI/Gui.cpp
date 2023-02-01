#include "Gui.h"

World* Gui::TheWorld = nullptr;

void Gui::SetWorld(World* newWorld)
{
    TheWorld = newWorld;
}