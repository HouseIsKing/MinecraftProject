#include "Zombie.h"

#include "World/Generic/World.h"

void Zombie::RebuildRender(const int brightness)
{
    this->Tessellation.Reset();
    PreviousLightLevel = brightness;
    Head.PrepareRender(this->Tessellation);
    Head.GenerateTessellationData(this->Tessellation, brightness);
    Body.PrepareRender(this->Tessellation);
    Body.GenerateTessellationData(this->Tessellation, brightness);
    RightArm.PrepareRender(this->Tessellation);
    RightArm.GenerateTessellationData(this->Tessellation, brightness);
    LeftArm.PrepareRender(this->Tessellation);
    LeftArm.GenerateTessellationData(this->Tessellation, brightness);
    RightLeg.PrepareRender(this->Tessellation);
    RightLeg.GenerateTessellationData(this->Tessellation, brightness);
    LeftLeg.PrepareRender(this->Tessellation);
    LeftLeg.GenerateTessellationData(this->Tessellation, brightness);
}

Zombie::Zombie(const float x, const float y, const float z) : LivingEntity(ZOMBIE_SIZE, x, y, z, new LivingEntityState(TheWorld->RegisterEntity(this))), TimeOffset(static_cast<float>(this->TheWorld->RandomEngineState.GetNext(1239813))),
                                                              PreviousLightLevel(0),
                                                              NextRotation((TheWorld->RandomEngineState.GetNextFloat() + 1.0F) * 0.01F),
                                                              Head("Textures/Entities/zombie.png", 0.0F, 24 * ZOMBIE_SCALE_FACTOR - ZOMBIE_SIZE.y, 0.0F, -4 * ZOMBIE_SCALE_FACTOR, 0,
                                                                   -4 * ZOMBIE_SCALE_FACTOR, 4 * ZOMBIE_SCALE_FACTOR, 8 * ZOMBIE_SCALE_FACTOR, 4 * ZOMBIE_SCALE_FACTOR,
                                                                   {
                                                                       0.125F, 0.75F, 0.25F, 1.0F, 0.25F, 0.75F, 0.375F, 1.0F, 0.375F, 0.5F, 0.5F, 0.75F, 0.125F, 0.5F, 0.25F,
                                                                       0.75F, 0.25F, 0.5F, 0.375F, 0.75F, 0.0F, 0.5F, 0.125F, 0.75F
                                                                   }, this->Tessellation),
                                                              Body("Textures/Entities/zombie.png", 0.0F, 24 * ZOMBIE_SCALE_FACTOR - ZOMBIE_SIZE.y, 0.0F, -2 * ZOMBIE_SCALE_FACTOR,
                                                                   -12 * ZOMBIE_SCALE_FACTOR, -4 * ZOMBIE_SCALE_FACTOR, 2 * ZOMBIE_SCALE_FACTOR, 0, 4 * ZOMBIE_SCALE_FACTOR,
                                                                   {
                                                                       0.3125F, 0.375F, 0.4375F, 0.5F, 0.4375F, 0.375F, 0.5625F, 0.5F, 0.5F, 0.0F, 0.625F, 0.375F, 0.3125F, 0.0F,
                                                                       0.4375F, 0.375F, 0.4375F, 0.0F, 0.5F, 0.375F, 0.25F, 0.0F, 0.3125F, 0.375F
                                                                   }, this->Tessellation),
                                                              RightArm("Textures/Entities/zombie.png", 0.0F, 22 * ZOMBIE_SCALE_FACTOR - ZOMBIE_SIZE.y, -5 * ZOMBIE_SCALE_FACTOR,
                                                                       -2 * ZOMBIE_SCALE_FACTOR, -10 * ZOMBIE_SCALE_FACTOR, -3 * ZOMBIE_SCALE_FACTOR, 2 * ZOMBIE_SCALE_FACTOR,
                                                                       2 * ZOMBIE_SCALE_FACTOR, ZOMBIE_SCALE_FACTOR,
                                                                       {
                                                                           0.6875F, 0.375F, 0.75F, 0.5F, 0.75F, 0.375F, 0.8125F, 0.5F, 0.8125F, 0.0F, 0.875F, 0.375F, 0.6875F,
                                                                           0.0F, 0.75F, 0.375F, 0.75F, 0.0F, 0.8125F, 0.375F, 0.625F, 0.0F, 0.6875F, 0.375F
                                                                       }, this->Tessellation),
                                                              LeftArm("Textures/Entities/zombie.png", 0.0F, 22 * ZOMBIE_SCALE_FACTOR - ZOMBIE_SIZE.y, 5 * ZOMBIE_SCALE_FACTOR, -2 * ZOMBIE_SCALE_FACTOR,
                                                                      -10 * ZOMBIE_SCALE_FACTOR, -ZOMBIE_SCALE_FACTOR, 2 * ZOMBIE_SCALE_FACTOR, 2 * ZOMBIE_SCALE_FACTOR,
                                                                      3 * ZOMBIE_SCALE_FACTOR,
                                                                      {
                                                                          0.6875F, 0.375F, 0.75F, 0.5F, 0.75F, 0.375F, 0.8125F, 0.5F, 0.8125F, 0.0F, 0.875F, 0.375F, 0.6875F,
                                                                          0.0F, 0.75F, 0.375F, 0.75F, 0.0F, 0.8125F, 0.375F, 0.625F, 0.0F, 0.6875F, 0.375F
                                                                      }, this->Tessellation),
                                                              RightLeg("Textures/Entities/zombie.png", 0.0F, 12 * ZOMBIE_SCALE_FACTOR - ZOMBIE_SIZE.y, -2 * ZOMBIE_SCALE_FACTOR,
                                                                       -2 * ZOMBIE_SCALE_FACTOR, -12 * ZOMBIE_SCALE_FACTOR, -2 * ZOMBIE_SCALE_FACTOR,
                                                                       2 * ZOMBIE_SCALE_FACTOR, 0,
                                                                       2 * ZOMBIE_SCALE_FACTOR,
                                                                       {
                                                                           0.0625F, 0.375F, 0.125F, 0.5F, 0.125F, 0.375F, 0.1875F, 0.5F, 0.1875F, 0.0F, 0.25F, 0.375F,
                                                                           0.0625F,
                                                                           0.0F, 0.125F, 0.375F, 0.125F, 0.0F, 0.1875F, 0.375F, 0.0F, 0.0F, 0.0625F, 0.375F
                                                                       }, this->Tessellation), LeftLeg("Textures/Entities/zombie.png", 0.0F, 12 * ZOMBIE_SCALE_FACTOR - ZOMBIE_SIZE.y, 2 * ZOMBIE_SCALE_FACTOR,
                                                                  -2 * ZOMBIE_SCALE_FACTOR, -12 * ZOMBIE_SCALE_FACTOR, -2 * ZOMBIE_SCALE_FACTOR, 2 * ZOMBIE_SCALE_FACTOR, 0,
                                                                  2 * ZOMBIE_SCALE_FACTOR,
                                                                  {
                                                                      0.0625F, 0.375F, 0.125F, 0.5F, 0.125F, 0.375F, 0.1875F, 0.5F, 0.1875F, 0.0F, 0.25F, 0.375F, 0.0625F,
                                                                      0.0F, 0.125F, 0.375F, 0.125F, 0.0F, 0.1875F, 0.375F, 0.0F, 0.0F, 0.0625F, 0.375F
                                                                  }, this->Tessellation)
{
    this->GetTransform().Rotation.y = TheWorld->RandomEngineState.GetNextFloat() * 360.0F;
    const glm::vec3 pos = this->GetTransform().Position;
    const int brightness = TheWorld->GetBrightnessAt(static_cast<int>(pos.x), static_cast<int>(pos.y), static_cast<int>(pos.z));
    RebuildRender(brightness);
}

