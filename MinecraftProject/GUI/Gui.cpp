#include "Gui.h"

SinglePlayerWorld* Gui::World = nullptr;

SinglePlayerWorld* Gui::GetWorld()
{
    return World;
}

void Gui::SetWorld(SinglePlayerWorld* newWorld)
{
    World = newWorld;
}
