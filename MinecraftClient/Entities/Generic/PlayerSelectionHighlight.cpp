#include "PlayerSelectionHighlight.h"

#include "CameraController.h"
#include "Util/EngineDefaultsClient.h"
#include "World/Blocks/AirBlock.h"
#include "World/Blocks/BlockTypeList.h"
#include "World/Generic/Blocks/BlockRenderer.h"
#include "World/Generic/Blocks/BlockRendererList.h"
#include "World/World.h"

PlayerSelectionHighlight::PlayerSelectionHighlight(const PlayerState& player) : Tessellation(&Transform), TextureIndex(EngineDefaultsClient::RegisterTexture(Texture::LoadTexture("Textures/HighlightTexture.png"))),
	Player(player), BlockHit(nullptr), FaceHit(BlockFaces::Bottom), HitPosition(0, 0, 0)
{
}

void PlayerSelectionHighlight::Reset()
{
	BlockHit = nullptr;
}

void PlayerSelectionHighlight::Render()
{
	Tessellation.Reset();
	BlockHit = nullptr;
	FindClosestFace();
	if (BlockHit == nullptr)
	{
		return;
	}
	Transform.Position = glm::vec3(HitPosition.x, HitPosition.y, HitPosition.z);
	const auto helper = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
	if (Player.Mode)
	{
		const float bright = static_cast<float>(sin(static_cast<double>(helper.count()) / 100.0)) * 0.2F + 0.8F;
		const float alpha = static_cast<float>(sin(static_cast<double>(helper.count()) / 200.0)) * 0.2F + 0.5F;
		int brightness;
		switch (FaceHit)
		{
		case BlockFaces::Top:
			brightness = World::GetWorld()->GetBrightnessAt(HitPosition.x, HitPosition.y + 1, HitPosition.z);
			BlockRendererList::GetBlockRenderer(Player.CurrentSelectedBlock)->GenerateTessellationDataForAllFaces(BlockTypeList::GetBlockTypeData(Player.CurrentSelectedBlock), Tessellation, 0.0F, 1.0F, 0.0F, brightness, bright, bright, bright, alpha);
			break;
		case BlockFaces::Bottom:
			brightness = World::GetWorld()->GetBrightnessAt(HitPosition.x, HitPosition.y - 1, HitPosition.z);
			BlockRendererList::GetBlockRenderer(Player.CurrentSelectedBlock)->GenerateTessellationDataForAllFaces(BlockTypeList::GetBlockTypeData(Player.CurrentSelectedBlock), Tessellation, 0.0F, -1.0F, 0.0F, brightness, bright, bright, bright, alpha);
			break;
		case BlockFaces::North:
			brightness = World::GetWorld()->GetBrightnessAt(HitPosition.x, HitPosition.y, HitPosition.z + 1);
			BlockRendererList::GetBlockRenderer(Player.CurrentSelectedBlock)->GenerateTessellationDataForAllFaces(BlockTypeList::GetBlockTypeData(Player.CurrentSelectedBlock), Tessellation, 0.0F, 0.0F, 1.0F, brightness, bright, bright, bright, alpha);
			break;
		case BlockFaces::South:
			brightness = World::GetWorld()->GetBrightnessAt(HitPosition.x, HitPosition.y, HitPosition.z - 1);
			BlockRendererList::GetBlockRenderer(Player.CurrentSelectedBlock)->GenerateTessellationDataForAllFaces(BlockTypeList::GetBlockTypeData(Player.CurrentSelectedBlock), Tessellation, 0.0F, 0.0F, -1.0F, brightness, bright, bright, bright, alpha);
			break;
		case BlockFaces::East:
			brightness = World::GetWorld()->GetBrightnessAt(HitPosition.x + 1, HitPosition.y, HitPosition.z);
			BlockRendererList::GetBlockRenderer(Player.CurrentSelectedBlock)->GenerateTessellationDataForAllFaces(BlockTypeList::GetBlockTypeData(Player.CurrentSelectedBlock), Tessellation, 1.0F, 0.0F, 0.0F, brightness, bright, bright, bright, alpha);
			break;
		case BlockFaces::West:
			brightness = World::GetWorld()->GetBrightnessAt(HitPosition.x - 1, HitPosition.y, HitPosition.z);
			BlockRendererList::GetBlockRenderer(Player.CurrentSelectedBlock)->GenerateTessellationDataForAllFaces(BlockTypeList::GetBlockTypeData(Player.CurrentSelectedBlock), Tessellation, -1.0F, 0.0F, 0.0F, brightness, bright, bright, bright, alpha);
			break;
		}
	}
	else
	{
		const int brightness = World::GetWorld()->GetBrightnessAt(HitPosition.x, HitPosition.y, HitPosition.z);
		const float alpha = (static_cast<float>(sin(static_cast<double>(helper.count()) / 100.0)) * 0.2F + 0.4F) * 0.5F;
		const BoundingBox blockBounds = BlockHit->GetBoundingBox();
		constexpr float offset = 0.0001F;
		uint16_t triangleIndex1 = Tessellation.AddVertex(Vertex(blockBounds.GetMinX(),
		                                                        blockBounds.GetMaxY() + offset,
		                                                        blockBounds.GetMinZ(), 0, 0, 1.0F, 1.0F, 1.0F, alpha,
		                                                        TextureIndex, 0.0F, 1.0F, 0.0F, brightness));
		uint16_t triangleIndex2 = Tessellation.AddVertex(Vertex(blockBounds.GetMinX(),
		                                                        blockBounds.GetMaxY() + offset,
		                                                        blockBounds.GetMaxZ(), 0, 1, 1.0F, 1.0F, 1.0F, alpha,
		                                                        TextureIndex, 0.0F, 1.0F, 0.0F, brightness));
		uint16_t triangleIndex3 = Tessellation.AddVertex(Vertex(blockBounds.GetMaxX(),
		                                                        blockBounds.GetMaxY() + offset,
		                                                        blockBounds.GetMinZ(), 1, 0, 1.0F, 1.0F, 1.0F, alpha,
		                                                        TextureIndex, 0.0F, 1.0F, 0.0F, brightness));
		uint16_t triangleIndex4 = Tessellation.AddVertex(Vertex(blockBounds.GetMaxX(),
		                                                        blockBounds.GetMaxY() + offset,
		                                                        blockBounds.GetMaxZ(), 1, 1, 1.0F, 1.0F, 1.0F, alpha,
		                                                        TextureIndex, 0.0F, 1.0F, 0.0F, brightness));
		Tessellation.AddTriangle(triangleIndex1);
		Tessellation.AddTriangle(triangleIndex2);
		Tessellation.AddTriangle(triangleIndex3);
		Tessellation.AddTriangle(triangleIndex4);
		Tessellation.AddTriangle(triangleIndex3);
		Tessellation.AddTriangle(triangleIndex2);
		triangleIndex1 = Tessellation.AddVertex(Vertex(blockBounds.GetMinX(), blockBounds.GetMinY() - offset, blockBounds.GetMinZ(), 0, 0, 1.0F, 1.0F, 1.0F, alpha, TextureIndex, 0.0F, -1.0F, 0.0F, brightness));
		triangleIndex2 = Tessellation.AddVertex(Vertex(blockBounds.GetMaxX(), blockBounds.GetMinY() - offset, blockBounds.GetMinZ(), 1, 0, 1.0F, 1.0F, 1.0F, alpha, TextureIndex, 0.0F, -1.0F, 0.0F, brightness));
		triangleIndex3 = Tessellation.AddVertex(Vertex(blockBounds.GetMinX(), blockBounds.GetMinY() - offset, blockBounds.GetMaxZ(), 0, 1, 1.0F, 1.0F, 1.0F, alpha, TextureIndex, 0.0F, -1.0F, 0.0F, brightness));
		triangleIndex4 = Tessellation.AddVertex(Vertex(blockBounds.GetMaxX(), blockBounds.GetMinY() - offset, blockBounds.GetMaxZ(), 1, 1, 1.0F, 1.0F, 1.0F, alpha, TextureIndex, 0.0F, -1.0F, 0.0F, brightness));
		Tessellation.AddTriangle(triangleIndex1);
		Tessellation.AddTriangle(triangleIndex2);
		Tessellation.AddTriangle(triangleIndex3);
		Tessellation.AddTriangle(triangleIndex4);
		Tessellation.AddTriangle(triangleIndex3);
		Tessellation.AddTriangle(triangleIndex2);
		triangleIndex1 = Tessellation.AddVertex(Vertex(blockBounds.GetMinX(), blockBounds.GetMinY(), blockBounds.GetMinZ() - offset, 0, 0, 1.0F, 1.0F, 1.0F, alpha, TextureIndex, 0.0F, 0.0F, -1.0F, brightness));
		triangleIndex2 = Tessellation.AddVertex(Vertex(blockBounds.GetMinX(), blockBounds.GetMaxY(), blockBounds.GetMinZ() - offset, 0, 1, 1.0F, 1.0F, 1.0F, alpha, TextureIndex, 0.0F, 0.0F, -1.0F, brightness));
		triangleIndex3 = Tessellation.AddVertex(Vertex(blockBounds.GetMaxX(), blockBounds.GetMinY(), blockBounds.GetMinZ() - offset, 1, 0, 1.0F, 1.0F, 1.0F, alpha, TextureIndex, 0.0F, 0.0F, -1.0F, brightness));
		triangleIndex4 = Tessellation.AddVertex(Vertex(blockBounds.GetMaxX(), blockBounds.GetMaxY(), blockBounds.GetMinZ() - offset, 1, 1, 1.0F, 1.0F, 1.0F, alpha, TextureIndex, 0.0F, 0.0F, -1.0F, brightness));
		Tessellation.AddTriangle(triangleIndex1);
		Tessellation.AddTriangle(triangleIndex2);
		Tessellation.AddTriangle(triangleIndex3);
		Tessellation.AddTriangle(triangleIndex4);
		Tessellation.AddTriangle(triangleIndex3);
		Tessellation.AddTriangle(triangleIndex2);
		triangleIndex1 = Tessellation.AddVertex(Vertex(blockBounds.GetMinX(), blockBounds.GetMinY(), blockBounds.GetMaxZ() + offset, 0, 0, 1.0F, 1.0F, 1.0F, alpha, TextureIndex, 0.0F, 0.0F, 1.0F, brightness));
		triangleIndex2 = Tessellation.AddVertex(Vertex(blockBounds.GetMaxX(), blockBounds.GetMinY(), blockBounds.GetMaxZ() + offset, 1, 0, 1.0F, 1.0F, 1.0F, alpha, TextureIndex, 0.0F, 0.0F, 1.0F, brightness));
		triangleIndex3 = Tessellation.AddVertex(Vertex(blockBounds.GetMinX(), blockBounds.GetMaxY(), blockBounds.GetMaxZ() + offset, 0, 1, 1.0F, 1.0F, 1.0F, alpha, TextureIndex, 0.0F, 0.0F, 1.0F, brightness));
		triangleIndex4 = Tessellation.AddVertex(Vertex(blockBounds.GetMaxX(), blockBounds.GetMaxY(), blockBounds.GetMaxZ() + offset, 1, 1, 1.0F, 1.0F, 1.0F, alpha, TextureIndex, 0.0F, 0.0F, 1.0F, brightness));
		Tessellation.AddTriangle(triangleIndex1);
		Tessellation.AddTriangle(triangleIndex2);
		Tessellation.AddTriangle(triangleIndex3);
		Tessellation.AddTriangle(triangleIndex4);
		Tessellation.AddTriangle(triangleIndex3);
		Tessellation.AddTriangle(triangleIndex2);
		triangleIndex1 = Tessellation.AddVertex(Vertex(blockBounds.GetMaxX() + offset, blockBounds.GetMinY(), blockBounds.GetMinZ(), 0, 0, 1.0F, 1.0F, 1.0F, alpha, TextureIndex, 1.0F, 0.0F, 0.0F, brightness));
		triangleIndex2 = Tessellation.AddVertex(Vertex(blockBounds.GetMaxX() + offset, blockBounds.GetMaxY(), blockBounds.GetMinZ(), 0, 1, 1.0F, 1.0F, 1.0F, alpha, TextureIndex, 1.0F, 0.0F, 0.0F, brightness));
		triangleIndex3 = Tessellation.AddVertex(Vertex(blockBounds.GetMaxX() + offset, blockBounds.GetMinY(), blockBounds.GetMaxZ(), 1, 0, 1.0F, 1.0F, 1.0F, alpha, TextureIndex, 1.0F, 0.0F, 0.0F, brightness));
		triangleIndex4 = Tessellation.AddVertex(Vertex(blockBounds.GetMaxX() + offset, blockBounds.GetMaxY(), blockBounds.GetMaxZ(), 1, 1, 1.0F, 1.0F, 1.0F, alpha, TextureIndex, 1.0F, 0.0F, 0.0F, brightness));
		Tessellation.AddTriangle(triangleIndex1);
		Tessellation.AddTriangle(triangleIndex2);
		Tessellation.AddTriangle(triangleIndex3);
		Tessellation.AddTriangle(triangleIndex4);
		Tessellation.AddTriangle(triangleIndex3);
		Tessellation.AddTriangle(triangleIndex2);
		triangleIndex1 = Tessellation.AddVertex(Vertex(blockBounds.GetMinX() - offset, blockBounds.GetMinY(), blockBounds.GetMinZ(), 0, 0, 1.0F, 1.0F, 1.0F, alpha, TextureIndex, -1.0F, 0.0F, 0.0F, brightness));
		triangleIndex2 = Tessellation.AddVertex(Vertex(blockBounds.GetMinX() - offset, blockBounds.GetMinY(), blockBounds.GetMaxZ(), 1, 0, 1.0F, 1.0F, 1.0F, alpha, TextureIndex, -1.0F, 0.0F, 0.0F, brightness));
		triangleIndex3 = Tessellation.AddVertex(Vertex(blockBounds.GetMinX() - offset, blockBounds.GetMaxY(), blockBounds.GetMinZ(), 0, 1, 1.0F, 1.0F, 1.0F, alpha, TextureIndex, -1.0F, 0.0F, 0.0F, brightness));
		triangleIndex4 = Tessellation.AddVertex(Vertex(blockBounds.GetMinX() - offset, blockBounds.GetMaxY(), blockBounds.GetMaxZ(), 1, 1, 1.0F, 1.0F, 1.0F, alpha, TextureIndex, -1.0F, 0.0F, 0.0F, brightness));
		Tessellation.AddTriangle(triangleIndex1);
		Tessellation.AddTriangle(triangleIndex2);
		Tessellation.AddTriangle(triangleIndex3);
		Tessellation.AddTriangle(triangleIndex4);
		Tessellation.AddTriangle(triangleIndex3);
		Tessellation.AddTriangle(triangleIndex2);
	}
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	Tessellation.Draw();
	glDisable(GL_BLEND);
}

