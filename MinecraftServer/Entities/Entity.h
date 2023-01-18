#pragma once
#include <memory>

#include "Util/BoundingBox.h"
#include "Util/Transform.h"
#include <glm/vec3.hpp>

#include "Network/Packets/Packet.h"

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
    Transform EntityTransform;
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
    Transform& GetTransform();
    BoundingBox GetBoundingBox();
    [[nodiscard]] uint16_t GetEntityId() const;
    [[nodiscard]] glm::vec3 GetEntitySize() const;
    [[nodiscard]] virtual std::shared_ptr<Packet> GetTickPacket();
};
