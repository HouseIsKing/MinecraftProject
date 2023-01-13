#include "ConnectionInitPacket.h"

ConnectionInitPacket::ConnectionInitPacket(std::string clientName) : ClientName(std::move(clientName))
{
}

const std::string& ConnectionInitPacket::GetClientName() const
{
    return ClientName;
}

Packet ConnectionInitPacket::GetPacketAsSend()
{
    Packet result{PacketHeader(EPacketType::ConnectionInit, static_cast<uint16_t>(ClientName.size()))};
    return result << ClientName;
}
