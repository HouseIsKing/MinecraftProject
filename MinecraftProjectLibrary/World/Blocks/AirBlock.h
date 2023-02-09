#pragma once
#include "Block.h"

class AirBlock final : public Block
{
public:
	AirBlock() = default;
	[[nodiscard]] bool IsSolidBlock() const override;
	[[nodiscard]] bool IsBlockingLight() const override;
};

