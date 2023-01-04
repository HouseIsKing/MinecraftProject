//
// Created by amit on 4/21/2022.
//

#pragma once
#include "TessellationInterface.h"
#include "Transform.h"
#include "Vertex.h"
#include <vector>

using std::vector;

class TessellationHelper final : public TessellationInterface
{
    vector<Vertex> Vertices{};
    vector<Transform> TessellationTransforms{};
    int PositionUniform;
    mat4x4 GetTransformationMatrix(size_t id);

public:
    TessellationHelper();
    TessellationHelper(float x, float y, float z);
    [[nodiscard]] Transform& GetTransform(size_t id);
    uint16_t AddVertex(const Vertex& vertex);
    void AddTriangle(uint16_t triangle) override;
    void Reset() override;
    void Draw() override;
    void FreeMemory() override;
    void Draw(size_t transformId, size_t startPos, size_t count);
    size_t AddTransform(Transform transform);
};
