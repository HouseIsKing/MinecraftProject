#include "PacketData.h"

PacketData::PacketData(std::shared_ptr<ConnectionToClient> source) : Source(std::move(source))
{
}

const std::shared_ptr<ConnectionToClient>& PacketData::GetConnectionToClient() const
{
    return Source;
}
