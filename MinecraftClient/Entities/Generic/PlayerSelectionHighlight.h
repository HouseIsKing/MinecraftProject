#pragma once
#include "Util/EngineDefaults.h"
#include "World/Generic/Blocks/Block.h"
#include "World/Generic/Blocks/BlockTypeList.h"
#include <chrono>

class PlayerController;

class PlayerSelectionHighlight
{
	TessellationHelper Tessellation;
	TransformStruct Transform;
	uint16_t TextureIndex;
	PlayerController* Player;

public:
	explicit PlayerSelectionHighlight(PlayerController* player);
	void Reset();
	const Block* BlockHit;
	BlockFaces FaceHit;
	glm::ivec3 HitPosition;
	void Render(const int& brightness);
};