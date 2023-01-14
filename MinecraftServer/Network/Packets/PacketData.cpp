#include "PacketData.h"

#include "Network/ConnectionToClient.h"

PacketData::PacketData(std::shared_ptr<ConnectionToClient> source) : Source(std::move(source))
{
}

std::shared_ptr<ConnectionToClient>& PacketData::GetConnectionToClient()
{
    return Source;
}
