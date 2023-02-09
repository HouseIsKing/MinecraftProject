#include "AirBlockRenderer.h"

void AirBlockRenderer::GenerateTessellationData(const Block* /*block*/, TessellationHelper& /*tessellationHelper*/, const BlockFaces& /*face*/, const float& /*x*/, const float& /*y*/, const float& /*z*/, const int& /*brightness*/, const float /*r*/, const float /*g*/, const float /*b*/, const float /*a*/) const
{
}

size_t AirBlockRenderer::GetIndexTextureSide(BlockFaces /*face*/) const
{
    return 0;
}

void AirBlockRenderer::GenerateTessellationData(const Block* /*block*/, GuiTessellation& /*tessellationHelper*/, const BlockFaces& /*face*/, glm::mat4x4 /*transformationMatrix*/) const
{
}