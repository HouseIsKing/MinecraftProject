#include "Zombie.h"

#include "Util/EngineDefaults.h"
#include <chrono>
#include <glm/gtx/string_cast.hpp>

Zombie::Zombie(const uint16_t entityId, const float x, const float y, const float z) :
    LivingEntity(entityId, ZOMBIE_SIZE, x, y, z),
    TimeOffset(static_cast<float>(EngineDefaults::GetNext<int>(0, 1239813))),
    NextRotation(static_cast<float>(EngineDefaults::GetNext<int>(1, 2)) * 0.01F),
    Head("Textures/Entities/zombie.png", 0.0F, 24 * ZOMBIE_SCALE_FACTOR - ZOMBIE_SIZE.y, 0.0F, -4 * ZOMBIE_SCALE_FACTOR, 0,
         -4 * ZOMBIE_SCALE_FACTOR, 4 * ZOMBIE_SCALE_FACTOR, 8 * ZOMBIE_SCALE_FACTOR, 4 * ZOMBIE_SCALE_FACTOR,
         {
             0.125F, 0.75F, 0.25F, 1.0F, 0.25F, 0.75F, 0.375F, 1.0F, 0.375F, 0.5F, 0.5F, 0.75F, 0.125F, 0.5F, 0.25F,
             0.75F, 0.25F, 0.5F, 0.375F, 0.75F, 0.0F, 0.5F, 0.125F, 0.75F
         }, Tessellation),
    Body("Textures/Entities/zombie.png", 0.0F, 24 * ZOMBIE_SCALE_FACTOR - ZOMBIE_SIZE.y, 0.0F, -2 * ZOMBIE_SCALE_FACTOR,
         -12 * ZOMBIE_SCALE_FACTOR, -4 * ZOMBIE_SCALE_FACTOR, 2 * ZOMBIE_SCALE_FACTOR, 0, 4 * ZOMBIE_SCALE_FACTOR,
         {
             0.3125F, 0.375F, 0.4375F, 0.5F, 0.4375F, 0.375F, 0.5625F, 0.5F, 0.5F, 0.0F, 0.625F, 0.375F, 0.3125F, 0.0F,
             0.4375F, 0.375F, 0.4375F, 0.0F, 0.5F, 0.375F, 0.25F, 0.0F, 0.3125F, 0.375F
         }, Tessellation),
    RightArm("Textures/Entities/zombie.png", 0.0F, 22 * ZOMBIE_SCALE_FACTOR - ZOMBIE_SIZE.y, -5 * ZOMBIE_SCALE_FACTOR,
             -2 * ZOMBIE_SCALE_FACTOR, -10 * ZOMBIE_SCALE_FACTOR, -3 * ZOMBIE_SCALE_FACTOR, 2 * ZOMBIE_SCALE_FACTOR,
             2 * ZOMBIE_SCALE_FACTOR, ZOMBIE_SCALE_FACTOR,
             {
                 0.6875F, 0.375F, 0.75F, 0.5F, 0.75F, 0.375F, 0.8125F, 0.5F, 0.8125F, 0.0F, 0.875F, 0.375F, 0.6875F,
                 0.0F, 0.75F, 0.375F, 0.75F, 0.0F, 0.8125F, 0.375F, 0.625F, 0.0F, 0.6875F, 0.375F
             }, Tessellation),
    LeftArm("Textures/Entities/zombie.png", 0.0F, 22 * ZOMBIE_SCALE_FACTOR - ZOMBIE_SIZE.y, 5 * ZOMBIE_SCALE_FACTOR, -2 * ZOMBIE_SCALE_FACTOR,
            -10 * ZOMBIE_SCALE_FACTOR, -ZOMBIE_SCALE_FACTOR, 2 * ZOMBIE_SCALE_FACTOR, 2 * ZOMBIE_SCALE_FACTOR,
            3 * ZOMBIE_SCALE_FACTOR,
            {
                0.6875F, 0.375F, 0.75F, 0.5F, 0.75F, 0.375F, 0.8125F, 0.5F, 0.8125F, 0.0F, 0.875F, 0.375F, 0.6875F,
                0.0F, 0.75F, 0.375F, 0.75F, 0.0F, 0.8125F, 0.375F, 0.625F, 0.0F, 0.6875F, 0.375F
            }, Tessellation),
    RightLeg("Textures/Entities/zombie.png", 0.0F, 12 * ZOMBIE_SCALE_FACTOR - ZOMBIE_SIZE.y, -2 * ZOMBIE_SCALE_FACTOR,
             -2 * ZOMBIE_SCALE_FACTOR, -12 * ZOMBIE_SCALE_FACTOR, -2 * ZOMBIE_SCALE_FACTOR,
             2 * ZOMBIE_SCALE_FACTOR, 0,
             2 * ZOMBIE_SCALE_FACTOR,
             {
                 0.0625F, 0.375F, 0.125F, 0.5F, 0.125F, 0.375F, 0.1875F, 0.5F, 0.1875F, 0.0F, 0.25F, 0.375F,
                 0.0625F,
                 0.0F, 0.125F, 0.375F, 0.125F, 0.0F, 0.1875F, 0.375F, 0.0F, 0.0F, 0.0625F, 0.375F
             }, Tessellation),
    LeftLeg("Textures/Entities/zombie.png", 0.0F, 12 * ZOMBIE_SCALE_FACTOR - ZOMBIE_SIZE.y, 2 * ZOMBIE_SCALE_FACTOR,
            -2 * ZOMBIE_SCALE_FACTOR, -12 * ZOMBIE_SCALE_FACTOR, -2 * ZOMBIE_SCALE_FACTOR, 2 * ZOMBIE_SCALE_FACTOR, 0,
            2 * ZOMBIE_SCALE_FACTOR,
            {
                0.0625F, 0.375F, 0.125F, 0.5F, 0.125F, 0.375F, 0.1875F, 0.5F, 0.1875F, 0.0F, 0.25F, 0.375F, 0.0625F,
                0.0F, 0.125F, 0.375F, 0.125F, 0.0F, 0.1875F, 0.375F, 0.0F, 0.0F, 0.0625F, 0.375F
            }, Tessellation)
{
    Head.PrepareRender(Tessellation);
    Head.GenerateTessellationData(Tessellation);
    Body.PrepareRender(Tessellation);
    Body.GenerateTessellationData(Tessellation);
    RightArm.PrepareRender(Tessellation);
    RightArm.GenerateTessellationData(Tessellation);
    LeftArm.PrepareRender(Tessellation);
    LeftArm.GenerateTessellationData(Tessellation);
    RightLeg.PrepareRender(Tessellation);
    RightLeg.GenerateTessellationData(Tessellation);
    LeftLeg.PrepareRender(Tessellation);
    LeftLeg.GenerateTessellationData(Tessellation);
}

