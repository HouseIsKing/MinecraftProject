#pragma once
#include "Util/BoundingBox.h"
#include "Util/States/EntityState.h"
#include "Util/TessellationHelper.h"

class World;

class Entity
{
protected:
    std::unique_ptr<EntityState> State;
    TessellationHelper Tessellation;
    TransformStruct PrevTransform;
    static World* TheWorld;
    Entity(glm::vec3 entitySize, float x, float y, float z, EntityState* state);
    void CheckCollisionAndMove() const;
    virtual void ApplyEntityChange(const std::vector<uint8_t>& changes, size_t& pos, EChangeTypeEntity change);

public:
    virtual ~Entity() = default;
    Entity(const Entity&) = delete;
    Entity& operator=(const Entity&) = delete;
    Entity& operator=(Entity&&) = delete;
    Entity(Entity&&) = delete;
    static void SetWorld(World* newWorld);
    virtual void Tick();
    void virtual Render(float partialTick);
    [[nodiscard]] TransformStruct& GetTransform() const;
    [[nodiscard]] BoundingBox GetBoundingBox() const;
    [[nodiscard]] uint16_t GetEntityId() const;
    [[nodiscard]] glm::vec3 GetEntitySize() const;
    [[nodiscard]] EEntityType GetEntityType() const;
    [[nodiscard]] virtual EntityState* GetEntityState() const;
    void ApplyEntityChanges(const std::vector<uint8_t>& changes, size_t& pos);
};
