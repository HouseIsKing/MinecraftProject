#pragma once
#include "PacketHeader.h"
#include "Network/ConnectionToClient.h"

class Packet
{
    PacketHeader Header;
    ConnectionToClient* Connection;
    std::vector<>
    
public:
    Packet();
};
