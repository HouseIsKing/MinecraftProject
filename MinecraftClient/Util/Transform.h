#pragma once
#include <glm/glm.hpp>

class Transform
{
    glm::vec3 Position;
    glm::vec3 Rotation;
    glm::vec3 Scale;
    glm::mat4x4 TransformMatrix;
    bool IsDirty;
    void CalculateTransformMatrix();
    Transform* Parent;

public:
    Transform();
    explicit Transform(Transform* parent);
    [[nodiscard]] glm::vec3 GetPosition() const;
    [[nodiscard]] glm::vec3 GetRotation() const;
    void Rotate(float x, float y, float z);
    void SetRotation(float x, float y, float z);
    void Move(float x, float y, float z);
    void SetPosition(float x, float y, float z);
    void SetPosition(const glm::vec3& position);
    void Grow(float x, float y, float z);
    void SetScale(float x, float y, float z);
    glm::mat4x4 GetTransformMatrix();
    [[nodiscard]] Transform* GetParent() const;
};
