#include "WorldDataPacket.h"

WorldDataPacket::WorldDataPacket(Packet& packet)
{
    packet >> LastInputIdServerProcessed;
    uint8_t ticksCount = 0;
    packet >> ticksCount;
    std::vector<uint8_t> restOfData;
    packet >> restOfData;
    restOfData = EngineDefaults::Inflate(restOfData);
    uint64_t restOfDataPointer = 0;
    Data.reserve(ticksCount);
    for (uint8_t i = 0; i < ticksCount; i++)
    {
        Data.emplace_back();
        const uint64_t size = *reinterpret_cast<uint64_t*>(restOfData.data() + restOfDataPointer);
        Data[i].reserve(size);
        restOfDataPointer += sizeof(uint64_t);
        Data[i].insert(Data[i].end(), restOfData.begin() + restOfDataPointer, restOfData.begin() + restOfDataPointer + size);
        restOfDataPointer += size;
    }
}

EPacketType WorldDataPacket::GetPacketType() const
{
    return EPacketType::WorldData;
}

uint64_t WorldDataPacket::GetLastInputIdServerProcessed() const
{
    return LastInputIdServerProcessed;
}

const std::vector<std::vector<uint8_t>>& WorldDataPacket::GetData() const
{
    return Data;
}
