#pragma once
#include "PacketHeader.h"
#include <stdexcept>
#include <vector>

class Packet
{
    PacketHeader Header;
    std::vector<uint8_t> Data;
    size_t DataPos;

public:
    explicit Packet(const PacketHeader& header);
    Packet& operator>>(std::string& data);
    Packet& operator>>(float& data);
    Packet& operator>>(long& data);
    Packet& operator>>(int& data);
    Packet& operator>>(uint32_t& data);
    Packet& operator>>(uint16_t& data);
    Packet& operator>>(uint8_t& data);
    Packet& operator<<(const std::string& data);
    Packet& operator<<(const float& data);
    Packet& operator<<(const long& data);
    Packet& operator<<(const int& data);
    Packet& operator<<(const uint32_t& data);
    Packet& operator<<(const uint16_t& data);
    Packet& operator<<(const uint8_t& data);
    std::vector<uint8_t>& GetData();
    [[nodiscard]] const PacketHeader& GetHeader() const;
};
