//
// Created by amit on 4/22/2022.
//

#include "EngineDefaults.h"
#include "../World/Chunk.h"
#include <memory>

bool EngineDefaults::HasInit = false;
bool EngineDefaults::HasBuiltTextureUbo = false;
unique_ptr<Shader> EngineDefaults::TheShader = {};
CustomRandomEngine EngineDefaults::Engine = {};
vector<Texture*> EngineDefaults::TextureList = {};
GLuint EngineDefaults::UboTextures = 0;

Shader* EngineDefaults::GetShader()
{
    if (!HasInit)
    {
        Init();
    }
    return TheShader.get();
}

void EngineDefaults::Init()
{
    HasInit = true;
    TheShader = std::make_unique<Shader>("Shaders/VertexShader.glsl", "Shaders/FragmentShader.glsl");
    glGenBuffers(1, &UboTextures);
}

int EngineDefaults::GetChunkLocalIndex(int x, int y, int z)
{
    x %= Chunk::CHUNK_WIDTH;
    y %= Chunk::CHUNK_HEIGHT;
    z %= Chunk::CHUNK_DEPTH;
    return x + z * Chunk::CHUNK_WIDTH + y * Chunk::CHUNK_WIDTH * Chunk::CHUNK_DEPTH;
}

uint16_t EngineDefaults::RegisterTexture(Texture* texture)
{
    HasBuiltTextureUbo = false;
    TextureList.push_back(texture);
    texture->Resident();
    return static_cast<uint16_t>(TextureList.size() - 1);
}

void EngineDefaults::BuildTextureUbo()
{
    if (HasBuiltTextureUbo)
    {
        return;
    }
    HasBuiltTextureUbo = true;
    vector<GLuint64> helper;
    helper.reserve(TextureList.size());
    for (const Texture* texture : TextureList)
    {
        helper.push_back(texture->GetHandle());
    }
    glBindBuffer(GL_UNIFORM_BUFFER, UboTextures);
    glBufferData(GL_UNIFORM_BUFFER, static_cast<GLintptr>(sizeof(GLuint64) * helper.size()), helper.data(), GL_STATIC_DRAW);
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, UboTextures, 0, static_cast<GLintptr>(sizeof(GLuint64) * helper.size()));
}

void EngineDefaults::ResetTextures()
{
    for (const auto& texture : TextureList)
    {
        texture->NonResident();
    }
    TextureList.clear();
    HasBuiltTextureUbo = false;
}
