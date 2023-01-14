#include "Packet.h"

Packet::Packet(const PacketHeader& header) : Header(header), DataPos(0)
{
    Data.reserve(header.PacketSize);
}

std::vector<uint8_t>& Packet::GetData()
{
    Data.resize(Header.PacketSize);
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

Packet& Packet::operator>>(float& data)
{
    if (DataPos + sizeof(float) > Header.PacketSize)
    {
        throw std::runtime_error("Packet size exceeded");
    }
    data = *reinterpret_cast<float*>(Data.data() + DataPos);
    DataPos += sizeof(float);
    return *this;
}

Packet& Packet::operator>>(long& data)
{
    if (DataPos + sizeof(long) > Header.PacketSize)
    {
        throw std::runtime_error("Packet size exceeded");
    }
    data = *reinterpret_cast<long*>(Data.data() + DataPos);
    DataPos += sizeof(long);
    return *this;
}

Packet& Packet::operator>>(int& data)
{
    if (DataPos + sizeof(int) > Header.PacketSize)
    {
        throw std::runtime_error("Packet size exceeded");
    }
    data = *reinterpret_cast<int*>(Data.data() + DataPos);
    DataPos += sizeof(int);
    return *this;
}

Packet& Packet::operator>>(uint16_t& data)
{
    if (DataPos + sizeof(uint16_t) > Header.PacketSize)
    {
        throw std::runtime_error("Packet size exceeded");
    }
    data = *reinterpret_cast<uint16_t*>(Data.data() + DataPos);
    DataPos += sizeof(uint16_t);
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

Packet& Packet::operator<<(const float& data)
{
    if (Data.size() + sizeof(float) > Header.PacketSize)
    {
        throw std::runtime_error("Packet size exceeded");
    }
    Data.insert(Data.end(), reinterpret_cast<const uint8_t*>(&data), reinterpret_cast<const uint8_t*>(&data) + sizeof(float));
    return *this;
}

Packet& Packet::operator<<(const long& data)
{
    if (Data.size() + sizeof(long) > Header.PacketSize)
    {
        throw std::runtime_error("Packet size exceeded");
    }
    Data.insert(Data.end(), reinterpret_cast<const uint8_t*>(&data), reinterpret_cast<const uint8_t*>(&data) + sizeof(long));
    return *this;
}

Packet& Packet::operator<<(const int& data)
{
    if (Data.size() + sizeof(int) > Header.PacketSize)
    {
        throw std::runtime_error("Packet size exceeded");
    }
    Data.insert(Data.end(), reinterpret_cast<const uint8_t*>(&data), reinterpret_cast<const uint8_t*>(&data) + sizeof(int));
    return *this;
}

Packet& Packet::operator<<(const uint16_t& data)
{
    if (Data.size() + sizeof(uint16_t) > Header.PacketSize)
    {
        throw std::runtime_error("Packet size exceeded");
    }
    Data.insert(Data.end(), reinterpret_cast<const uint8_t*>(&data), reinterpret_cast<const uint8_t*>(&data) + sizeof(uint16_t));
    return *this;
}
