#pragma once
#include "TessellationInterface.h"
#include "Transform.h"
#include "Vertex.h"
#include <vector>

using std::vector;

class TessellationHelper final : public TessellationInterface
{
    vector<Vertex> Vertices{};
    vector<unique_ptr<Transform>> TessellationTransforms{};
    int PositionUniform;
    [[nodiscard]] mat4x4 GetTransformationMatrix(size_t id) const;

public:
    TessellationHelper();
    TessellationHelper(float x, float y, float z);
    [[nodiscard]] Transform* GetTransform(size_t id) const;
    uint16_t AddVertex(const Vertex& vertex);
    void AddTriangle(uint16_t triangle) override;
    void Reset() override;
    void Draw() override;
    void FreeMemory() override;
    void Draw(size_t transformId, size_t startPos, size_t count);
    size_t AddTransform(Transform* transform);
};