float PlayerSelectionHighlight::CalculateMaxDistanceForHighlight(const glm::vec3& front, const bool up, const bool right,
                                                                 const bool forward) const
{
	float xDistance = right ? 4.0F + EngineDefaults::PLAYER_SIZE.x : 3.0F + EngineDefaults::PLAYER_SIZE.x;
	float yDistance = up ? 4.0F + 2 * EngineDefaults::PLAYER_SIZE.y - EngineDefaults::CAMERA_OFFSET : 3.0F + EngineDefaults::CAMERA_OFFSET;
	float zDistance = forward ? 4.0F + EngineDefaults::PLAYER_SIZE.z : 3.0F + EngineDefaults::PLAYER_SIZE.z;
	xDistance /= front.x;
	yDistance /= front.y;
	zDistance /= front.z;
	xDistance = abs(xDistance);
	yDistance = abs(yDistance);
	zDistance = abs(zDistance);
	return glm::min(xDistance, glm::min(yDistance, zDistance));
}

void PlayerSelectionHighlight::FindClosestFace()
{
	const glm::vec3 frontVector = CameraController::GetActiveCamera().GetFrontVector();
	const glm::vec3 cameraPos = CameraController::GetActiveCamera().Position;
	const bool right = frontVector.x > 0.0F;
	const bool up = frontVector.y > 0.0F;
	const bool forward = frontVector.z > 0.0F;
	float totalDistance = 0.0F;
	const float maxDistance = CalculateMaxDistanceForHighlight(frontVector, up, right, forward);
	float xDistance = cameraPos.x;
	float yDistance = cameraPos.y;
	float zDistance = cameraPos.z;
	while (totalDistance <= maxDistance)
	{
		float distanceForX = ((right ? floor(xDistance) : ceil(xDistance)) - xDistance + (right ? 1.0F : -1.0F)) / frontVector.x;
		float distanceForY = ((up ? floor(yDistance) : ceil(yDistance)) - yDistance + (up ? 1.0F : -1.0F)) / frontVector.y;
		float distanceForZ = ((forward ? floor(zDistance) : ceil(zDistance)) - zDistance + (forward ? 1.0F : -1.0F)) / frontVector.z;
		float distanceForXAbs = abs(distanceForX);
		if (distanceForXAbs < 0.000001F)
		{
			distanceForXAbs = 1.0F;
			distanceForX += right ? 1.0F : -1.0F;
		}
		float distanceForYAbs = abs(distanceForY);
		if (distanceForYAbs < 0.000001F)
		{
			distanceForYAbs = 1.0F;
			distanceForY += up ? 1.0F : -1.0F;
		}
		float distanceForZAbs = abs(distanceForZ);
		if (distanceForZAbs < 0.000001F)
		{
			distanceForZAbs = 1.0F;
			distanceForZ += forward ? 1.0F : -1.0F;
		}
		float minDistance;
		int xyzChoice;
		if (distanceForXAbs < distanceForYAbs && distanceForXAbs < distanceForZAbs)
		{
			minDistance = distanceForX;
			xyzChoice = 0;
		}
		else if (distanceForYAbs < distanceForZAbs)
		{
			minDistance = distanceForY;
			xyzChoice = 1;
		}
		else
		{
			minDistance = distanceForZ;
			xyzChoice = 2;
		}
		if (totalDistance + abs(minDistance) <= maxDistance)
		{
			xDistance += frontVector.x * minDistance;
			yDistance += frontVector.y * minDistance;
			zDistance += frontVector.z * minDistance;
			HitPosition = glm::ivec3(static_cast<int>(floor(xDistance)) - (!right && xyzChoice == 0 ? 1 : 0), static_cast<int>(floor(yDistance)) - (!up && xyzChoice == 1 ? 1 : 0), static_cast<int>(floor(zDistance)) - (!forward && xyzChoice == 2 ? 1 : 0));
			if (const EBlockType blockTypeHit = World::GetWorld()->GetBlockTypeAt(HitPosition.x, HitPosition.y, HitPosition.z); blockTypeHit != EBlockType::Air)
			{
				BlockHit = BlockTypeList::GetBlockTypeData(blockTypeHit);
				if (xyzChoice == 0 && frontVector.x > 0.0F)
				{
					FaceHit = BlockFaces::West;
				}
				if (xyzChoice == 0 && frontVector.x < 0.0F)
				{
					FaceHit = BlockFaces::East;
				}
				if (xyzChoice == 1 && frontVector.y > 0.0F)
				{
					FaceHit = BlockFaces::Bottom;
				}
				if (xyzChoice == 1 && frontVector.y < 0.0F)
				{
					FaceHit = BlockFaces::Top;
				}
				if (xyzChoice == 2 && frontVector.z > 0.0F)
				{
					FaceHit = BlockFaces::South;
				}
				if (xyzChoice == 2 && frontVector.z < 0.0F)
				{
					FaceHit = BlockFaces::North;
				}
				return;
			}
		}
		totalDistance += abs(minDistance);
	}
}
