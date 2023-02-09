/*#include "ParticleEntity.h"

#include "Util/States/PlayerState.h"
#include "World/Generic/Blocks/BlockClient.h"
#include "World/Generic/World.h"

void ParticleEntity::GenerateTextureTessellation(const int lightLevel)
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

ParticleEntity::ParticleEntity(const float x, const float y, const float z, float xSpeed, float ySpeed, float zSpeed, const Block* blockType) : Entity(PARTICLE_SIZE * (TheWorld->RandomEngineState.GetNextFloat() * 0.5F + 0.5F), x, y, z, new EntityState(TheWorld->RegisterEntity(this))),
    U0(TheWorld->RandomEngineState.GetNextFloat() * 0.75F), V0(TheWorld->RandomEngineState.GetNextFloat() * 0.75F), BlockType(blockType), PreviousLightLevel(TheWorld->GetBrightnessAt(this->GetTransform().Position)), LifeTime(static_cast<int>(4.0F / (TheWorld->RandomEngineState.GetNextFloat() * 0.9F + 0.1F))), Age(0)
{
    xSpeed = xSpeed + (TheWorld->RandomEngineState.GetNextFloat() * 2.0F - 1.0F) * 0.4F;
    ySpeed = ySpeed + (TheWorld->RandomEngineState.GetNextFloat() * 2.0F - 1.0F) * 0.4F;
    zSpeed = zSpeed + (TheWorld->RandomEngineState.GetNextFloat() * 2.0F - 1.0F) * 0.4F;
    const float f = (TheWorld->RandomEngineState.GetNextFloat() + TheWorld->RandomEngineState.GetNextFloat() + 1.0F) * 0.15F;
    const float distance = length(glm::vec3(xSpeed, ySpeed, zSpeed));
    this->State->EntityVelocity.x = xSpeed / distance * f * 0.7F;
    this->State->EntityVelocity.y = ySpeed / distance * f;
    this->State->EntityVelocity.z = zSpeed / distance * f * 0.7F;
    GenerateTextureTessellation(PreviousLightLevel);
}

void ParticleEntity::Tick()
{
    Entity::Tick();
    if (Age++ >= LifeTime)
    {
        TheWorld->RemoveEntity(this->GetEntityId());
    }
    State->EntityVelocity.y -= 0.06F;
    this->CheckCollisionAndMove();
    State->EntityVelocity.x *= 0.98F;
    State->EntityVelocity.y *= 0.98F;
    State->EntityVelocity.z *= 0.98F;
    if (this->State->IsGrounded)
    {
        this->State->EntityVelocity.x *= 0.7F;
        this->State->EntityVelocity.z *= 0.7F;
    }
}

void ParticleEntity::Render(const float partialTick)
{
    Entity::Render(partialTick);
    const glm::vec3 pos = this->GetTransform().Position;
    this->GetTransform().Position = this->PrevTransform.Position + (pos - this->PrevTransform.Position) * partialTick;
    const auto playerRot = glm::vec3(TheWorld->GetPlayer()->GetCameraPitch(), TheWorld->GetPlayer()->GetTransform().Rotation.y, 0.0F);
    this->GetTransform().Rotation = glm::vec3(playerRot.x, playerRot.y + 90.0F, playerRot.z);
    if (const int brightness = TheWorld->GetBrightnessAt(pos); PreviousLightLevel != brightness)
    {
        GenerateTextureTessellation(brightness);
    }
    this->Tessellation.Draw();
    this->Tessellation.FreeMemory();
    this->GetTransform().Position = pos;
}*/