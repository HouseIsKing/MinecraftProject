#include "Zombie.h"

#include <chrono>

#include "Util/EngineDefaults.h"

Zombie::Zombie(const uint16_t entityId, const float x, const float y, const float z) :
    LivingEntity(entityId, ZOMBIE_SIZE, x, y, z),
    TimeOffset(static_cast<float>(EngineDefaults::GetNext<int>(0, 1239813))),
    //NextRotation(static_cast<float>(EngineDefaults::GetNext<int>(1, 2)) * 0.01F),
    NextRotation(0),
    Head("Textures/Entities/zombie.png", ZOMBIE_SIZE.x, 24 * ZOMBIE_SCALE_FACTOR, ZOMBIE_SIZE.z, -4 * ZOMBIE_SCALE_FACTOR, 0,
         -4 * ZOMBIE_SCALE_FACTOR, 4 * ZOMBIE_SCALE_FACTOR, 8 * ZOMBIE_SCALE_FACTOR, 4 * ZOMBIE_SCALE_FACTOR,
         {
             0.125F, 0.75F, 0.25F, 1.0F, 0.25F, 0.75F, 0.375F, 1.0F, 0.375F, 0.5F, 0.5F, 0.75F, 0.125F, 0.5F, 0.25F,
             0.75F, 0.25F, 0.5F, 0.375F, 0.75F, 0.0F, 0.5F, 0.125F, 0.75F
         }),
    Body("Textures/Entities/zombie.png", ZOMBIE_SIZE.x, 24 * ZOMBIE_SCALE_FACTOR, ZOMBIE_SIZE.z, -4 * ZOMBIE_SCALE_FACTOR,
         -12 * ZOMBIE_SCALE_FACTOR, -2 * ZOMBIE_SCALE_FACTOR, 4 * ZOMBIE_SCALE_FACTOR, 0, 2 * ZOMBIE_SCALE_FACTOR,
         {
             0.3125F, 0.375F, 0.4375F, 0.5F, 0.4375F, 0.375F, 0.5625F, 0.5F, 0.5F, 0.0F, 0.625F, 0.375F, 0.3125F, 0.0F,
             0.4375F, 0.375F, 0.4375F, 0.0F, 0.5F, 0.375F, 0.25F, 0.0F, 0.3125F, 0.375F
         }),
    RightArm("Textures/Entities/zombie.png", ZOMBIE_SIZE.x + 5 * ZOMBIE_SCALE_FACTOR, 22 * ZOMBIE_SCALE_FACTOR, ZOMBIE_SIZE.z, -ZOMBIE_SCALE_FACTOR,
             -10 * ZOMBIE_SCALE_FACTOR, -2 * ZOMBIE_SCALE_FACTOR, 3 * ZOMBIE_SCALE_FACTOR, 2 * ZOMBIE_SCALE_FACTOR,
             2 * ZOMBIE_SCALE_FACTOR,
             {
                 0.6875F, 0.375F, 0.75F, 0.5F, 0.75F, 0.375F, 0.8125F, 0.5F, 0.8125F, 0.0F, 0.875F, 0.375F, 0.6875F,
                 0.0F, 0.75F, 0.375F, 0.75F, 0.0F, 0.8125F, 0.375F, 0.625F, 0.0F, 0.6875F, 0.375F
             }),
    LeftArm("Textures/Entities/zombie.png", ZOMBIE_SIZE.x + -5 * ZOMBIE_SCALE_FACTOR, 22 * ZOMBIE_SCALE_FACTOR, ZOMBIE_SIZE.z,
            -3 * ZOMBIE_SCALE_FACTOR, -10 * ZOMBIE_SCALE_FACTOR, -2 * ZOMBIE_SCALE_FACTOR, ZOMBIE_SCALE_FACTOR,
            2 * ZOMBIE_SCALE_FACTOR, 2 * ZOMBIE_SCALE_FACTOR,
            {
                0.6875F, 0.375F, 0.75F, 0.5F, 0.75F, 0.375F, 0.8125F, 0.5F, 0.8125F, 0.0F, 0.875F, 0.375F, 0.6875F,
                0.0F, 0.75F, 0.375F, 0.75F, 0.0F, 0.8125F, 0.375F, 0.625F, 0.0F, 0.6875F, 0.375F
            }),
    RightLeg("Textures/Entities/zombie.png", ZOMBIE_SIZE.x + 2 * ZOMBIE_SCALE_FACTOR, 12 * ZOMBIE_SCALE_FACTOR, ZOMBIE_SIZE.z,
             -2 * ZOMBIE_SCALE_FACTOR, -12 * ZOMBIE_SCALE_FACTOR, -2 * ZOMBIE_SCALE_FACTOR, 2 * ZOMBIE_SCALE_FACTOR, 0,
             2 * ZOMBIE_SCALE_FACTOR,
             {
                 0.0625F, 0.375F, 0.125F, 0.5F, 0.125F, 0.375F, 0.1875F, 0.5F, 0.1875F, 0.0F, 0.25F, 0.375F, 0.0625F,
                 0.0F, 0.125F, 0.375F, 0.125F, 0.0F, 0.1875F, 0.375F, 0.0F, 0.0F, 0.0625F, 0.375F
             }),
    LeftLeg("Textures/Entities/zombie.png", ZOMBIE_SIZE.x + -2 * ZOMBIE_SCALE_FACTOR, 12 * ZOMBIE_SCALE_FACTOR, ZOMBIE_SIZE.z,
            -2 * ZOMBIE_SCALE_FACTOR, -12 * ZOMBIE_SCALE_FACTOR, -2 * ZOMBIE_SCALE_FACTOR,
            2 * ZOMBIE_SCALE_FACTOR, 0,
            2 * ZOMBIE_SCALE_FACTOR,
            {
                0.0625F, 0.375F, 0.125F, 0.5F, 0.125F, 0.375F, 0.1875F, 0.5F, 0.1875F, 0.0F, 0.25F, 0.375F,
                0.0625F,
                0.0F, 0.125F, 0.375F, 0.125F, 0.0F, 0.1875F, 0.375F, 0.0F, 0.0F, 0.0625F, 0.375F
            })
{
}

