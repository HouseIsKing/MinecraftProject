#include "AirBlock.h"

AirBlock::AirBlock() = default;

void AirBlock::GenerateTessellationData(TessellationHelper& /*tessellationHelper*/, const BlockFaces& /*face*/, const float& /*x*/, const float& /*y*/, const float& /*z*/, const int& /*brightness*/, const float /*r*/, const float /*g*/, const float /*b*/, const float /*a*/) const
{
}

size_t AirBlock::GetIndexTextureSide(BlockFaces /*face*/) const
{
    return 0;
}

void AirBlock::GenerateTessellationData(GuiTessellation& /*tessellationHelper*/, const BlockFaces& /*face*/,
                                        mat4x4 /*transformationMatrix*/) const
{
}

bool AirBlock::IsSolidBlock() const
{
    return false;
}

bool AirBlock::IsBlockingLight() const
{
    return false;
}
