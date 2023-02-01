#include "Transform.h"

#include <glm/gtx/transform.hpp>

Transform::Transform(TransformStruct* transformData) : Transform(transformData, nullptr)
{
}

Transform::Transform(TransformStruct* transformData, Transform* parent) : TransformData(transformData), TransformMatrix(), Parent(parent)
{
}

void Transform::CalculateTransformMatrix()
{
    TransformMatrix = translate(TransformData->Position);
    TransformMatrix = rotate(TransformMatrix, glm::radians(TransformData->Rotation.z), glm::vec3(0.0F, 0.0F, 1.0F));
    TransformMatrix = rotate(TransformMatrix, glm::radians(TransformData->Rotation.y), glm::vec3(0.0F, -1.0F, 0.0F));
    TransformMatrix = rotate(TransformMatrix, glm::radians(TransformData->Rotation.x), glm::vec3(1.0F, 0.0F, 0.0F));
}

glm::mat4x4 Transform::GetTransformMatrix()
{
    CalculateTransformMatrix();
    return TransformMatrix;
}

Transform* Transform::GetParent() const
{
    return Parent;
}
