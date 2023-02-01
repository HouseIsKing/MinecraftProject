#pragma once
#include <cstdint>

struct LightState
{
    uint16_t X;
    uint16_t Y;
    uint8_t LightValue;
    bool operator<(const LightState& rhs) const;
};

inline bool LightState::operator<(const LightState& rhs) const
{
    if (X < rhs.X)
    {
        return true;
    }
    if (rhs.X < X)
    {
        return false;
    }
    return Y < rhs.Y;
}
