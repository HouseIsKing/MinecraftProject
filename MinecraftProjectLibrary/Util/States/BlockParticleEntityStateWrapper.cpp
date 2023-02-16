#include "BlockParticleEntityStateWrapper.h"

void BlockParticleEntityStateWrapper::WriteChangesToVector(std::vector<uint8_t>& changes,
                                                           const EChangeTypeEntity& changeType) const
{
    switch (changeType)
    {
    case EChangeTypeEntity::BlockParticleLifeTime:
        EngineDefaults::EmplaceReplaceDataInVector(changes, &OldState.LifeTime);
        EngineDefaults::EmplaceReplaceDataInVector(changes, &State.LifeTime);
        break;
    case EChangeTypeEntity::BlockParticleAge:
        EngineDefaults::EmplaceReplaceDataInVector(changes, &OldState.Age);
        EngineDefaults::EmplaceReplaceDataInVector(changes, &State.Age);
        break;
    case EChangeTypeEntity::BlockParticleBlockType:
        EngineDefaults::EmplaceReplaceDataInVector(changes, &OldState.BlockParticleType);
        EngineDefaults::EmplaceReplaceDataInVector(changes, &State.BlockParticleType);
        break;
    default:
        EntityStateWrapper::WriteChangesToVector(changes, changeType);
        break;
    }
}

void BlockParticleEntityStateWrapper::ClearAllChanges(const EChangeTypeEntity& change)
{
    switch (change)
    {
    case EChangeTypeEntity::BlockParticleAge:
        OldState.Age = State.Age;
        break;
    case EChangeTypeEntity::BlockParticleBlockType:
        OldState.BlockParticleType = State.BlockParticleType;
        break;
    case EChangeTypeEntity::BlockParticleLifeTime:
        OldState.LifeTime = State.LifeTime;
        break;
    default:
        EntityStateWrapper::ClearAllChanges(change);
        break;
    }
}

BlockParticleEntityStateWrapper::BlockParticleEntityStateWrapper(const uint16_t id) : EntityStateWrapper(id, EEntityType::BlockBreakParticle)
{
}

BlockParticleEntityStateWrapper::BlockParticleEntityStateWrapper(const BlockParticleEntityState& otherState) : EntityStateWrapper(otherState)
{
}

void BlockParticleEntityStateWrapper::WriteChangesToVector(std::vector<uint8_t>& changes) const
{
    EntityStateWrapper::WriteChangesToVector(changes);
}

void BlockParticleEntityStateWrapper::ClearAllChanges()
{
    EntityStateWrapper::ClearAllChanges();
}

void BlockParticleEntityStateWrapper::SetLifeTime(const uint8_t lifeTime)
{
    State.LifeTime = lifeTime;
    Changes.emplace(EChangeTypeEntity::BlockParticleLifeTime);
}

void BlockParticleEntityStateWrapper::SetAge(const uint8_t age)
{
    State.Age = age;
    Changes.emplace(EChangeTypeEntity::BlockParticleAge);
}

void BlockParticleEntityStateWrapper::SetBlockType(const EBlockType blockType)
{
    State.BlockParticleType = blockType;
    Changes.emplace(EChangeTypeEntity::BlockParticleBlockType);
}
