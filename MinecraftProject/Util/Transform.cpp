//
// Created by amit on 4/22/2022.
//

#include "Transform.h"

Transform::Transform() : Transform(nullptr)
{
}

Transform::Transform(Transform* parent) : Position(vec3(0.0F)), Rotation(vec3(0.0F)), Scale(vec3(1.0F)), TransformMatrix(1.0F), IsDirtyLocal(true), Parent(parent)
{
}

void Transform::CalculateTransformMatrix()
{
    if (Parent != nullptr && Parent->IsDirty())
    {
        Parent->CalculateTransformMatrix();
    }
    TransformMatrix = translate(Position);
    TransformMatrix = rotate(TransformMatrix, glm::radians(Rotation.x), vec3(1.0F, 0.0F, 0.0F));
    TransformMatrix = rotate(TransformMatrix, glm::radians(Rotation.y), vec3(0.0F, -1.0F, 0.0F));
    TransformMatrix = rotate(TransformMatrix, glm::radians(Rotation.z), vec3(0.0F, 0.0F, 1.0F));
    TransformMatrix = scale(TransformMatrix, Scale);
    if (Parent != nullptr)
    {
        TransformMatrix = Parent->GetTransformMatrix() * TransformMatrix;
    }
    IsDirtyLocal = false;
}

vec3 Transform::GetPosition() const
{
    return Position;
}

vec3 Transform::GetRotation() const
{
    return Rotation;
}

bool Transform::IsDirty() const
{
    // ReSharper disable once CppRedundantParentheses
    return IsDirtyLocal || (Parent != nullptr && Parent->IsDirty());
}

void Transform::Rotate(const float x, const float y, const float z)
{
    Rotation.x += x;
    Rotation.y += y;
    Rotation.z += z;
    IsDirtyLocal = true;
}

void Transform::SetRotation(const float x, const float y, const float z)
{
    Rotation.x = x;
    Rotation.y = y;
    Rotation.z = z;
    IsDirtyLocal = true;
}

void Transform::Move(const float x, const float y, const float z)
{
    Position.x += x;
    Position.y += y;
    Position.z += z;
    IsDirtyLocal = true;
}

void Transform::SetPosition(const float x, const float y, const float z)
{
    Position.x = x;
    Position.y = y;
    Position.z = z;
    IsDirtyLocal = true;
}

void Transform::Grow(const float x, const float y, const float z)
{
    Scale.x += x;
    Scale.y += y;
    Scale.z += z;
    IsDirtyLocal = true;
}

void Transform::SetScale(const float x, const float y, const float z)
{
    Scale.x = x;
    Scale.y = y;
    Scale.z = z;
    IsDirtyLocal = true;
}

mat4x4 Transform::GetTransformMatrix()
{
    if (IsDirty())
    {
        CalculateTransformMatrix();
    }
    return TransformMatrix;
}
