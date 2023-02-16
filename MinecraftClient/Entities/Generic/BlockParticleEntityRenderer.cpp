#include "BlockParticleEntityRenderer.h"
#include "World/Generic/Blocks/BlockRenderer.h"
#include "World/Generic/Blocks/BlockRendererList.h"
#include "World/Generic/ClientWorld.h"
#include "World/World.h"

void BlockParticleEntityRenderer::GenerateTextureTessellation(const int lightLevel)
{
    this->Tessellation.Reset();
    PreviousLightLevel = lightLevel;
    const float ambientColor = EngineDefaults::ConvertLightLevelToAmbient(lightLevel);
    const BlockRenderer* renderer = BlockRendererList::GetBlockRenderer(State.BlockParticleType);
    const uint16_t texture = renderer->GetTextureFromIndex(renderer->GetIndexTextureSide(BlockFaces::North));
    const glm::vec3 size = State.EntityTransform.Scale;
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

BlockParticleEntityRenderer::BlockParticleEntityRenderer(const BlockParticleEntityState& state,
                                                         const BlockParticleEntityState& oldState) : EntityRenderer(state, oldState), PreviousLightLevel(World::GetWorld()->GetBrightnessAt(state.EntityTransform.Position))
{
    CustomRandomEngine& randomEngine = ClientWorld::GetWorld()->RenderRandomEngine;
    U0 = randomEngine.GetNextFloat() * 0.75F;
    V0 = randomEngine.GetNextFloat() * 0.75F;
    GenerateTextureTessellation(PreviousLightLevel);
}

void BlockParticleEntityRenderer::Render(const float partialTick)
{
    EntityRenderer::Render(partialTick);
    Transform.Position = OldState.EntityTransform.Position + (State.EntityTransform.Position - OldState.EntityTransform.Position) * partialTick;
    Transform.Rotation = glm::vec3(ClientWorld::GetWorld()->GetPlayer()->GetState().CameraPitch, ClientWorld::GetWorld()->GetPlayer()->GetState().EntityTransform.Rotation.y + 90.0F, 0.0F);
    if (const int brightness = World::GetWorld()->GetBrightnessAt(Transform.Position); PreviousLightLevel != brightness)
    {
        GenerateTextureTessellation(brightness);
    }
    this->Tessellation.Draw();
    this->Tessellation.FreeMemory();
}