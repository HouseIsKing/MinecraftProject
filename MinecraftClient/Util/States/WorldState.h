#pragma once
#include <algorithm>
#include <map>
#include <set>
#include <vector>

#include "ChunkState.h"
#include "EntityState.h"
#include "LightState.h"
#include "Util/CustomRandomEngine.h"

struct LightStateComparator
{
    bool operator()(const LightState& lhs, const LightState& rhs) const
    {
        return lhs < rhs;
    }
};

struct ChunkStateComparator
{
    bool operator()(const ChunkState& lhs, const ChunkState& rhs) const
    {
        return lhs.ChunkPosition < rhs.ChunkPosition;
    }
};

struct EntityStateComparator
{
    bool operator()(const std::shared_ptr<EntityState>& lhs, const std::shared_ptr<EntityState>& rhs) const
    {
        return lhs->EntityId < rhs->EntityId;
    }
};

struct WorldState
{
    std::map<ChunkCoords, ChunkState> Chunks;
    std::map<uint16_t, std::shared_ptr<EntityState>> Entities;
    std::map<glm::ivec2, LightState, IVec2Comparator> Lights;
    uint64_t WorldTime{};
    CustomRandomEngine RandomEngine;
    std::vector<uint8_t> operator-(const WorldState& newState) const;
};

inline std::vector<uint8_t> WorldState::operator-(const WorldState& newState) const
{
    std::vector<uint8_t> result{};
    std::map chunksSet{Chunks};
    chunksSet.insert(newState.Chunks.begin(), newState.Chunks.end());
    for (const auto& pair : chunksSet)
    {
        auto newChunk = newState.Chunks.find(pair.first);
        auto oldChunk = Chunks.find(pair.first);
        if (newChunk == newState.Chunks.end())
        {
            //Chunk got removed
            auto changeType = EChangeType::ChunkLeaveWorld;
            EngineDefaults::EmplaceDataInVector(result, reinterpret_cast<const uint8_t*>(&changeType), sizeof(EChangeType));
            EngineDefaults::EmplaceDataInVector(result, reinterpret_cast<const uint8_t*>(&pair.first), sizeof ChunkCoords);
        }
        else
        {
            if (oldChunk == Chunks.end())
            {
                //Chunk got added
                auto changeType = EChangeType::ChunkEnterWorld;
                EngineDefaults::EmplaceDataInVector(result, reinterpret_cast<const uint8_t*>(&changeType), sizeof(EChangeType));
                EngineDefaults::EmplaceDataInVector(result, reinterpret_cast<const uint8_t*>(&pair.second), sizeof ChunkState);
            }
            else
            {
                //Chunk got updated
                std::vector<uint8_t> changes = oldChunk->second - newChunk->second;
                if (changes[sizeof(ChunkCoords)] == 0 && changes[1 + sizeof(ChunkCoords)] == 0)
                {
                    continue;
                }
                auto changeType = EChangeType::ChunkState;
                EngineDefaults::EmplaceDataInVector(result, reinterpret_cast<const uint8_t*>(&changeType), sizeof(EChangeType));
                EngineDefaults::EmplaceDataInVector(result, changes.data(), changes.size());
            }
        }
    }
    std::map entitiesSet{Entities};
    entitiesSet.insert(newState.Entities.begin(), newState.Entities.end());
    for (const auto& entity : entitiesSet)
    {
        auto newEntity = newState.Entities.find(entity.first);
        auto oldEntity = Entities.find(entity.first);
        if (newEntity == newState.Entities.end())
        {
            //Chunk got removed
            auto changeType = EChangeType::EntityLeaveWorld;
            EngineDefaults::EmplaceDataInVector(result, reinterpret_cast<const uint8_t*>(&changeType), sizeof(EChangeType));
            EngineDefaults::EmplaceDataInVector(result, reinterpret_cast<const uint8_t*>(&entity.first), sizeof entity.first);
        }
        else
        {
            if (oldEntity == Entities.end())
            {
                //Chunk got added
                auto changeType = EChangeType::EntityEnterWorld;
                EngineDefaults::EmplaceDataInVector(result, reinterpret_cast<const uint8_t*>(&changeType), sizeof(EChangeType));
                EngineDefaults::EmplaceDataInVector(result, reinterpret_cast<const uint8_t*>(entity.second.get()), entity.second->GetSize());
            }
            else
            {
                //Chunk got updated
                std::vector<uint8_t> changes = *oldEntity->second - *newEntity->second;
                if (changes[sizeof(uint16_t)] == 0)
                {
                    continue;
                }
                auto changeType = EChangeType::EntityState;
                EngineDefaults::EmplaceDataInVector(result, reinterpret_cast<const uint8_t*>(&changeType), sizeof(EChangeType));
                EngineDefaults::EmplaceDataInVector(result, changes.data(), changes.size());
            }
        }
    }
    std::map lightsSet{Lights};
    lightsSet.insert(newState.Lights.begin(), newState.Lights.end());
    for (const auto& light : lightsSet)
    {
        auto newLight = newState.Lights.find(light.first);
        auto oldLight = Lights.find(light.first);
        if (newLight == newState.Lights.end())
        {
            //Chunk got removed
            auto changeType = EChangeType::LightLeaveWorld;
            EngineDefaults::EmplaceDataInVector(result, reinterpret_cast<const uint8_t*>(&changeType), sizeof(EChangeType));
            EngineDefaults::EmplaceDataInVector(result, reinterpret_cast<const uint8_t*>(&light.second.X), sizeof light.second.X);
            EngineDefaults::EmplaceDataInVector(result, reinterpret_cast<const uint8_t*>(&light.second.Y), sizeof light.second.Y);
        }
        else
        {
            if (oldLight == Lights.end())
            {
                //Chunk got added
                auto changeType = EChangeType::LightEnterWorld;
                EngineDefaults::EmplaceDataInVector(result, reinterpret_cast<const uint8_t*>(&changeType), sizeof(EChangeType));
                EngineDefaults::EmplaceDataInVector(result, reinterpret_cast<const uint8_t*>(&light.second), sizeof LightState);
            }
            else
            {
                //Chunk got updated
                if (oldLight->second.LightValue == newLight->second.LightValue)
                {
                    continue;
                }
                auto changeType = EChangeType::LightState;
                EngineDefaults::EmplaceDataInVector(result, reinterpret_cast<const uint8_t*>(&changeType), sizeof(EChangeType));
                EngineDefaults::EmplaceDataInVector(result, reinterpret_cast<const uint8_t*>(&newLight->second), sizeof LightState);
            }
        }
    }
    if (WorldTime != newState.WorldTime)
    {
        constexpr auto changeType = EChangeType::WorldTime;
        EngineDefaults::EmplaceDataInVector(result, reinterpret_cast<const uint8_t*>(&changeType), sizeof(EChangeType));
        EngineDefaults::EmplaceDataInVector(result, reinterpret_cast<const uint8_t*>(&newState.WorldTime), sizeof newState.WorldTime);
    }
    if (RandomEngine.Seed != newState.RandomEngine.Seed)
    {
        constexpr auto changeType = EChangeType::RandomState;
        EngineDefaults::EmplaceDataInVector(result, reinterpret_cast<const uint8_t*>(&changeType), sizeof(EChangeType));
        EngineDefaults::EmplaceDataInVector(result, reinterpret_cast<const uint8_t*>(&newState.RandomEngine.Seed), sizeof newState.RandomEngine.Seed);
    }
    return result;
}
