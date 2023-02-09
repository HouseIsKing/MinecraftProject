#pragma once
#include "Util/EngineDefaults.h"
#include <glm/glm.hpp>

class Transform
{
    TransformStruct* TransformData;
    glm::mat4x4 TransformMatrix;
    void CalculateTransformMatrix();
    Transform* Parent;

public:
    explicit Transform(TransformStruct* transformData);
    Transform(TransformStruct* transformData, Transform* parent);
    glm::mat4x4 GetTransformMatrix();
    [[nodiscard]] Transform* GetParent() const;
};
