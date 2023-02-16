#pragma once
#include "Util/EngineDefaults.h"


struct EntityState : State
{
    TransformStruct EntityTransform{};
    glm::vec3 EntityVelocity{};
    bool IsGrounded{false};
    uint16_t EntityId{};
    EEntityType EntityType{EEntityType::Player};
    void Serialize(std::vector<uint8_t>& changes) const override;
    void Deserialize(const std::vector<uint8_t>& changes, size_t& pos) override;
};

inline void EntityState::Serialize(std::vector<uint8_t>& changes) const
{
    EngineDefaults::EmplaceReplaceDataInVector(changes, &EntityTransform);
    EngineDefaults::EmplaceReplaceDataInVector(changes, &EntityVelocity);
    EngineDefaults::EmplaceReplaceDataInVector(changes, &IsGrounded);
    EngineDefaults::EmplaceReplaceDataInVector(changes, &EntityId);
    EngineDefaults::EmplaceReplaceDataInVector(changes, &EntityType);
}

inline void EntityState::Deserialize(const std::vector<uint8_t>& changes, size_t& pos)
{
    EntityTransform = EngineDefaults::ReadDataFromVector<TransformStruct>(changes, pos);
    EntityVelocity = EngineDefaults::ReadDataFromVector<glm::vec3>(changes, pos);
    IsGrounded = EngineDefaults::ReadDataFromVector<bool>(changes, pos);
    EntityId = EngineDefaults::ReadDataFromVector<uint16_t>(changes, pos);
    EntityType = EngineDefaults::ReadDataFromVector<EEntityType>(changes, pos);
}
