#include "EngineDefaults.h"
#include <ranges>

#include "World/Generic/Blocks/BlockTypeList.h"

bool EngineDefaults::HasInit = false;
bool EngineDefaults::HasBuiltTextureUbo = false;
CustomRandomEngine EngineDefaults::Engine = {};
std::unordered_map<Texture*, uint16_t, TextureHasher> EngineDefaults::TextureList = {};
std::array<std::unique_ptr<Shader>, 2> EngineDefaults::Shaders = {};
GLuint EngineDefaults::UboTextures = 0;
FontManager EngineDefaults::MainFont{"Textures/Font/default.png"};

Shader* EngineDefaults::GetShader(const uint8_t index)
{
    if (!HasInit)
    {
        Init();
    }
    Shader* result = Shaders.at(index).get();
    result->Use();
    return result;
}

Shader* EngineDefaults::GetShader()
{
    return GetShader(0);
}

void EngineDefaults::Init()
{
    HasInit = true;
    Shaders[0] = std::make_unique<Shader>("Shaders/VertexShader.glsl", "Shaders/FragmentShader.glsl");
    Shaders[1] = std::make_unique<Shader>("Shaders/GUIVertexShader.glsl", "Shaders/GUIFragmentShader.glsl");
    glGenBuffers(1, &UboTextures);
}

float EngineDefaults::ConvertLightLevelToAmbient(const int lightLevel)
{
    if (lightLevel == 0)
    {
        return 0.6F;
    }
    return 1.0F;
}

float EngineDefaults::GetNextFloat()
{
    return static_cast<float>(static_cast<double>(CustomRandomEngine::GetNext()) / CustomRandomEngine::M);
}

uint16_t EngineDefaults::RegisterTexture(Texture* texture)
{
    HasBuiltTextureUbo = false;
    if (!TextureList.emplace(texture, static_cast<uint16_t>(TextureList.size())).second)
    {
        return TextureList.at(texture);
    }
    texture->Resident();
    return static_cast<uint16_t>(TextureList.size() - 1);
}

void EngineDefaults::InitTextures()
{
    BlockTypeList::InitBlockTypes();
    RegisterTexture(Texture::LoadTexture("Textures/Entities/zombie.png"));
    BuildTextureUbo();
}

void EngineDefaults::BuildTextureUbo()
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

void EngineDefaults::ResetTextures()
{
    for (const auto& texture : TextureList | std::ranges::views::keys)
    {
        texture->NonResident();
    }
    TextureList.clear();
    HasBuiltTextureUbo = false;
}

const FontManager& EngineDefaults::GetFontManager()
{
    return MainFont;
}
