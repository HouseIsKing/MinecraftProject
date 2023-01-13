#pragma once
#include <stdexcept>

#include "PacketHeader.h"
#include <vector>

class Packet
{
    PacketHeader Header;
    std::vector<uint8_t> Data;
    size_t DataPos;

public:
    explicit Packet(const PacketHeader& header);
    Packet& operator>>(std::string& data);
    Packet& operator<<(const std::string& data);
    std::vector<uint8_t>& GetData();
    [[nodiscard]] const PacketHeader& GetHeader() const;
};
