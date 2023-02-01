#pragma once
#include "EngineDefaults.h"
#include <glm/gtc/epsilon.hpp>
#include <glm/trigonometric.hpp>
#include <glm/vec3.hpp>
#include <vector>

struct TransformStruct
{
    glm::vec3 Position;
    glm::vec3 Rotation;
    glm::vec3 Scale;
    std::vector<uint8_t> operator-(const TransformStruct& newTransform) const;
    [[nodiscard]] glm::vec3 GetForwardVector() const;
};

inline std::vector<uint8_t> TransformStruct::operator-(const TransformStruct& newTransform) const
{
    std::vector<uint8_t> result{};
    result.emplace_back(static_cast<uint8_t>(0));
    auto changeType = EChangeTypeEntity::Position;
    if (!all(epsilonEqual(Position, newTransform.Position, 0.001F)))
    {
        result[0]++;
        EngineDefaults::EmplaceDataInVector(result, reinterpret_cast<uint8_t*>(&changeType), sizeof(EChangeTypeEntity));
        EngineDefaults::EmplaceDataInVector(result, reinterpret_cast<const uint8_t*>(&newTransform.Position), sizeof(glm::vec3));
    }
    if (!all(epsilonEqual(Rotation, newTransform.Rotation, 0.001F)))
    {
        result[0]++;
        changeType = EChangeTypeEntity::Rotation;
        EngineDefaults::EmplaceDataInVector(result, reinterpret_cast<uint8_t*>(&changeType), sizeof(EChangeTypeEntity));
        EngineDefaults::EmplaceDataInVector(result, reinterpret_cast<const uint8_t*>(&newTransform.Rotation), sizeof(glm::vec3));
    }
    if (!all(epsilonEqual(Scale, newTransform.Scale, 0.001F)))
    {
        result[0]++;
        changeType = EChangeTypeEntity::Scale;
        EngineDefaults::EmplaceDataInVector(result, reinterpret_cast<uint8_t*>(&changeType), sizeof(EChangeTypeEntity));
        EngineDefaults::EmplaceDataInVector(result, reinterpret_cast<const uint8_t*>(&newTransform.Scale), sizeof(glm::vec3));
    }
    return result;
}

inline glm::vec3 TransformStruct::GetForwardVector() const
{
    return {
        glm::cos(glm::radians(Rotation.x) * glm::sin(glm::radians(Rotation.y))), -glm::sin(glm::radians(Rotation.x)), glm::cos(glm::radians(Rotation.x) * glm::cos(glm::radians(Rotation.y)))
    };
}
