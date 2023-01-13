#include "Packet.h"

Packet::Packet(const PacketHeader& header) : Header(header), DataPos(0)
{
    Data.reserve(header.PacketSize);
}

std::vector<uint8_t>& Packet::GetData()
{
    return Data;
}

const PacketHeader& Packet::GetHeader() const
{
    return Header;
}

Packet& Packet::operator>>(std::string& data)
{
    if (DataPos + data.size() > Header.PacketSize)
    {
        throw std::runtime_error("Packet size exceeded");
    }
    data = std::string(Data.begin() + static_cast<int>(DataPos), Data.begin() + static_cast<int>(DataPos + data.size()));
    DataPos += data.size();
    return *this;
}

Packet& Packet::operator<<(const std::string& data)
{
    if (Data.size() + data.size() > Header.PacketSize)
    {
        throw std::runtime_error("Packet size exceeded");
    }
    Data.insert(Data.end(), reinterpret_cast<const uint8_t*>(data.data()), reinterpret_cast<const uint8_t*>(data.data()) + data.size());
    return *this;
}