void Zombie::Render()
{
    Tessellation.Reset();
    LivingEntity::Render();
    const double time = static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count()) / 1.0E9 * 10.0 + static_cast<double>(TimeOffset);
    //Head.SetRotationRadians(static_cast<float>(sin(time)) * 0.8F, static_cast<float>(sin(time * 0.83)), 0.0F);
    Head.PrepareRender();
    Head.GenerateTessellationData(Tessellation);
    Body.PrepareRender();
    Body.GenerateTessellationData(Tessellation);
    //RightArm.SetRotationRadians(static_cast<float>(sin(time * 0.662 + glm::pi<double>())) * 2.0F, 0, static_cast<float>(sin(time * 0.2312) + 1.0) * 1.0F);
    RightArm.PrepareRender();
    RightArm.GenerateTessellationData(Tessellation);
    //LeftArm.SetRotationRadians(static_cast<float>(sin(time * 0.662)) * 2.0F, 0, static_cast<float>(sin(time * 0.2312) - 1.0) * 1.0F);
    LeftArm.PrepareRender();
    LeftArm.GenerateTessellationData(Tessellation);
    //RightLeg.SetRotationRadians(static_cast<float>(sin(time * 0.6662 + glm::pi<double>())) * 1.4F, 0, 0);
    RightLeg.PrepareRender();
    RightLeg.GenerateTessellationData(Tessellation);
    //LeftLeg.SetRotationRadians(static_cast<float>(sin(time * 0.6662)) * 1.4F, 0, 0);
    LeftLeg.PrepareRender();
    LeftLeg.GenerateTessellationData(Tessellation);
    Tessellation.Draw();
}

void Zombie::Tick()
{
//    GetTransform().Rotate(0, glm::degrees(NextRotation), 0);
//   NextRotation *= 0.99F;
//    NextRotation += (EngineDefaults::GetNextFloat() - EngineDefaults::GetNextFloat()) * EngineDefaults::GetNextFloat() * EngineDefaults::GetNextFloat() * 0.01F;
//    GetTransform().SetRotation(0, 0, 0);
    //VerticalInput = 1;
//    JumpRequested = EngineDefaults::GetNextFloat() < 0.01F;
//    LivingEntity::Tick();
}
