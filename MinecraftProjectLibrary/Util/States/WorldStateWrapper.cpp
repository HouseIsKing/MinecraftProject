#include "WorldStateWrapper.h"
#include "World/World.h"

void WorldStateWrapper::WriteChangesToVector(std::vector<uint8_t>& changes) const
{
    for (const EChangeType& change : Changes)
    {
        switch (change)
        {
        case EChangeType::RandomState:
            EngineDefaults::EmplaceReplaceDataInVector(changes, &change);
            EngineDefaults::EmplaceReplaceDataInVector(changes, &OldState.RandomEngine);
            EngineDefaults::EmplaceReplaceDataInVector(changes, &State.RandomEngine);
            break;
        case EChangeType::WorldTime:
            EngineDefaults::EmplaceReplaceDataInVector(changes, &change);
            EngineDefaults::EmplaceReplaceDataInVector(changes, &OldState.WorldTime);
            EngineDefaults::EmplaceReplaceDataInVector(changes, &State.WorldTime);
            break;
        case EChangeType::ChunkEnterWorld:
            {
                EngineDefaults::EmplaceReplaceDataInVector(changes, &change);
                auto chunkCount = static_cast<uint32_t>(ChunksEnterWorld.size());
                EngineDefaults::EmplaceReplaceDataInVector(changes, &chunkCount);
                for (const ChunkCoords& coords : ChunksEnterWorld)
                {
                    EngineDefaults::EmplaceReplaceDataInVector(changes, &State.Chunks.at(coords).GetState());
                }
                break;
            }
        case EChangeType::ChunkLeaveWorld:
            {
                EngineDefaults::EmplaceReplaceDataInVector(changes, &change);
                auto chunkCount = static_cast<uint32_t>(ChunksLeaveWorld.size());
                EngineDefaults::EmplaceReplaceDataInVector(changes, &chunkCount);
                for (const ChunkCoords& coords : ChunksLeaveWorld)
                {
                    EngineDefaults::EmplaceReplaceDataInVector(changes, &OldState.Chunks.at(coords).GetState());
                }
                break;
            }
        case EChangeType::PlayerEnterWorld:
            {
                EngineDefaults::EmplaceReplaceDataInVector(changes, &change);
                auto playerCount = static_cast<uint32_t>(PlayersEnterWorld.size());
                EngineDefaults::EmplaceReplaceDataInVector(changes, &playerCount);
                for (const uint16_t& playerId : PlayersEnterWorld)
                {
                    EngineDefaults::EmplaceReplaceDataInVector(changes, &State.Players.at(playerId).GetState());
                }
                break;
            }
        case EChangeType::PlayerLeaveWorld:
            {
                EngineDefaults::EmplaceReplaceDataInVector(changes, &change);
                auto playerCount = static_cast<uint32_t>(PlayersLeaveWorld.size());
                EngineDefaults::EmplaceReplaceDataInVector(changes, &playerCount);
                for (const uint16_t& playerId : PlayersLeaveWorld)
                {
                    EngineDefaults::EmplaceReplaceDataInVector(changes, &OldState.Players.at(playerId).GetState());
                }
                break;
            }
        case EChangeType::LightEnterWorld:
            {
                EngineDefaults::EmplaceReplaceDataInVector(changes, &change);
                auto lightCount = static_cast<uint32_t>(LightsEnterWorld.size());
                EngineDefaults::EmplaceReplaceDataInVector(changes, &lightCount);
                for (const glm::ivec2& coords : LightsEnterWorld)
                {
                    EngineDefaults::EmplaceReplaceDataInVector(changes, &coords);
                    EngineDefaults::EmplaceReplaceDataInVector(changes, &State.Lights.at(coords));
                }
                break;
            }
        case EChangeType::LightLeaveWorld:
            {
                EngineDefaults::EmplaceReplaceDataInVector(changes, &change);
                auto lightCount = static_cast<uint32_t>(LightsLeaveWorld.size());
                EngineDefaults::EmplaceReplaceDataInVector(changes, &lightCount);
                for (const glm::ivec2& coords : LightsLeaveWorld)
                {
                    EngineDefaults::EmplaceReplaceDataInVector(changes, &coords);
                    EngineDefaults::EmplaceReplaceDataInVector(changes, &OldState.Lights.at(coords));
                }
                break;
            }
        case EChangeType::LightState:
            {
                EngineDefaults::EmplaceReplaceDataInVector(changes, &change);
                auto size = static_cast<uint32_t>(LightStatesChanged.size());
                EngineDefaults::EmplaceReplaceDataInVector(changes, &size);
                for (const glm::ivec2& coords : LightStatesChanged)
                {
                    EngineDefaults::EmplaceReplaceDataInVector(changes, &coords);
                    EngineDefaults::EmplaceReplaceDataInVector(changes, &OldState.Lights.at(coords));
                    EngineDefaults::EmplaceReplaceDataInVector(changes, &State.Lights.at(coords));
                }
                break;
            }
        default:
            break;
        }
    }
    for (const Chunk& chunk : State.Chunks | std::ranges::views::values)
    {
        chunk.AttachChunkChanges(changes);
    }
    for (const Player& player : State.Players | std::ranges::views::values)
    {
        player.AttachEntityChange(changes);
    }
}

