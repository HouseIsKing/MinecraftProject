#pragma once
#include "Entity.h"
#include "Util/BlockFaces.h"
#include "Util/EngineDefaults.h"

class Block;

template <typename T>
class ParticleEntity final : public Entity<T>
{
    static constexpr glm::vec3 PARTICLE_SIZE = glm::vec3(0.1F, 0.1F, 0.1F);
    float U0;
    float V0;
    const Block* BlockType;
    int PreviousLightLevel;
    const int LifeTime;
    int Age;
    void GenerateTextureTessellation(int lightLevel);

public:
    ParticleEntity(float x, float y, float z, float xSpeed, float ySpeed, float zSpeed, const Block* blockType);
    void Tick() override;
    void Render(float partialTick) override;
};

template <typename T>
void ParticleEntity<T>::GenerateTextureTessellation(const int lightLevel)
{
    this->Tessellation.Reset();
    PreviousLightLevel = lightLevel;
    const float ambientColor = EngineDefaults::ConvertLightLevelToAmbient(lightLevel);
    const uint16_t texture = BlockType->GetTextureFromIndex(BlockType->GetIndexTextureSide(BlockFaces::North));
    const glm::vec3 size = this->GetEntitySize();
    const uint16_t vert1 = this->Tessellation.AddVertex(Vertex{-size.x, -size.y, 0.0F, U0, V0, ambientColor, ambientColor, ambientColor, 1.0F, texture, 0.0F, 0.0F, 0.0F, lightLevel});
    const uint16_t vert2 = this->Tessellation.AddVertex(Vertex{size.x, -size.y, 0.0F, U0 + 0.25F, V0, ambientColor, ambientColor, ambientColor, 1.0F, texture, 0.0F, 0.0F, 0.0F, lightLevel});
    const uint16_t vert3 = this->Tessellation.AddVertex(Vertex{size.x, size.y, 0.0F, U0 + 0.25F, V0 + 0.25F, ambientColor, ambientColor, ambientColor, 1.0F, texture, 0.0F, 0.0F, 0.0F, lightLevel});
    const uint16_t vert4 = this->Tessellation.AddVertex(Vertex{-size.x, size.y, 0.0F, U0, V0 + 0.25F, ambientColor, ambientColor, ambientColor, 1.0F, texture, 0.0F, 0.0F, 0.0F, lightLevel});
    this->Tessellation.AddTriangle(vert1);
    this->Tessellation.AddTriangle(vert2);
    this->Tessellation.AddTriangle(vert3);
    this->Tessellation.AddTriangle(vert1);
    this->Tessellation.AddTriangle(vert3);
    this->Tessellation.AddTriangle(vert4);
}

template <typename T>
ParticleEntity<T>::ParticleEntity(const float x, const float y, const float z, float xSpeed, float ySpeed, float zSpeed, const Block* blockType) : Entity<T>(PARTICLE_SIZE * (EngineDefaults::GetNextFloat() * 0.5F + 0.5F), x, y, z),
    U0(EngineDefaults::GetNextFloat() * 0.75F), V0(EngineDefaults::GetNextFloat() * 0.75F), BlockType(blockType), PreviousLightLevel(this->GetWorld()->GetBrightnessAt(this->GetTransform().GetPosition())), LifeTime(static_cast<int>(4.0F / (EngineDefaults::GetNextFloat() * 0.9F + 0.1F))), Age(0)
{
    xSpeed = xSpeed + (EngineDefaults::GetNextFloat() * 2.0F - 1.0F) * 0.4F;
    ySpeed = ySpeed + (EngineDefaults::GetNextFloat() * 2.0F - 1.0F) * 0.4F;
    zSpeed = zSpeed + (EngineDefaults::GetNextFloat() * 2.0F - 1.0F) * 0.4F;
    const float f = (EngineDefaults::GetNextFloat() + EngineDefaults::GetNextFloat() + 1.0F) * 0.15F;
    const float distance = length(glm::vec3(xSpeed, ySpeed, zSpeed));
    this->VelocityX = xSpeed / distance * f * 0.7F;
    this->VelocityY = ySpeed / distance * f;
    this->VelocityZ = zSpeed / distance * f * 0.7F;
    GenerateTextureTessellation(PreviousLightLevel);
}

template <typename T>
void ParticleEntity<T>::Tick()
{
    Entity<T>::Tick();
    if (Age++ >= LifeTime)
    {
        this->GetWorld()->RemoveEntity(this->GetEntityId());
    }
    this->VelocityY -= 0.06F;
    this->CheckCollisionAndMove();
    this->VelocityX *= 0.98F;
    this->VelocityY *= 0.98F;
    this->VelocityZ *= 0.98F;
    if (this->IsOnGround())
    {
        this->VelocityX *= 0.7F;
        this->VelocityZ *= 0.7F;
    }
}

template <typename T>
void ParticleEntity<T>::Render(const float partialTick)
{
    Entity<T>::Render(partialTick);
    const glm::vec3 pos = this->GetTransform().GetPosition();
    this->GetTransform().SetPosition(this->PrevPos + (pos - this->PrevPos) * partialTick);
    const auto playerRot = glm::vec3(this->GetWorld()->GetPlayer()->GetCameraPitch(), Entity<T>::GetWorld()->GetPlayer()->GetTransform().GetRotation().y, 0.0F);
    this->GetTransform().SetRotation(playerRot.x, playerRot.y + 90.0F, playerRot.z);
    if (const int brightness = this->GetWorld()->GetBrightnessAt(pos); PreviousLightLevel != brightness)
    {
        GenerateTextureTessellation(brightness);
    }
    this->Tessellation.Draw();
    this->Tessellation.FreeMemory();
    this->GetTransform().SetPosition(pos);
}
