#include "GuiVertex.h"

GuiVertex::GuiVertex(const float x, const float y, const float z, const float u, const float v, const float r, const float g, const float b, const float a,
                     const uint16_t indexTexture) : Position(x, y, z), TexCoords(u, v), Color(r, g, b, a), IndexTexture(indexTexture)
{
}

GuiVertex::GuiVertex(const glm::vec3& pos, float u, float v, float r, float g, float b, float a, uint16_t indexTexture) : GuiVertex(pos.x, pos.y, pos.z, u, v, r, g, b, a, indexTexture)
{
}
