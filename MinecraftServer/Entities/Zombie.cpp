#include "Zombie.h"

#include <GLFW/glfw3.h>
#include <glm/ext/scalar_constants.hpp>

#include "Util/EngineDefaults.h"
#include "World/MultiPlayerWorld.h"

/*
Zombie::Zombie(const float x, const float y, const float z) :
    LivingEntity(ZOMBIE_SIZE, x, y, z),
    TimeOffset(static_cast<float>(GetWorld()->GetRandomEngineState().GetNext(1239813))),
    NextRotation((GetWorld()->GetRandomEngineState().GetNextFloat() + 1.0F) * 0.01F),
    Head(0.0F, 24 * ZOMBIE_SCALE_FACTOR - ZOMBIE_SIZE.y, 0.0F, -4 * ZOMBIE_SCALE_FACTOR, 0,
         -4 * ZOMBIE_SCALE_FACTOR, 4 * ZOMBIE_SCALE_FACTOR, 8 * ZOMBIE_SCALE_FACTOR, 4 * ZOMBIE_SCALE_FACTOR, &GetTransform()),
    Body(0.0F, 24 * ZOMBIE_SCALE_FACTOR - ZOMBIE_SIZE.y, 0.0F, -2 * ZOMBIE_SCALE_FACTOR,
         -12 * ZOMBIE_SCALE_FACTOR, -4 * ZOMBIE_SCALE_FACTOR, 2 * ZOMBIE_SCALE_FACTOR, 0, 4 * ZOMBIE_SCALE_FACTOR,
         &GetTransform()),
    RightArm(0.0F, 22 * ZOMBIE_SCALE_FACTOR - ZOMBIE_SIZE.y, -5 * ZOMBIE_SCALE_FACTOR,
             -2 * ZOMBIE_SCALE_FACTOR, -10 * ZOMBIE_SCALE_FACTOR, -3 * ZOMBIE_SCALE_FACTOR, 2 * ZOMBIE_SCALE_FACTOR,
             2 * ZOMBIE_SCALE_FACTOR, ZOMBIE_SCALE_FACTOR, &GetTransform()),
    LeftArm(0.0F, 22 * ZOMBIE_SCALE_FACTOR - ZOMBIE_SIZE.y, 5 * ZOMBIE_SCALE_FACTOR, -2 * ZOMBIE_SCALE_FACTOR,
            -10 * ZOMBIE_SCALE_FACTOR, -ZOMBIE_SCALE_FACTOR, 2 * ZOMBIE_SCALE_FACTOR, 2 * ZOMBIE_SCALE_FACTOR,
            3 * ZOMBIE_SCALE_FACTOR, &GetTransform()),
    RightLeg(0.0F, 12 * ZOMBIE_SCALE_FACTOR - ZOMBIE_SIZE.y, -2 * ZOMBIE_SCALE_FACTOR,
             -2 * ZOMBIE_SCALE_FACTOR, -12 * ZOMBIE_SCALE_FACTOR, -2 * ZOMBIE_SCALE_FACTOR,
             2 * ZOMBIE_SCALE_FACTOR, 0, 2 * ZOMBIE_SCALE_FACTOR, &GetTransform()),
    LeftLeg(0.0F, 12 * ZOMBIE_SCALE_FACTOR - ZOMBIE_SIZE.y, 2 * ZOMBIE_SCALE_FACTOR,
            -2 * ZOMBIE_SCALE_FACTOR, -12 * ZOMBIE_SCALE_FACTOR, -2 * ZOMBIE_SCALE_FACTOR, 2 * ZOMBIE_SCALE_FACTOR, 0, 2 * ZOMBIE_SCALE_FACTOR,
            &GetTransform())
{
    GetTransform().SetRotation(0.0F, GetWorld()->GetRandomEngineState().GetNextFloat() * 360.0F, 0.0F);
    const glm::vec3 pos = GetTransform().GetPosition();
    const int brightness = GetWorld()->GetBrightnessAt(static_cast<int>(pos.x), static_cast<int>(pos.y), static_cast<int>(pos.z));
    PreviousLightLevel = brightness;
}

void Zombie::Tick()
{
    LivingEntity::Tick();
    GetTransform().Rotate(0, glm::degrees(NextRotation), 0);
    NextRotation *= 0.99F;
    NextRotation += (GetWorld()->GetRandomEngineState().GetNextFloat() - GetWorld()->GetRandomEngineState().GetNextFloat()) * GetWorld()->GetRandomEngineState().GetNextFloat() * GetWorld()->GetRandomEngineState().GetNextFloat() * 0.01F;
    VerticalInput = 1;
    JumpRequested = GetWorld()->GetRandomEngineState().GetNextFloat() < 0.08F;
    if (GetTransform().GetPosition().y < -100.0F)
    {
        GetWorld()->RemoveEntity(GetEntityId());
    }
    const double time = glfwGetTime() * 10.0 + static_cast<double>(TimeOffset);
    Head.GetTransform().SetRotation(0.0F, glm::degrees(static_cast<float>(sin(time * 0.83))), glm::degrees(static_cast<float>(sin(time)) * 0.8F));
    RightArm.GetTransform().SetRotation(glm::degrees(static_cast<float>(sin(time * 0.2312) + 1.0)), 0.0F, glm::degrees(static_cast<float>(sin(time * 0.662 + glm::pi<double>()))) * 2.0F);
    LeftArm.GetTransform().SetRotation(glm::degrees(static_cast<float>(sin(time * 0.2312) - 1.0)), 0.0F, glm::degrees(static_cast<float>(sin(time * 0.662)) * 2.0F));
    RightLeg.GetTransform().SetRotation(0.0F, 0.0F, glm::degrees(static_cast<float>(sin(time * 0.6662 + glm::pi<double>())) * 1.4F));
    LeftLeg.GetTransform().SetRotation(0.0F, 0.0F, glm::degrees(static_cast<float>(sin(time * 0.6662)) * 1.4F));
}

EEntityType Zombie::GetEntityType() const
{
    return EEntityType::Zombie;
}
*/