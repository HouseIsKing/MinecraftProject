#pragma once
#include <glm/vec3.hpp>

#include "../Util/TessellationHelper.h"
#include "Util/BoundingBox.h"

class MultiPlayerWorld;

class Entity
{
    static MultiPlayerWorld* World;
    bool IsGrounded;
    glm::vec3 EntitySize;
    const uint16_t EntityId;

protected:
    float VelocityX;
    float VelocityY;
    float VelocityZ;
    glm::vec3 PrevPos;
    Entity(glm::vec3 entitySize, float x, float y, float z);
    [[nodiscard]] bool IsOnGround() const;
    void CheckCollisionAndMove();
    virtual void Tick();
    static MultiPlayerWorld* GetWorld();

public:
    virtual ~Entity();
    Entity(const Entity&) = delete;
    Entity& operator=(const Entity&) = delete;
    Entity& operator=(Entity&&) = delete;
    Entity(Entity&&) = delete;
    static void SetWorld(MultiPlayerWorld* newWorld);
    void DoTick();
    void DoRender(float partialTick);
    [[nodiscard]] Transform& GetTransform() const;
    [[nodiscard]] BoundingBox GetBoundingBox() const;
    [[nodiscard]] uint16_t GetEntityId() const;
    [[nodiscard]] vec3 GetEntitySize() const;
};
