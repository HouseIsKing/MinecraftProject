#include "AirBlock.h"

AirBlock::AirBlock() = default;

bool AirBlock::IsSolidBlock() const
{
    return false;
}

bool AirBlock::IsBlockingLight() const
{
    return false;
}
