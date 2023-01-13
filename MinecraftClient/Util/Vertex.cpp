#include "Vertex.h"

Vertex::Vertex(const float x, const float y, const float z, const float u, const float v, const float r, const float g,
               const float b, const float a, const uint16_t indexTexture, const float normalX, const float normalY,
               const float normalZ, const int brightness) : Vertex(x, y, z, u, v, r, g, b, a, indexTexture, normalX, normalY, normalZ, brightness, 0)
{
}

Vertex::Vertex(const glm::vec3& pos, const float u, const float v, const float r, const float g, const float b, const float a, const uint16_t indexTexture, const float normalX,
               const float normalY, const float normalZ, const int brightness) : Vertex(pos.x, pos.y, pos.z, u, v, r, g, b, a, indexTexture, normalX, normalY, normalZ, brightness)
{
}

Vertex::Vertex(const float x, const float y, const float z, const float u, const float v, const float r, const float g, const float b, const float a, const uint16_t indexTexture,
               const float normalX, const float normalY, const float normalZ, const int brightness, const unsigned int specialFactors) : Position(x, y, z), TexCoords(u, v), Color(r, g, b, a),
    IndexTexture(indexTexture), Normal(normalX, normalY, normalZ), Brightness(brightness), SpecialFactors(specialFactors)
{
}