void WorldStateWrapper::ClearAllChanges()
{
    for (const EChangeType& change : Changes)
    {
        switch (change)
        {
        case EChangeType::ChunkEnterWorld:
            for (const ChunkCoords& coords : ChunksEnterWorld)
            {
                OldState.Chunks.emplace(coords, State.Chunks.at(coords).GetState()).first->second.ClearAllChanges();
            }
            break;
        case EChangeType::ChunkLeaveWorld:
            for (const ChunkCoords& coords : ChunksLeaveWorld)
            {
                OldState.Chunks.erase(coords);
            }
            break;
        case EChangeType::PlayerEnterWorld:
            for (const uint16_t& playerId : PlayersEnterWorld)
            {
                OldState.Players.emplace(playerId, State.Players.at(playerId).GetState()).first->second.ClearAllChanges();
            }
            break;
        case EChangeType::PlayerLeaveWorld:
            for (const uint16_t& playerId : PlayersLeaveWorld)
            {
                OldState.Players.erase(playerId);
            }
            break;
        case EChangeType::LightEnterWorld:
            for (const glm::ivec2& coords : LightsEnterWorld)
            {
                OldState.Lights.emplace(coords, State.Lights.at(coords));
            }
            break;
        case EChangeType::LightLeaveWorld:
            for (const glm::ivec2& coords : LightsLeaveWorld)
            {
                OldState.Lights.erase(coords);
            }
            break;
        case EChangeType::LightState:
            for (const glm::ivec2& coords : LightStatesChanged)
            {
                OldState.Lights.at(coords) = State.Lights.at(coords);
            }
            break;
        case EChangeType::WorldTime:
            OldState.WorldTime = State.WorldTime;
            break;
        case EChangeType::RandomState:
            OldState.RandomEngine.Seed = State.RandomEngine.Seed;
            break;
        default:
            break;
        }
    }
    for (Chunk& chunk : State.Chunks | std::ranges::views::values)
    {
        chunk.ClearAllChanges();
    }
    for (Player& player : State.Players | std::ranges::views::values)
    {
        player.ClearAllChanges();
    }
    Changes.clear();
    ChunksEnterWorld.clear();
    ChunksLeaveWorld.clear();
    PlayersEnterWorld.clear();
    PlayersLeaveWorld.clear();
    LightStatesChanged.clear();
    LightsEnterWorld.clear();
    LightsLeaveWorld.clear();
}

