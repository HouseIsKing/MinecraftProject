#include "Transform.h"

#include <glm/gtx/transform.hpp>

Transform::Transform() : Transform(nullptr)
{
}

Transform::Transform(Transform* parent) : Position(glm::vec3(0.0F)), Rotation(glm::vec3(0.0F)), Scale(glm::vec3(1.0F)), TransformMatrix(1.0F), IsDirty(true), Parent(parent)
{
}

void Transform::CalculateTransformMatrix()
{
    TransformMatrix = translate(Position);
    TransformMatrix = rotate(TransformMatrix, glm::radians(Rotation.z), glm::vec3(0.0F, 0.0F, 1.0F));
    TransformMatrix = rotate(TransformMatrix, glm::radians(Rotation.y), glm::vec3(0.0F, -1.0F, 0.0F));
    TransformMatrix = rotate(TransformMatrix, glm::radians(Rotation.x), glm::vec3(1.0F, 0.0F, 0.0F));
    TransformMatrix = scale(TransformMatrix, Scale);
    IsDirty = false;
}

glm::vec3 Transform::GetPosition() const
{
    return Position;
}

glm::vec3 Transform::GetRotation() const
{
    return Rotation;
}

void Transform::Rotate(const float x, const float y, const float z)
{
    Rotation.x += x;
    Rotation.y += y;
    Rotation.z += z;
    IsDirty = true;
}

void Transform::SetRotation(const float x, const float y, const float z)
{
    Rotation.x = x;
    Rotation.y = y;
    Rotation.z = z;
    IsDirty = true;
}

void Transform::Move(const float x, const float y, const float z)
{
    Position.x += x;
    Position.y += y;
    Position.z += z;
    IsDirty = true;
}

void Transform::SetPosition(const float x, const float y, const float z)
{
    Position.x = x;
    Position.y = y;
    Position.z = z;
    IsDirty = true;
}

void Transform::SetPosition(const glm::vec3& position)
{
    Position = position;
    IsDirty = true;
}

void Transform::Grow(const float x, const float y, const float z)
{
    Scale.x += x;
    Scale.y += y;
    Scale.z += z;
    IsDirty = true;
}

void Transform::SetScale(const float x, const float y, const float z)
{
    Scale.x = x;
    Scale.y = y;
    Scale.z = z;
    IsDirty = true;
}

glm::mat4x4 Transform::GetTransformMatrix()
{
    if (IsDirty)
    {
        CalculateTransformMatrix();
    }
    return TransformMatrix;
}

Transform* Transform::GetParent() const
{
    return Parent;
}
