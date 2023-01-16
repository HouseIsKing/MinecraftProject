#include "EntityPositionPacket.h"

EntityPositionPacket::EntityPositionPacket(Packet& packet) : Id(0), X(0), Y(0), Z(0)
{
    packet >> Id >> X >> Y >> Z;
}
