#include "EngineDefaultsClient.h"
#include <ranges>

#include "World/Blocks/BlockTypeList.h"

bool EngineDefaultsClient::HasInit = false;
bool EngineDefaultsClient::HasBuiltTextureUbo = false;
std::unordered_map<Texture*, uint16_t, TextureHasher> EngineDefaultsClient::TextureList = {};
std::array<std::unique_ptr<Shader>, 2> EngineDefaultsClient::Shaders = {};
GLuint EngineDefaultsClient::UboTextures = 0;
FontManager EngineDefaultsClient::MainFont{"Textures/Font/default.png"};

Shader* EngineDefaultsClient::GetShader(const uint8_t index)
{
    if (!HasInit)
    {
        Init();
    }
    Shader* result = Shaders.at(index).get();
    result->Use();
    return result;
}

Shader* EngineDefaultsClient::GetShader()
{
    return GetShader(0);
}

void EngineDefaultsClient::Init()
{
    HasInit = true;
    Shaders[0] = std::make_unique<Shader>("Shaders/VertexShader.glsl", "Shaders/FragmentShader.glsl");
    Shaders[1] = std::make_unique<Shader>("Shaders/GUIVertexShader.glsl", "Shaders/GUIFragmentShader.glsl");
    glGenBuffers(1, &UboTextures);
}

glm::ivec3 EngineDefaultsClient::GetChunkLocalPositionFromIndex(uint16_t index)
{
    const uint16_t x = index % CHUNK_WIDTH;
    const uint16_t z = (index / CHUNK_WIDTH) % CHUNK_DEPTH;
    const uint16_t y = index / (CHUNK_WIDTH * CHUNK_DEPTH);
    return {x, y, z};
}

uint16_t EngineDefaultsClient::RegisterTexture(Texture* texture)
{
    HasBuiltTextureUbo = false;
    if (!TextureList.emplace(texture, static_cast<uint16_t>(TextureList.size())).second)
    {
        return TextureList.at(texture);
    }
    texture->Resident();
    return static_cast<uint16_t>(TextureList.size() - 1);
}

void EngineDefaultsClient::InitTextures()
{
    BlockTypeList::InitBlockTypes();
    RegisterTexture(Texture::LoadTexture("Textures/Entities/zombie.png"));
    BuildTextureUbo();
}

void EngineDefaultsClient::BuildTextureUbo()
{
    if (HasBuiltTextureUbo)
    {
        return;
    }
    HasBuiltTextureUbo = true;
    std::vector<GLuint64> helper;
    helper.reserve(TextureList.size());
    for (const Texture* texture : TextureList | std::ranges::views::keys)
    {
        helper.push_back(texture->GetHandle());
    }
    glBindBuffer(GL_UNIFORM_BUFFER, UboTextures);
    glBufferData(GL_UNIFORM_BUFFER, static_cast<GLintptr>(sizeof(GLuint64) * helper.size()), helper.data(), GL_STATIC_COPY);
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, UboTextures, 0, static_cast<GLintptr>(sizeof(GLuint64) * helper.size()));
}

void EngineDefaultsClient::ResetTextures()
{
    for (const auto& texture : TextureList | std::ranges::views::keys)
    {
        texture->NonResident();
    }
    TextureList.clear();
    HasBuiltTextureUbo = false;
}

const FontManager& EngineDefaultsClient::GetFontManager()
{
    return MainFont;
}
