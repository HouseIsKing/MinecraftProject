#include "Block.h"

#include <glm/detail/func_geometric.inl>

#include "World/World.h"

Block::Block() : Block(0, 0, 0, 1, 1, 1)
{
}

Block::Block(const float minX, const float minY, const float minZ, const float maxX, const float maxY, const float maxZ) : BlockBounds(minX, minY, minZ, maxX, maxY, maxZ)
{
}

BoundingBox Block::GetBoundingBox() const
{
	return BlockBounds;
}

void Block::Tick(World* /*world*/, int /*x*/, int /*y*/, int /*z*/) const
{
}

void Block::OnBreak(const int x, const int y, const int z) const
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				const float posX = static_cast<float>(x) + (static_cast<float>(i) + 0.5F) / 4.0F;
				const float posY = static_cast<float>(y) + (static_cast<float>(j) + 0.5F) / 4.0F;
				const float posZ = static_cast<float>(z) + (static_cast<float>(k) + 0.5F) / 4.0F;
				CustomRandomEngine& randomEngine = World::GetWorld()->TickRandomEngine;
				const float xSpeed = posX - static_cast<float>(x) - 0.5F + (randomEngine.GetNextFloat() * 2.0F - 1.0F) * 0.4F;
				const float ySpeed = posY - static_cast<float>(y) - 0.5F + (randomEngine.GetNextFloat() * 2.0F - 1.0F) * 0.4F;
				const float zSpeed = posZ - static_cast<float>(z) - 0.5F + (randomEngine.GetNextFloat() * 2.0F - 1.0F) * 0.4F;
				const float f = (randomEngine.GetNextFloat() + randomEngine.GetNextFloat() + 1.0F) * 0.15F;
				const float distance = length(glm::vec3(xSpeed, ySpeed, zSpeed));
				auto* const state = dynamic_cast<BlockParticleEntityState*>(World::GetWorld()->EntitiesToAdd.emplace_back(new BlockParticleEntityState{}).get());
				state->EntityTransform.Position = glm::vec3(posX, posY, posZ);
				state->EntityTransform.Scale = EngineDefaults::PARTICLE_SIZE * (World::GetWorld()->TickRandomEngine.GetNextFloat() * 0.5F + 0.5F);
				state->BlockParticleType = World::GetWorld()->GetBlockTypeAt(x, y, z);
				state->EntityVelocity = glm::vec3(xSpeed / distance * f * 0.7F, ySpeed / distance * f, zSpeed / distance * f * 0.7F);
				state->LifeTime = static_cast<uint8_t>(4.0F / (randomEngine.GetNextFloat() * 0.9F + 0.1F));
				//state->LifeTime = 40;
			}
		}
	}
}

bool Block::IsSolidBlock() const
{
	return true;
}

bool Block::IsBlockingLight() const
{
	return true;
}
