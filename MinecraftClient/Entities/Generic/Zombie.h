#pragma once
#include "EntityMeshBlock.h"
#include "LivingEntity.h"
#include "Network/Packets/EntityDataPacket.h"
#include "Util/EngineDefaults.h"

template <class WorldType>
class Zombie final : public LivingEntity<WorldType>
{
    constexpr static glm::vec3 ZOMBIE_SIZE = glm::vec3(0.3F, 0.9F, 0.3F);
    constexpr static float ZOMBIE_SCALE_FACTOR = 0.05833334F;
    const float TimeOffset;
    int PreviousLightLevel;
    float NextRotation;
    EntityMeshBlock Head;
    EntityMeshBlock Body;
    EntityMeshBlock RightArm;
    EntityMeshBlock LeftArm;
    EntityMeshBlock RightLeg;
    EntityMeshBlock LeftLeg;
    void RebuildRender(int brightness);

public:
    Zombie(float x, float y, float z);
    explicit Zombie(const EntityDataPacket& packet);
    void Render(float partialTick) override;
    void Tick() override;
    [[nodiscard]] EEntityType GetEntityType() const override;
};

template <class WorldType>
void Zombie<WorldType>::RebuildRender(const int brightness)
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

template <class WorldType>
Zombie<WorldType>::Zombie(float x, float y, float z) : LivingEntity<WorldType>(ZOMBIE_SIZE, x, y, z), TimeOffset(static_cast<float>(EngineDefaults::GetNext<int>(0, 1239813))),
                                                       NextRotation((EngineDefaults::GetNextFloat() + 1.0F) * 0.01F),
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
                                                                }, this->Tessellation),
                                                       LeftLeg("Textures/Entities/zombie.png", 0.0F, 12 * ZOMBIE_SCALE_FACTOR - ZOMBIE_SIZE.y, 2 * ZOMBIE_SCALE_FACTOR,
                                                               -2 * ZOMBIE_SCALE_FACTOR, -12 * ZOMBIE_SCALE_FACTOR, -2 * ZOMBIE_SCALE_FACTOR, 2 * ZOMBIE_SCALE_FACTOR, 0,
                                                               2 * ZOMBIE_SCALE_FACTOR,
                                                               {
                                                                   0.0625F, 0.375F, 0.125F, 0.5F, 0.125F, 0.375F, 0.1875F, 0.5F, 0.1875F, 0.0F, 0.25F, 0.375F, 0.0625F,
                                                                   0.0F, 0.125F, 0.375F, 0.125F, 0.0F, 0.1875F, 0.375F, 0.0F, 0.0F, 0.0625F, 0.375F
                                                               }, this->Tessellation)
{
    this->GetTransform().SetRotation(0.0F, EngineDefaults::GetNextFloat() * 360.0F, 0.0F);
    const glm::vec3 pos = this->GetTransform().GetPosition();
    const int brightness = this->GetWorld()->GetBrightnessAt(static_cast<int>(pos.x), static_cast<int>(pos.y), static_cast<int>(pos.z));
    RebuildRender(brightness);
}

template <class WorldType>
Zombie<WorldType>::Zombie(const EntityDataPacket& packet) : LivingEntity<WorldType>(ZOMBIE_SIZE, packet.GetXPos(), packet.GetYPos(), packet.GetZPos()), TimeOffset(static_cast<float>(EngineDefaults::GetNext<int>(0, 1239813))),
                                                            NextRotation((EngineDefaults::GetNextFloat() + 1.0F) * 0.01F),
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
                                                                     }, this->Tessellation),
                                                            LeftLeg("Textures/Entities/zombie.png", 0.0F, 12 * ZOMBIE_SCALE_FACTOR - ZOMBIE_SIZE.y, 2 * ZOMBIE_SCALE_FACTOR,
                                                                    -2 * ZOMBIE_SCALE_FACTOR, -12 * ZOMBIE_SCALE_FACTOR, -2 * ZOMBIE_SCALE_FACTOR, 2 * ZOMBIE_SCALE_FACTOR, 0,
                                                                    2 * ZOMBIE_SCALE_FACTOR,
                                                                    {
                                                                        0.0625F, 0.375F, 0.125F, 0.5F, 0.125F, 0.375F, 0.1875F, 0.5F, 0.1875F, 0.0F, 0.25F, 0.375F, 0.0625F,
                                                                        0.0F, 0.125F, 0.375F, 0.125F, 0.0F, 0.1875F, 0.375F, 0.0F, 0.0F, 0.0625F, 0.375F
                                                                    }, this->Tessellation)
{
    this->GetTransform().SetRotation(0.0F, EngineDefaults::GetNextFloat() * 360.0F, 0.0F);
    const glm::vec3 pos = this->GetTransform().GetPosition();
    const int brightness = this->GetWorld()->GetBrightnessAt(static_cast<int>(pos.x), static_cast<int>(pos.y), static_cast<int>(pos.z));
    RebuildRender(brightness);
}

