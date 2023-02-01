#pragma once
#include "Util/TransformStruct.h"
#include <glm/gtc/epsilon.hpp>


struct EntityState
{
    TransformStruct EntityTransform;
    glm::vec3 EntityVelocity;
    bool IsGrounded{false};
    const uint16_t EntityId;
    const EEntityType EntityType{EEntityType::Player};
    virtual ~EntityState() = default;
    EntityState(const EntityState& other) = default;
    EntityState(EntityState&& other) noexcept = default;
    EntityState& operator=(const EntityState& other) = delete;
    EntityState& operator=(EntityState&& other) noexcept = delete;

    explicit EntityState(uint16_t entityId, EEntityType entityType = EEntityType::Player);
    [[nodiscard]] virtual size_t GetSize() const;
    virtual std::vector<uint8_t> operator-(const EntityState& newState) const;
};

inline EntityState::EntityState(const uint16_t entityId, const EEntityType entityType) : EntityTransform(),
    EntityVelocity(), EntityId(entityId), EntityType(entityType)
{
}

inline size_t EntityState::GetSize() const
{
    return sizeof EntityState;
}

inline std::vector<uint8_t> EntityState::operator-(const EntityState& newState) const
{
    std::vector<uint8_t> result{};
    EngineDefaults::EmplaceDataInVector(result, reinterpret_cast<const uint8_t*>(&EntityId), sizeof(EntityId));
    result.emplace_back(static_cast<uint8_t>(0));
    const std::vector<uint8_t> transformResult = EntityTransform - newState.EntityTransform;
    result[sizeof EntityId] += transformResult[0];
    EngineDefaults::EmplaceDataInVector(result, transformResult.data() + 1, transformResult.size() - 1);
    if (!all(epsilonEqual(EntityVelocity, newState.EntityVelocity, 0.001F)))
    {
        result[sizeof EntityId]++;
        constexpr auto temp = EChangeTypeEntity::Velocity;
        EngineDefaults::EmplaceDataInVector(result, reinterpret_cast<const uint8_t*>(&temp), sizeof(EChangeTypeEntity));
        EngineDefaults::EmplaceDataInVector(result, reinterpret_cast<const uint8_t*>(&newState.EntityVelocity), sizeof(EntityVelocity));
    }
    if (IsGrounded != newState.IsGrounded)
    {
        result[sizeof EntityId]++;
        constexpr auto temp = EChangeTypeEntity::IsGrounded;
        EngineDefaults::EmplaceDataInVector(result, reinterpret_cast<const uint8_t*>(&temp), sizeof(EChangeTypeEntity));
        EngineDefaults::EmplaceDataInVector(result, reinterpret_cast<const uint8_t*>(&newState.IsGrounded), sizeof IsGrounded);
    }
    return result;
}
