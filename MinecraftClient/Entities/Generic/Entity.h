#pragma once
#include "Util/BoundingBox.h"
#include "Util/TessellationHelper.h"

template <typename T>
class Entity
{
    static T* World;
    bool IsGrounded;
    glm::vec3 EntitySize;
    const uint16_t EntityId;

protected:
    float VelocityX;
    float VelocityY;
    float VelocityZ;
    TessellationHelper Tessellation;
    glm::vec3 PrevPos;
    Entity(glm::vec3 entitySize, float x, float y, float z);
    [[nodiscard]] bool IsOnGround() const;
    void CheckCollisionAndMove();
    static T* GetWorld();

public:
    virtual ~Entity();
    Entity(const Entity&) = delete;
    Entity& operator=(const Entity&) = delete;
    Entity& operator=(Entity&&) = delete;
    Entity(Entity&&) = delete;
    static void SetWorld(T* newWorld);
    virtual void Tick();
    void virtual Render(float partialTick);
    [[nodiscard]] Transform& GetTransform() const;
    [[nodiscard]] BoundingBox GetBoundingBox() const;
    [[nodiscard]] uint16_t GetEntityId() const;
    [[nodiscard]] glm::vec3 GetEntitySize() const;
};

template <typename T>
T* Entity<T>::World = nullptr;

template <typename T>
Entity<T>::Entity(const glm::vec3 entitySize, const float x, const float y, const float z) : IsGrounded(false), EntitySize(entitySize), EntityId(GetWorld()->RegisterEntity(this)), VelocityX(0), VelocityY(0), VelocityZ(0),
    Tessellation(x + entitySize.x, y + entitySize.y, z + entitySize.z), PrevPos(x, y, z)
{
}

template <typename T>
Entity<T>::~Entity() = default;

template <typename T>
void Entity<T>::SetWorld(T* newWorld)
{
    World = newWorld;
}

template <typename T>
T* Entity<T>::GetWorld()
{
    return World;
}

template <typename T>
bool Entity<T>::IsOnGround() const
{
    return IsGrounded;
}

template <typename T>
void Entity<T>::CheckCollisionAndMove()
{
    const float originalY = VelocityY;
    const glm::vec3 pos = GetTransform().GetPosition();
    auto myBoundingBox = BoundingBox(pos.x - EntitySize.x, pos.y - EntitySize.y, pos.z - EntitySize.z, pos.x + EntitySize.x, pos.y + EntitySize.y, pos.z + EntitySize.z);
    auto movementBox = BoundingBox(myBoundingBox);
    movementBox.Expand(VelocityX, VelocityY, VelocityZ);
    movementBox.Grow(1.0F, 1.0F, 1.0F);
    std::vector<BoundingBox> collidingBoxes = World->GetBlockBoxesInBoundingBox(movementBox);
    for (BoundingBox& box : collidingBoxes)
    {
        VelocityX = myBoundingBox.ClipCollisionX(box, VelocityX);
    }
    myBoundingBox.Move(VelocityX, 0.0F, 0.0F);
    GetTransform().Move(VelocityX, 0.0F, 0.0F);
    for (BoundingBox& box : collidingBoxes)
    {
        VelocityY = myBoundingBox.ClipCollisionY(box, VelocityY);
    }
    myBoundingBox.Move(0.0F, VelocityY, 0.0F);
    GetTransform().Move(0.0F, VelocityY, 0.0F);
    for (BoundingBox& box : collidingBoxes)
    {
        VelocityZ = myBoundingBox.ClipCollisionZ(box, VelocityZ);
    }
    GetTransform().Move(0.0F, 0.0F, VelocityZ);
    IsGrounded = originalY <= 0 && abs(VelocityY - originalY) > 0.001F;
}

template <typename T>
void Entity<T>::Render(float /*partialTick*/)
{
}

template <typename T>
void Entity<T>::Tick()
{
    PrevPos = GetTransform().GetPosition();
}

template <typename T>
Transform& Entity<T>::GetTransform() const
{
    return *Tessellation.GetTransform(0);
}

template <typename T>
BoundingBox Entity<T>::GetBoundingBox() const
{
    const glm::vec3 pos = GetTransform().GetPosition();
    return {pos.x - EntitySize.x, pos.y - EntitySize.y, pos.z - EntitySize.z, pos.x + EntitySize.x, pos.y + EntitySize.y, pos.z + EntitySize.z};
}

template <typename T>
uint16_t Entity<T>::GetEntityId() const
{
    return EntityId;
}

template <typename T>
glm::vec3 Entity<T>::GetEntitySize() const
{
    return EntitySize;
}

