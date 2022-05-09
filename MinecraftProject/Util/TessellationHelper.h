//
// Created by amit on 4/21/2022.
//

#pragma once
#include "../Shaders/Shader.h"
#include "../Textures/Texture.h"
#include "Transform.h"
#include "Vertex.h"
#include <vector>

using std::vector;

class TessellationHelper
{
    GLuint Vbo;
    GLuint Vao;
    GLuint Ebo;
    vector<Vertex> Vertices{};
    vector<GLushort> TriangleIndices{};
    Transform TessellationTransform;
    Shader* TheShader;
    bool hasInit;
    int PositionUniform;
public:
    void Reset();
    ~TessellationHelper();
    [[nodiscard]] bool HasInit() const;
    explicit TessellationHelper(Shader* shader);
    TessellationHelper(Shader* shader, float x, float y, float z);
    [[nodiscard]] Transform& GetTransform();
    uint16_t AddVertex(const Vertex& vertex);
    void AddTriangle(uint16_t triangle);
    //void changeVertex(uint16_t vertexID, Vertex vertex);
    void Draw();
};
