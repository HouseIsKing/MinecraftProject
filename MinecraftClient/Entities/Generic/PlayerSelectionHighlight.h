#pragma once
#include "Util/TessellationHelper.h"
#include "World/Blocks/Block.h"
#include <chrono>

#include "Util/States/PlayerState.h"

class PlayerSelectionHighlight
{
	TessellationHelper Tessellation;
	TransformStruct Transform;
	uint16_t TextureIndex;
	const uint16_t PlayerId;
	void Reset();
	const Block* BlockHit;
	BlockFaces FaceHit;
	glm::ivec3 HitPosition;
	[[nodiscard]] float CalculateMaxDistanceForHighlight(const glm::vec3& front, bool up, bool right, bool forward) const;
	void FindClosestFace();

public:
	explicit PlayerSelectionHighlight(const uint16_t playerId);
	void Render();
};