template <class WorldType>
void Zombie<WorldType>::Render(float partialTick)
{
    LivingEntity<WorldType>::Render(partialTick);
    const glm::vec3 pos = this->GetTransform().GetPosition();
    const glm::vec3 rotation = this->GetTransform().GetRotation();
    this->GetTransform().SetPosition(this->PrevTransform.GetPosition() + (pos - this->PrevTransform.GetPosition()) * partialTick);
    this->GetTransform().SetRotation(this->PrevTransform.GetRotation() + (rotation - this->PrevTransform.GetRotation()) * partialTick);
    if (const int brightness = this->GetWorld()->GetBrightnessAt(pos); PreviousLightLevel != brightness)
    {
        RebuildRender(brightness);
    }
    glm::vec3 prevRot = Head.GetTransform().GetRotation();
    Head.GetTransform().SetRotation(Head.GetPrevTransform().GetRotation() + (prevRot - Head.GetPrevTransform().GetRotation()) * partialTick);
    this->Tessellation.Draw(Head.GetBlockTransformId(), Head.GetTrianglesStartPos(), 36);
    Head.GetTransform().SetRotation(prevRot);
    prevRot = Body.GetTransform().GetRotation();
    Body.GetTransform().SetRotation(Body.GetPrevTransform().GetRotation() + (prevRot - Body.GetPrevTransform().GetRotation()) * partialTick);
    this->Tessellation.Draw(Body.GetBlockTransformId(), Body.GetTrianglesStartPos(), 36);
    Body.GetTransform().SetRotation(prevRot);
    prevRot = RightArm.GetTransform().GetRotation();
    RightArm.GetTransform().SetRotation(RightArm.GetPrevTransform().GetRotation() + (prevRot - RightArm.GetPrevTransform().GetRotation()) * partialTick);
    this->Tessellation.Draw(RightArm.GetBlockTransformId(), RightArm.GetTrianglesStartPos(), 36);
    RightArm.GetTransform().SetRotation(prevRot);
    prevRot = LeftArm.GetTransform().GetRotation();
    LeftArm.GetTransform().SetRotation(LeftArm.GetPrevTransform().GetRotation() + (prevRot - LeftArm.GetPrevTransform().GetRotation()) * partialTick);
    this->Tessellation.Draw(LeftArm.GetBlockTransformId(), LeftArm.GetTrianglesStartPos(), 36);
    LeftArm.GetTransform().SetRotation(prevRot);
    prevRot = RightLeg.GetTransform().GetRotation();
    RightLeg.GetTransform().SetRotation(RightLeg.GetPrevTransform().GetRotation() + (prevRot - RightLeg.GetPrevTransform().GetRotation()) * partialTick);
    this->Tessellation.Draw(RightLeg.GetBlockTransformId(), RightLeg.GetTrianglesStartPos(), 36);
    RightLeg.GetTransform().SetRotation(prevRot);
    prevRot = LeftLeg.GetTransform().GetRotation();
    LeftLeg.GetTransform().SetRotation(LeftLeg.GetPrevTransform().GetRotation() + (prevRot - LeftLeg.GetPrevTransform().GetRotation()) * partialTick);
    this->Tessellation.Draw(LeftLeg.GetBlockTransformId(), LeftLeg.GetTrianglesStartPos(), 36);
    LeftLeg.GetTransform().SetRotation(prevRot);
    this->Tessellation.FreeMemory();
    this->GetTransform().SetPosition(pos);
    this->GetTransform().SetRotation(rotation);
}

template <class WorldType>
void Zombie<WorldType>::Tick()
{
    LivingEntity<WorldType>::Tick();
    this->GetTransform().Rotate(0, glm::degrees(NextRotation), 0);
    NextRotation *= 0.99F;
    NextRotation += (EngineDefaults::GetNextFloat() - EngineDefaults::GetNextFloat()) * EngineDefaults::GetNextFloat() * EngineDefaults::GetNextFloat() * 0.01F;
    this->VerticalInput = 1;
    this->JumpRequested = EngineDefaults::GetNextFloat() < 0.08F;
    if (this->GetTransform().GetPosition().y < -100.0F)
    {
        this->GetWorld()->RemoveEntity(this->GetEntityId());
    }
    const double time = glfwGetTime() * 10.0 + static_cast<double>(TimeOffset);
    Head.SetPrevTransform();
    Body.SetPrevTransform();
    RightArm.SetPrevTransform();
    LeftArm.SetPrevTransform();
    RightLeg.SetPrevTransform();
    LeftLeg.SetPrevTransform();
    Head.GetTransform().SetRotation(0.0F, glm::degrees(static_cast<float>(sin(time * 0.83))), glm::degrees(static_cast<float>(sin(time)) * 0.8F));
    RightArm.GetTransform().SetRotation(glm::degrees(static_cast<float>(sin(time * 0.2312) + 1.0)), 0.0F, glm::degrees(static_cast<float>(sin(time * 0.662 + glm::pi<double>()))) * 2.0F);
    LeftArm.GetTransform().SetRotation(glm::degrees(static_cast<float>(sin(time * 0.2312) - 1.0)), 0.0F, glm::degrees(static_cast<float>(sin(time * 0.662)) * 2.0F));
    RightLeg.GetTransform().SetRotation(0.0F, 0.0F, glm::degrees(static_cast<float>(sin(time * 0.6662 + glm::pi<double>())) * 1.4F));
    LeftLeg.GetTransform().SetRotation(0.0F, 0.0F, glm::degrees(static_cast<float>(sin(time * 0.6662)) * 1.4F));
}

template <class WorldType>
EEntityType Zombie<WorldType>::GetEntityType() const
{
    return EEntityType::Zombie;
}
