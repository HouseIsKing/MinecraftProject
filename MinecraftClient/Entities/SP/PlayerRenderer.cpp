#include "PlayerRenderer.h"

#include "World/World.h"

PlayerRenderer::PlayerRenderer(const uint16_t entityId) : LivingEntityRenderer(entityId)
{
}

const EntityState& PlayerRenderer::GetState()
{
    return World::GetWorld()->GetEntityState<PlayerStateWrapper, PlayerState>(EntityId).first;
}

EEntityType PlayerRenderer::GetEntityTypeRenderer()
{
    return EEntityType::Player;
}
