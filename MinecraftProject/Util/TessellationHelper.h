//
// Created by amit on 4/21/2022.
//

#ifndef MINECRAFTPROJECT_TESSELLATIONHELPER_H
#define MINECRAFTPROJECT_TESSELLATIONHELPER_H
#include "Vertex.h"
#include "Transform.h"
#include "../Shaders/Shader.h"
#include "../Textures/Texture.h"
#include <vector>
#include <unordered_map>

using std::unordered_map;
using std::vector;

class TessellationHelper {
private:
    GLuint vbo;
    GLuint vao;
    vector<Vertex> vertices;
    unordered_map<Texture, GLuint> elementBuffers;
    unordered_map<Texture, vector<uint16_t>> textureIDToTriangles;
    Transform transform;
    Shader* theShader;
    bool hasInit;
public:
    void reset();
    ~TessellationHelper();
    [[nodiscard]] bool HasInit() const;
    explicit TessellationHelper(Shader* shader);
    TessellationHelper(Shader* shader, float x, float y, float z);
    [[nodiscard]] Transform getTransform() const;
    uint16_t addVertex(Vertex vertex);
    void addTriangle(Texture textureID, uint16_t triangle);
    void changeVertex(uint16_t vertexID, Vertex vertex);
    void draw();
};
#endif //MINECRAFTPROJECT_TESSELLATIONHELPER_H
