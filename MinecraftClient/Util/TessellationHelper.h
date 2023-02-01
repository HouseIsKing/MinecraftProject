#pragma once
#include "TessellationInterface.h"
#include "Transform.h"
#include "Vertex.h"
#include <memory>

class TessellationHelper final : public TessellationInterface
{
    std::vector<Vertex> Vertices{};
    std::vector<std::unique_ptr<Transform>> TessellationTransforms{};
    int PositionUniform;
    [[nodiscard]] glm::mat4x4 GetTransformationMatrix(size_t id) const;

public:
    explicit TessellationHelper(TransformStruct* transform);
    [[nodiscard]] Transform* GetTransform(size_t id) const;
    uint16_t AddVertex(const Vertex& vertex);
    void AddTriangle(uint16_t triangle) override;
    void Reset() override;
    void Draw() override;
    void FreeMemory() override;
    void Draw(size_t transformId, size_t startPos, size_t count);
    size_t AddTransform(Transform* transform);
};