void Zombie::Render(const float partialTick)
{
    LivingEntity::Render(partialTick);
    const glm::vec3 pos = this->GetTransform().Position;
    const glm::vec3 rotation = this->GetTransform().Rotation;
    this->GetTransform().Position = this->PrevTransform.Position + (pos - this->PrevTransform.Position) * partialTick;
    this->GetTransform().Rotation = this->PrevTransform.Rotation + (rotation - this->PrevTransform.Rotation) * partialTick;
    if (const int brightness = this->TheWorld->GetBrightnessAt(pos); PreviousLightLevel != brightness)
    {
        RebuildRender(brightness);
    }
    glm::vec3 prevRot = Head.GetTransform().Rotation;
    Head.GetTransform().Rotation += (prevRot - Head.GetPrevTransform().Rotation) * partialTick;
    this->Tessellation.Draw(Head.GetBlockTransformId(), Head.GetTrianglesStartPos(), 36);
    Head.GetTransform().Rotation = prevRot;
    prevRot = Body.GetTransform().Rotation;
    Body.GetTransform().Rotation += (prevRot - Body.GetPrevTransform().Rotation) * partialTick;
    this->Tessellation.Draw(Body.GetBlockTransformId(), Body.GetTrianglesStartPos(), 36);
    Body.GetTransform().Rotation = prevRot;
    prevRot = RightArm.GetTransform().Rotation;
    RightArm.GetTransform().Rotation += (prevRot - RightArm.GetPrevTransform().Rotation) * partialTick;
    this->Tessellation.Draw(RightArm.GetBlockTransformId(), RightArm.GetTrianglesStartPos(), 36);
    RightArm.GetTransform().Rotation = prevRot;
    prevRot = LeftArm.GetTransform().Rotation;
    LeftArm.GetTransform().Rotation += (prevRot - LeftArm.GetPrevTransform().Rotation) * partialTick;
    this->Tessellation.Draw(LeftArm.GetBlockTransformId(), LeftArm.GetTrianglesStartPos(), 36);
    LeftArm.GetTransform().Rotation = prevRot;
    prevRot = RightLeg.GetTransform().Rotation;
    RightLeg.GetTransform().Rotation += (prevRot - RightLeg.GetPrevTransform().Rotation) * partialTick;
    this->Tessellation.Draw(RightLeg.GetBlockTransformId(), RightLeg.GetTrianglesStartPos(), 36);
    RightLeg.GetTransform().Rotation = prevRot;
    prevRot = LeftLeg.GetTransform().Rotation;
    LeftLeg.GetTransform().Rotation += (prevRot - LeftLeg.GetPrevTransform().Rotation) * partialTick;
    this->Tessellation.Draw(LeftLeg.GetBlockTransformId(), LeftLeg.GetTrianglesStartPos(), 36);
    LeftLeg.GetTransform().Rotation = prevRot;
    this->Tessellation.FreeMemory();
    this->GetTransform().Position = pos;
    this->GetTransform().Rotation = rotation;
}

