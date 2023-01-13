#include "PacketData.h"

PacketData::PacketData(ConnectionToClient* source) : Source(source)
{
}

ConnectionToClient* PacketData::GetConnectionToClient() const
{
    return Source;
}
