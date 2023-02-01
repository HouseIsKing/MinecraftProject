#pragma once
#include "Util/BoundingBox.h"
#include "Util/States/EntityState.h"
#include "Util/TransformStruct.h"
#include <memory>

class MultiPlayerWorld;

class Entity
{
    static MultiPlayerWorld* World;

protected:
    std::unique_ptr<EntityState> State;
    void CheckCollisionAndMove() const;
    static MultiPlayerWorld* GetWorld();
    virtual void ApplyEntityChange(const std::vector<uint8_t>& changes, size_t& pos, EChangeTypeEntity change);

public:
    Entity(glm::vec3 entitySize, float x, float y, float z, EntityState* state);
    virtual ~Entity();
    Entity(const Entity&) = delete;
    Entity& operator=(const Entity&) = delete;
    Entity& operator=(Entity&&) = delete;
    Entity(Entity&&) = delete;
    static void SetWorld(MultiPlayerWorld* newWorld);
    [[nodiscard]] TransformStruct& GetTransform() const;
    virtual void Tick();
    [[nodiscard]] BoundingBox GetBoundingBox() const;
    [[nodiscard]] uint16_t GetEntityId() const;
    [[nodiscard]] EEntityType GetEntityType() const;
    [[nodiscard]] virtual EntityState* GetEntityState() const;
    void ApplyEntityChanges(const std::vector<uint8_t>& changes, size_t& pos);
};
