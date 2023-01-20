#include "OtherPlayer.h"

void OtherPlayer::HandleEntityUpdate(const EntityDataPacket& packet)
{
    LivingEntity::HandleEntityUpdate(packet);
}

EEntityType OtherPlayer::GetEntityType() const
{
    return EEntityType::Player;
}
