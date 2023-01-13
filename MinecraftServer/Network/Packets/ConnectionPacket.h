#pragma once
#include "Packet.h"

class ConnectionToClient;

struct ConnectionPacket
{
    Packet ThePacket;
    ConnectionToClient* Connection;
    ConnectionPacket(ConnectionToClient* connection, const PacketHeader& header);
};
