#pragma once
#include "PacketHeader.h"
#include "Util/States/ClientInputState.h"
#include <stdexcept>
#include <vector>

class Packet
{
    PacketHeader Header;
    std::vector<uint8_t> Data;
    size_t DataPos;

public:
    explicit Packet(const PacketHeader& header);
    Packet(const PacketHeader& header, std::vector<uint8_t> data);
    Packet& operator>>(std::string& data);
    Packet& operator>>(float& data);
    Packet& operator>>(long& data);
    Packet& operator>>(int& data);
    Packet& operator>>(uint64_t& data);
    Packet& operator>>(uint32_t& data);
    Packet& operator>>(uint16_t& data);
    Packet& operator>>(uint8_t& data);
    Packet& operator>>(ClientInputState& state);
    Packet& operator<<(const std::string& data);
    Packet& operator<<(const float& data);
    Packet& operator<<(const long& data);
    Packet& operator<<(const int& data);
    Packet& operator<<(const uint64_t& data);
    Packet& operator<<(const uint32_t& data);
    Packet& operator<<(const uint16_t& data);
    Packet& operator<<(const uint8_t& data);
    Packet& operator<<(const ClientInputState& state);
    std::vector<uint8_t>& GetData();
    [[nodiscard]] const PacketHeader& GetHeader() const;
};
