//
// Created by amit on 4/21/2022.
//

#ifndef MINECRAFTPROJECT_VERTEX_H
#define MINECRAFTPROJECT_VERTEX_H
#include <glm/glm.hpp>
using glm::vec3;
using glm::vec2;
using glm::vec4;

enum class VertexUsage : std::uint8_t {
    POSITION_AND_COLOR, POSITION_AND_TEXTURE_COORDS_AND_COLOR
};

struct Vertex {
public:
    vec3 position;
    vec2 texCoords;
    vec4 color;
    VertexUsage usage;
    [[nodiscard]] VertexUsage getVertexUsage() const;
    Vertex(float x, float y, float z, float u, float v, float r, float g, float b, float a);
    Vertex(float x, float y, float z, float r, float g, float b, float a);

    [[nodiscard]] vec3 getPosition() const;
    [[nodiscard]] vec2 getTexCoords() const;
    [[nodiscard]] vec4 getColor() const;
};
#endif //MINECRAFTPROJECT_VERTEX_H
