#include "ParticleEntity.h"

#include "Util/EngineDefaults.h"
#include "World/SinglePlayerWorld.h"

void ParticleEntity::GenerateTextureTessellation(const int lightLevel)
{
    Tessellation.Reset();
    PreviousLightLevel = lightLevel;
    const float ambientColor = EngineDefaults::ConvertLightLevelToAmbient(lightLevel);
    const uint16_t texture = BlockType->GetTextureFromIndex(BlockType->GetIndexTextureSide(BlockFaces::North));
    const vec3 size = GetEntitySize();
    const uint16_t vert1 = Tessellation.AddVertex(Vertex{-size.x, -size.y, 0.0F, U0, V0, ambientColor, ambientColor, ambientColor, 1.0F, texture, 0.0F, 0.0F, 0.0F, lightLevel});
    const uint16_t vert2 = Tessellation.AddVertex(Vertex{size.x, -size.y, 0.0F, U0 + 0.25F, V0, ambientColor, ambientColor, ambientColor, 1.0F, texture, 0.0F, 0.0F, 0.0F, lightLevel});
    const uint16_t vert3 = Tessellation.AddVertex(Vertex{size.x, size.y, 0.0F, U0 + 0.25F, V0 + 0.25F, ambientColor, ambientColor, ambientColor, 1.0F, texture, 0.0F, 0.0F, 0.0F, lightLevel});
    const uint16_t vert4 = Tessellation.AddVertex(Vertex{-size.x, size.y, 0.0F, U0, V0 + 0.25F, ambientColor, ambientColor, ambientColor, 1.0F, texture, 0.0F, 0.0F, 0.0F, lightLevel});
    Tessellation.AddTriangle(vert1);
    Tessellation.AddTriangle(vert2);
    Tessellation.AddTriangle(vert3);
    Tessellation.AddTriangle(vert1);
    Tessellation.AddTriangle(vert3);
    Tessellation.AddTriangle(vert4);
}

ParticleEntity::ParticleEntity(const float x, const float y, const float z, float xSpeed, float ySpeed, float zSpeed, const Block* blockType) : Entity(PARTICLE_SIZE * (EngineDefaults::GetNextFloat() * 0.5F + 0.5F), x, y, z),
    U0(EngineDefaults::GetNextFloat() * 0.75F), V0(EngineDefaults::GetNextFloat() * 0.75F), BlockType(blockType), PreviousLightLevel(GetWorld()->GetBrightnessAt(GetTransform().GetPosition())), LifeTime(static_cast<int>(4.0F / (EngineDefaults::GetNextFloat() * 0.9F + 0.1F))), Age(0)
{
    xSpeed = xSpeed + (EngineDefaults::GetNextFloat() * 2.0F - 1.0F) * 0.4F;
    ySpeed = ySpeed + (EngineDefaults::GetNextFloat() * 2.0F - 1.0F) * 0.4F;
    zSpeed = zSpeed + (EngineDefaults::GetNextFloat() * 2.0F - 1.0F) * 0.4F;
    float f = (EngineDefaults::GetNextFloat() + EngineDefaults::GetNextFloat() + 1.0F) * 0.15F;
    float distance = length(vec3(xSpeed, ySpeed, zSpeed));
    VelocityX = xSpeed / distance * f * 0.7F;
    VelocityY = ySpeed / distance * f;
    VelocityZ = zSpeed / distance * f * 0.7F;
    GenerateTextureTessellation(PreviousLightLevel);
}

void ParticleEntity::Tick()
{
    Entity::Tick();
    if (Age++ >= LifeTime)
    {
        GetWorld()->RemoveEntity(GetEntityId());
    }
    VelocityY -= 0.06F;
    CheckCollisionAndMove();
    VelocityX *= 0.98F;
    VelocityY *= 0.98F;
    VelocityZ *= 0.98F;
    if (IsOnGround())
    {
        VelocityX *= 0.7F;
        VelocityZ *= 0.7F;
    }
}

void ParticleEntity::Render(const float partialTick)
{
    Entity::Render(partialTick);
    const vec3 pos = GetTransform().GetPosition();
    GetTransform().SetPosition(PrevPos + (pos - PrevPos) * partialTick);
    const auto playerRot = vec3(GetWorld()->GetPlayer()->GetCameraPitch(), GetWorld()->GetPlayer()->GetTransform().GetRotation().y, 0.0F);
    GetTransform().SetRotation(playerRot.x, playerRot.y + 90.0F, playerRot.z);
    if (const int brightness = GetWorld()->GetBrightnessAt(pos); PreviousLightLevel != brightness)
    {
        GenerateTextureTessellation(brightness);
    }
    Tessellation.Draw();
    Tessellation.FreeMemory();
    GetTransform().SetPosition(pos);
}
