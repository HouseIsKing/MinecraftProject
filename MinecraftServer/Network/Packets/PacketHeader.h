#pragma once

enum class EPacketType
{
    ConnectionInit,
    Position
};

struct PacketHeader
{
    EPacketType PacketType;
};
