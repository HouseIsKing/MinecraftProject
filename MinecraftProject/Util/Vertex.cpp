//
// Created by amit on 4/21/2022.
//

#include "Vertex.h"

Vertex::Vertex(const float x, const float y, const float z, const float u, const float v, const float r, const float g,
               const float b, const float a, const uint16_t indexTexture, const float normalX, const float normalY,
               const float normalZ, const int brightness) : Position(x, y, z), TexCoords(u, v), Color(r, g, b, a),
                                                            IndexTexture(indexTexture), Normal(normalX, normalY, normalZ), Brightness(brightness)
{
}

Vertex::Vertex(const vec3& pos, const float u, const float v, const float r, const float g, const float b, const float a, const uint16_t indexTexture, const float normalX,
               const float normalY, const float normalZ, const int brightness) : Vertex(pos.x, pos.y, pos.z, u, v, r, g, b, a, indexTexture, normalX, normalY, normalZ, brightness)
{
}
