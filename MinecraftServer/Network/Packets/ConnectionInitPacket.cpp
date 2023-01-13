#include "ConnectionInitPacket.h"

ConnectionInitPacket::ConnectionInitPacket(ConnectionPacket& packet) : PacketData(packet.Connection)
{
    ClientName.resize(packet.ThePacket.GetHeader().PacketSize);
    packet.ThePacket >> ClientName;
}

const std::string& ConnectionInitPacket::GetClientName() const
{
    return ClientName;
}
