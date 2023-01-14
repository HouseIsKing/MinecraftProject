#include "PacketData.h"

PacketData::PacketData(std::string name) : ClientName(std::move(name))
{
}

const std::string& PacketData::GetClientName()
{
    return ClientName;
}
