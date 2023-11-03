#include "WorldStateWrapper.h"
#include "World/World.h"

void WorldStateWrapper::ClearAllChanges(const EChangeType& change)
{
    switch (change)
    {
    case EChangeType::ChunkEnterWorld:
        for (const ChunkCoords& coords : ChunksEnterWorld)
        {
            OldState.Chunks.emplace(coords, State.Chunks.at(coords));
        }
        break;
    case EChangeType::ChunkLeaveWorld:
        for (const ChunkCoords& coords : ChunksLeaveWorld)
        {
            Chunks.erase(coords);
            OldState.Chunks.erase(coords);
        }
        break;
    case EChangeType::PlayerEnterWorld:
        for (const uint16_t& playerId : PlayersEnterWorld)
        {
            OldState.Players.emplace(playerId, State.Players.at(playerId));
        }
        break;
    case EChangeType::PlayerLeaveWorld:
        for (const uint16_t& playerId : PlayersLeaveWorld)
        {
            Players.erase(playerId);
            EntityIdToType.erase(playerId);
            EntityAvailableIDs.emplace(playerId);
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
    case EChangeType::BlockParticleEnterWorld:
        for (const uint16_t& particleId : BlockParticleEntitiesEnterWorld)
        {
            OldState.BlockParticleEntities.emplace(particleId, State.BlockParticleEntities.at(particleId));
        }
        break;
    case EChangeType::BlockParticleLeaveWorld:
        for (const uint16_t& particleId : BlockParticleEntitiesLeaveWorld)
        {
            BlockParticleEntities.erase(particleId);
            EntityAvailableIDs.emplace(particleId);
            EntityIdToType.erase(particleId);
            OldState.BlockParticleEntities.erase(particleId);
        }
        break;
    default:
        break;
    }
}

void WorldStateWrapper::WriteChangesToVector(std::vector<uint8_t>& changes, const EChangeType& changeType) const
{
    switch (changeType)
    {
    case EChangeType::RandomState:
        EngineDefaults::EmplaceReplaceDataInVector(changes, &OldState.RandomEngine.Seed);
        EngineDefaults::EmplaceReplaceDataInVector(changes, &State.RandomEngine.Seed);
        break;
    case EChangeType::WorldTime:
        EngineDefaults::EmplaceReplaceDataInVector(changes, &OldState.WorldTime);
        EngineDefaults::EmplaceReplaceDataInVector(changes, &State.WorldTime);
        break;
    case EChangeType::ChunkEnterWorld:
        {
            const auto chunkCount = static_cast<uint32_t>(ChunksEnterWorld.size());
            EngineDefaults::EmplaceReplaceDataInVector(changes, &chunkCount);
            for (const ChunkCoords& coords : ChunksEnterWorld)
            {
                State.Chunks.at(coords)->GetState().Serialize(changes);
            }
            break;
        }
    case EChangeType::ChunkLeaveWorld:
        {
            const auto chunkCount = static_cast<uint32_t>(ChunksLeaveWorld.size());
            EngineDefaults::EmplaceReplaceDataInVector(changes, &chunkCount);
            for (const ChunkCoords& coords : ChunksLeaveWorld)
            {
                OldState.Chunks.at(coords)->GetState().Serialize(changes);
            }
            break;
        }
    case EChangeType::PlayerEnterWorld:
        {
            const auto playerCount = static_cast<uint32_t>(PlayersEnterWorld.size());
            EngineDefaults::EmplaceReplaceDataInVector(changes, &playerCount);
            for (const uint16_t& playerId : PlayersEnterWorld)
            {
                State.Players.at(playerId)->GetState().Serialize(changes);
            }
            break;
        }
    case EChangeType::PlayerLeaveWorld:
        {
            const auto playerCount = static_cast<uint32_t>(PlayersLeaveWorld.size());
            EngineDefaults::EmplaceReplaceDataInVector(changes, &playerCount);
            for (const uint16_t& playerId : PlayersLeaveWorld)
            {
                OldState.Players.at(playerId)->GetState().Serialize(changes);
            }
            break;
        }
    case EChangeType::LightEnterWorld:
        {
            const auto lightCount = static_cast<uint32_t>(LightsEnterWorld.size());
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
            const auto lightCount = static_cast<uint32_t>(LightsLeaveWorld.size());
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
            const auto size = static_cast<uint32_t>(LightStatesChanged.size());
            EngineDefaults::EmplaceReplaceDataInVector(changes, &size);
            for (const glm::ivec2& coords : LightStatesChanged)
            {
                EngineDefaults::EmplaceReplaceDataInVector(changes, &coords);
                EngineDefaults::EmplaceReplaceDataInVector(changes, &OldState.Lights.at(coords));
                EngineDefaults::EmplaceReplaceDataInVector(changes, &State.Lights.at(coords));
            }
            break;
        }
    case EChangeType::BlockParticleEnterWorld:
        {
            const auto blockParticleCount = static_cast<uint32_t>(BlockParticleEntitiesEnterWorld.size());
            EngineDefaults::EmplaceReplaceDataInVector(changes, &blockParticleCount);
            for (const uint16_t& blockParticleId : BlockParticleEntitiesEnterWorld)
            {
                State.BlockParticleEntities.at(blockParticleId)->GetState().Serialize(changes);
            }
            break;
        }
    case EChangeType::BlockParticleLeaveWorld:
        {
            const auto blockParticleCount = static_cast<uint32_t>(BlockParticleEntitiesLeaveWorld.size());
            EngineDefaults::EmplaceReplaceDataInVector(changes, &blockParticleCount);
            for (const uint16_t& blockParticleId : BlockParticleEntitiesLeaveWorld)
            {
                OldState.BlockParticleEntities.at(blockParticleId)->GetState().Serialize(changes);
            }
            break;
        }
    default:
        break;
    }
}

WorldStateWrapper::WorldStateWrapper()
{
    const auto it = EntityAvailableIDs.begin();
    for (uint16_t i = 0xFFFF; i > 0; --i)
    {
        EntityAvailableIDs.emplace_hint(it, i);
    }
    EntityAvailableIDs.emplace_hint(it, static_cast<uint16_t>(0));
}

void WorldStateWrapper::WriteChangesToVector(std::vector<uint8_t>& changes) const
{
    StateWrapper::WriteChangesToVector(changes);
    for (const Chunk* chunk : State.Chunks | std::ranges::views::values)
    {
        chunk->AttachChunkChanges(changes);
    }
    for (const Player* player : State.Players | std::ranges::views::values)
    {
        player->AttachEntityChange(changes);
    }
    for (const BlockParticleEntity* blockParticleEntity : State.BlockParticleEntities | std::ranges::views::values)
    {
        blockParticleEntity->AttachEntityChange(changes);
    }
}

void WorldStateWrapper::ClearAllChanges()
{
    StateWrapper::ClearAllChanges();
    for (Chunk* chunk : State.Chunks | std::ranges::views::values)
    {
        chunk->ClearAllChanges();
    }
    for (Player* player : State.Players | std::ranges::views::values)
    {
        player->ClearAllChanges();
    }
    for (BlockParticleEntity* blockParticleEntity : State.BlockParticleEntities | std::ranges::views::values)
    {
        blockParticleEntity->ClearAllChanges();
    }
    ChunksEnterWorld.clear();
    ChunksLeaveWorld.clear();
    PlayersEnterWorld.clear();
    PlayersLeaveWorld.clear();
    LightStatesChanged.clear();
    LightsEnterWorld.clear();
    LightsLeaveWorld.clear();
    BlockParticleEntitiesEnterWorld.clear();
    BlockParticleEntitiesLeaveWorld.clear();
}

void WorldStateWrapper::WriteAllDataToVector(std::vector<uint8_t>& data) const
{
    const auto changeCount = static_cast<uint16_t>(Changes.size());
    EngineDefaults::EmplaceReplaceDataInVector(data, &changeCount);
    auto change = EChangeType::WorldTime;
    EngineDefaults::EmplaceReplaceDataInVector(data, &change);
    EngineDefaults::EmplaceReplaceDataInVector(data, &OldState.WorldTime);
    EngineDefaults::EmplaceReplaceDataInVector(data, &State.WorldTime);
    change = EChangeType::RandomState;
    EngineDefaults::EmplaceReplaceDataInVector(data, &change);
    EngineDefaults::EmplaceReplaceDataInVector(data, &OldState.RandomEngine.Seed);
    EngineDefaults::EmplaceReplaceDataInVector(data, &State.RandomEngine.Seed);
    change = EChangeType::ChunkEnterWorld;
    EngineDefaults::EmplaceReplaceDataInVector(data, &change);
    auto size = static_cast<uint32_t>(State.Chunks.size());
    EngineDefaults::EmplaceReplaceDataInVector(data, &size);
    for (const Chunk* chunk : State.Chunks | std::ranges::views::values)
    {
        chunk->AttachChunkData(data);
    }
    change = EChangeType::PlayerEnterWorld;
    EngineDefaults::EmplaceReplaceDataInVector(data, &change);
    size = static_cast<uint32_t>(State.Players.size());
    EngineDefaults::EmplaceReplaceDataInVector(data, &size);
    for (const Player* player : State.Players | std::ranges::views::values)
    {
        player->AttachEntityData(data);
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

void WorldStateWrapper::AddChunk(const ChunkState& state)
{
    Chunks.emplace(state.ChunkPosition, state);
    State.Chunks.emplace(state.ChunkPosition, &Chunks.at(state.ChunkPosition));
    ChunksEnterWorld.emplace_back(state.ChunkPosition);
    Changes.emplace(EChangeType::ChunkEnterWorld);
}

void WorldStateWrapper::RemoveChunk(const ChunkCoords& coords)
{
    State.Chunks.erase(coords);
    ChunksLeaveWorld.emplace_back(coords);
    Changes.emplace(EChangeType::ChunkLeaveWorld);
}

void WorldStateWrapper::AddEntity(EntityState* state, const bool isOldState)
{
    if (!isOldState)
    {
        state->EntityId = GetValidEntityId();
    }
    else
    {
        EntityAvailableIDs.erase(state->EntityId);
    }
    switch (state->EntityType)
    {
    case EEntityType::Player:
        Players.emplace(state->EntityId, *dynamic_cast<const PlayerState*>(state));
        EntityIdToType.emplace(state->EntityId, EEntityType::Player);
        State.Players.emplace(state->EntityId, &Players.at(state->EntityId));
        PlayersEnterWorld.emplace_back(state->EntityId);
        Changes.emplace(EChangeType::PlayerEnterWorld);
        break;
    case EEntityType::BlockBreakParticle:
        BlockParticleEntities.emplace(state->EntityId, *dynamic_cast<const BlockParticleEntityState*>(state));
        EntityIdToType.emplace(state->EntityId, EEntityType::BlockBreakParticle);
        State.BlockParticleEntities.emplace(state->EntityId, &BlockParticleEntities.at(state->EntityId));
        BlockParticleEntitiesEnterWorld.emplace_back(state->EntityId);
        Changes.emplace(EChangeType::BlockParticleEnterWorld);
        break;
    case EEntityType::Zombie:
        break;
    }
}

void WorldStateWrapper::RemoveEntity(const uint16_t entityId)
{
    switch (EntityIdToType.at(entityId))
    {
    case EEntityType::Player:
        State.Players.erase(entityId);
        PlayersLeaveWorld.emplace_back(entityId);
        Changes.emplace(EChangeType::PlayerLeaveWorld);
        break;
    case EEntityType::BlockBreakParticle:
        State.BlockParticleEntities.erase(entityId);
        BlockParticleEntitiesLeaveWorld.emplace_back(entityId);
        Changes.emplace(EChangeType::BlockParticleLeaveWorld);
        break;
    case EEntityType::Zombie:
        break;
    }
}

void WorldStateWrapper::AddLight(const glm::ivec2& coords, uint8_t value)
{
    State.Lights.emplace(coords, value);
    LightsEnterWorld.emplace_back(coords);
    Changes.emplace(EChangeType::LightEnterWorld);
}

void WorldStateWrapper::RemoveLight(const glm::ivec2& coords)
{
    State.Lights.erase(coords);
    LightsLeaveWorld.emplace_back(coords);
    Changes.emplace(EChangeType::LightLeaveWorld);
}

void WorldStateWrapper::ChangeLight(const glm::ivec2& coords, const uint8_t value)
{
    State.Lights.at(coords) = value;
    LightStatesChanged.emplace_back(coords);
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

uint16_t WorldStateWrapper::GetValidEntityId()
{
    const uint16_t id = *EntityAvailableIDs.begin();
    EntityAvailableIDs.erase(EntityAvailableIDs.begin());
    return id;
}

const std::unordered_map<uint16_t, EEntityType>& WorldStateWrapper::GetEntityIdToType() const
{
    return EntityIdToType;
}

Chunk* WorldStateWrapper::GetChunk(const ChunkCoords& coords) const
{
    return State.Chunks.at(coords);
}

std::map<ChunkCoords, Chunk*>::iterator WorldStateWrapper::GetChunksIterator()
{
    return State.Chunks.begin();
}

std::map<uint16_t, Player*>::iterator WorldStateWrapper::GetPlayersIterator()
{
    return State.Players.begin();
}

std::map<uint16_t, BlockParticleEntity*>::iterator WorldStateWrapper::GetBlockParticleEntitiesIterator()
{
    return State.BlockParticleEntities.begin();
}
