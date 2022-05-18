//
// Created by amit on 4/22/2022.
//

#pragma once
#include "../Util/TessellationHelper.h"
#include <glm/glm.hpp>

class SinglePlayerWorld;

using glm::vec3;

class Entity
{
    static SinglePlayerWorld* World;
    bool IsGrounded;
    vec3 EntitySize;
    const uint16_t EntityId; // NOLINT(clang-diagnostic-unused-private-field)
protected:
    float VelocityX;
    float VelocityY;
    float VelocityZ;
    TessellationHelper TessellationHelper;
    Entity(uint16_t entityId, vec3 entitySize, float x, float y, float z);
    [[nodiscard]] bool IsOnGround() const;
    void virtual Render();
    void CheckCollisionAndMove();
    virtual void Tick();
public:
    virtual ~Entity();
    Entity(const Entity&) = delete;
    Entity& operator=(const Entity&) = delete;
    Entity& operator=(Entity&&) = delete;
    Entity(Entity&&) = delete;
    static void SetWorld(SinglePlayerWorld* newWorld);
    static SinglePlayerWorld* GetWorld();
    void DoTick();
    void DoRender();
    Transform& GetTransform();
};
