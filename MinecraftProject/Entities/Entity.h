#pragma once
#include "../Util/TessellationHelper.h"
#include <glm/glm.hpp>

#include "Util/BoundingBox.h"

class SinglePlayerWorld;

using glm::vec3;

class Entity
{
    static SinglePlayerWorld* World;
    bool IsGrounded;
    vec3 EntitySize;
    const uint16_t EntityId;

protected:
    float VelocityX;
    float VelocityY;
    float VelocityZ;
    TessellationHelper Tessellation;
    vec3 PrevPos;
    Entity(vec3 entitySize, float x, float y, float z);
    [[nodiscard]] bool IsOnGround() const;
    void virtual Render(float partialTick);
    void CheckCollisionAndMove();
    virtual void Tick();
    static SinglePlayerWorld* GetWorld();

public:
    virtual ~Entity();
    Entity(const Entity&) = delete;
    Entity& operator=(const Entity&) = delete;
    Entity& operator=(Entity&&) = delete;
    Entity(Entity&&) = delete;
    static void SetWorld(SinglePlayerWorld* newWorld);
    void DoTick();
    void DoRender(float partialTick);
    [[nodiscard]] Transform& GetTransform() const;
    [[nodiscard]] BoundingBox GetBoundingBox() const;
    [[nodiscard]] uint16_t GetEntityId() const;
};