void Zombie::Tick()
{
    LivingEntity::Tick();
    this->GetTransform().Rotation.y += glm::degrees(NextRotation);
    NextRotation *= 0.99F;
    NextRotation += (TheWorld->RandomEngineState.GetNextFloat() - this->TheWorld->RandomEngineState.GetNextFloat()) * this->TheWorld->RandomEngineState.GetNextFloat() * this->TheWorld->RandomEngineState.GetNextFloat() * 0.01F;
    auto* state = reinterpret_cast<LivingEntityState*>(this->State.get());
    state->VerticalInput = 1;
    state->JumpRequested = this->TheWorld->RandomEngineState.GetNextFloat() < 0.08F;
    if (this->GetTransform().Position.y < -100.0F)
    {
        this->TheWorld->RemoveEntity(this->GetEntityId());
    }
    const double time = glfwGetTime() * 10.0 + static_cast<double>(TimeOffset);
    Head.SetPrevTransform();
    Body.SetPrevTransform();
    RightArm.SetPrevTransform();
    LeftArm.SetPrevTransform();
    RightLeg.SetPrevTransform();
    LeftLeg.SetPrevTransform();
    Head.GetTransform().Rotation = glm::vec3(0.0F, glm::degrees(static_cast<float>(sin(time * 0.83))), glm::degrees(static_cast<float>(sin(time)) * 0.8F));
    RightArm.GetTransform().Rotation = glm::vec3(glm::degrees(static_cast<float>(sin(time * 0.2312) + 1.0)), 0.0F, glm::degrees(static_cast<float>(sin(time * 0.662 + glm::pi<double>()))) * 2.0F);
    LeftArm.GetTransform().Rotation = glm::vec3(glm::degrees(static_cast<float>(sin(time * 0.2312) - 1.0)), 0.0F, glm::degrees(static_cast<float>(sin(time * 0.662)) * 2.0F));
    RightLeg.GetTransform().Rotation = glm::vec3(0.0F, 0.0F, glm::degrees(static_cast<float>(sin(time * 0.6662 + glm::pi<double>())) * 1.4F));
    LeftLeg.GetTransform().Rotation = glm::vec3(0.0F, 0.0F, glm::degrees(static_cast<float>(sin(time * 0.6662)) * 1.4F));
}