void Zombie::Render()
{
    LivingEntity::Render();
    const double time = static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count()) / 1.0E9 * 10.0 + static_cast<double>(TimeOffset);
    Head.SetRotationRadians(0.0F, static_cast<float>(sin(time * 0.83)), static_cast<float>(sin(time)) * 0.8F, Tessellation);
    Tessellation.Draw(Head.GetBlockTransformId(), Head.GetTrianglesStartPos(), 36);
    Tessellation.Draw(Body.GetBlockTransformId(), Body.GetTrianglesStartPos(), 36);
    RightArm.SetRotationRadians(static_cast<float>(sin(time * 0.2312) + 1.0), 0.0F, static_cast<float>(sin(time * 0.662 + glm::pi<double>())) * 2.0F, Tessellation);
    Tessellation.Draw(RightArm.GetBlockTransformId(), RightArm.GetTrianglesStartPos(), 36);
    LeftArm.SetRotationRadians(static_cast<float>(sin(time * 0.2312) - 1.0), 0.0F, static_cast<float>(sin(time * 0.662)) * 2.0F, Tessellation);
    Tessellation.Draw(LeftArm.GetBlockTransformId(), LeftArm.GetTrianglesStartPos(), 36);
    RightLeg.SetRotationRadians(0.0F, 0.0F, static_cast<float>(sin(time * 0.6662 + glm::pi<double>())) * 1.4F, Tessellation);
    Tessellation.Draw(RightLeg.GetBlockTransformId(), RightLeg.GetTrianglesStartPos(), 36);
    LeftLeg.SetRotationRadians(0.0F, 0.0F, static_cast<float>(sin(time * 0.6662)) * 1.4F, Tessellation);
    Tessellation.Draw(LeftLeg.GetBlockTransformId(), LeftLeg.GetTrianglesStartPos(), 36);
    //Tessellation.Draw();
}

void Zombie::Tick()
{
//    GetTransform().Rotate(0, glm::degrees(NextRotation), 0);
//    NextRotation *= 0.99F;
//    NextRotation += (EngineDefaults::GetNextFloat() - EngineDefaults::GetNextFloat()) * EngineDefaults::GetNextFloat() * EngineDefaults::GetNextFloat() * 0.01F;
//    VerticalInput = 1;
//    JumpRequested = EngineDefaults::GetNextFloat() < 0.01F;
    LivingEntity::Tick();
}