void WorldStateWrapper::WriteAllDataToVector(std::vector<uint8_t>& data) const
{
    auto change = EChangeType::WorldTime;
    EngineDefaults::EmplaceReplaceDataInVector(data, &change);
    EngineDefaults::EmplaceReplaceDataInVector(data, &State.WorldTime);
    change = EChangeType::RandomState;
    EngineDefaults::EmplaceReplaceDataInVector(data, &change);
    EngineDefaults::EmplaceReplaceDataInVector(data, &State.RandomEngine.Seed);
    change = EChangeType::ChunkEnterWorld;
    EngineDefaults::EmplaceReplaceDataInVector(data, &change);
    auto size = static_cast<uint32_t>(State.Chunks.size());
    EngineDefaults::EmplaceReplaceDataInVector(data, &size);
    for (const Chunk& chunk : State.Chunks | std::ranges::views::values)
    {
        chunk.AttachChunkData(data);
    }
    change = EChangeType::PlayerEnterWorld;
    EngineDefaults::EmplaceReplaceDataInVector(data, &change);
    size = static_cast<uint32_t>(State.Players.size());
    EngineDefaults::EmplaceReplaceDataInVector(data, &size);
    for (const Player& player : State.Players | std::ranges::views::values)
    {
        player.AttachEntityData(data);
    }
    change = EChangeType::LightEnterWorld;
    EngineDefaults::EmplaceReplaceDataInVector(data, &change);
    size = static_cast<uint32_t>(State.Lights.size());
    EngineDefaults::EmplaceReplaceDataInVector(data, &size);
    for (const auto& [coords, light] : State.Lights)
    {
        EngineDefaults::EmplaceReplaceDataInVector(data, &coords);
        EngineDefaults::EmplaceReplaceDataInVector(data, &light);
    }
}

void WorldStateWrapper::AddChunk(int x, int y, int z)
{
    State.Chunks.emplace(std::piecewise_construct, std::forward_as_tuple(x, y, z), std::forward_as_tuple(x, y, z));
    Changes.emplace(EChangeType::ChunkEnterWorld);
}

void WorldStateWrapper::AddChunk(const ChunkState& state)
{
    State.Chunks.emplace(state.ChunkPosition, state);
    Changes.emplace(EChangeType::ChunkEnterWorld);
}

void WorldStateWrapper::RemoveChunk(const ChunkCoords& coords)
{
    State.Chunks.erase(coords);
    Changes.emplace(EChangeType::ChunkLeaveWorld);
}

void WorldStateWrapper::AddPlayer(const uint16_t& playerId, float x, float y, float z)
{
    State.Players.emplace(std::piecewise_construct, std::forward_as_tuple(playerId), std::forward_as_tuple(x, y, z, playerId));
    Changes.emplace(EChangeType::PlayerEnterWorld);
}

void WorldStateWrapper::AddPlayer(const PlayerState& state)
{
    State.Players.emplace(state.EntityId, state);
    Changes.emplace(EChangeType::PlayerEnterWorld);
}

void WorldStateWrapper::RemovePlayer(const uint16_t& playerId)
{
    State.Players.erase(playerId);
    Changes.emplace(EChangeType::PlayerLeaveWorld);
}

void WorldStateWrapper::AddLight(const glm::ivec2& coords, uint8_t value)
{
    State.Lights.emplace(coords, value);
    Changes.emplace(EChangeType::LightEnterWorld);
}

void WorldStateWrapper::RemoveLight(const glm::ivec2& coords)
{
    State.Lights.erase(coords);
    Changes.emplace(EChangeType::LightLeaveWorld);
}

void WorldStateWrapper::ChangeLight(const glm::ivec2& coords, const uint8_t value)
{
    State.Lights.at(coords) = value;
    Changes.emplace(EChangeType::LightState);
}

void WorldStateWrapper::SetRandomSeed(const uint64_t seed)
{
    State.RandomEngine.Seed = seed;
    Changes.emplace(EChangeType::RandomState);
}

void WorldStateWrapper::SetWorldTime(const uint64_t worldTime)
{
    State.WorldTime = worldTime;
    Changes.emplace(EChangeType::WorldTime);
}

Chunk& WorldStateWrapper::GetChunk(const ChunkCoords& coords)
{
    return State.Chunks.at(coords);
}

Player& WorldStateWrapper::GetPlayer(const uint16_t& playerId)
{
    return State.Players.at(playerId);
}

std::map<ChunkCoords, Chunk>::iterator WorldStateWrapper::GetChunksIterator()
{
    return State.Chunks.begin();
}

std::map<uint16_t, Player>::iterator WorldStateWrapper::GetPlayersIterator()
{
    return State.Players.begin();
}
