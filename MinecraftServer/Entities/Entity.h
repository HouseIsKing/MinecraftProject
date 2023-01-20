#pragma once
#include <memory>

#include "Util/BoundingBox.h"
#include "Util/Transform.h"
#include <glm/vec3.hpp>

#include "Network/Packets/Packet.h"

class MultiPlayerWorld;

enum class EEntityType : uint8_t
{
    Player,
    BlockBreakParticle,
    Zombie,
};

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
    [[nodiscard]] bool IsOnGround() const;
    void CheckCollisionAndMove();
    static MultiPlayerWorld* GetWorld();

public:
    Entity(glm::vec3 entitySize, float x, float y, float z);
    virtual ~Entity();
    Entity(const Entity&) = delete;
    Entity& operator=(const Entity&) = delete;
    Entity& operator=(Entity&&) = delete;
    Entity(Entity&&) = delete;
    static void SetWorld(MultiPlayerWorld* newWorld);
    Transform& GetTransform();
    virtual void Tick();
    BoundingBox GetBoundingBox();
    [[nodiscard]] uint16_t GetEntityId() const;
    [[nodiscard]] glm::vec3 GetEntitySize() const;
    [[nodiscard]] virtual std::shared_ptr<Packet> GetTickPacket();
    [[nodiscard]] virtual std::shared_ptr<Packet> GetSpawnPacket();
    [[nodiscard]] virtual EEntityType GetEntityType() const = 0;
};
