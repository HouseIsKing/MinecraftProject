#pragma once
#include "FontManager.h"
#include "Shaders/Shader.h"
#include "Textures/Texture.h"
#include "Util/EngineDefaults.h"

class EngineDefaultsClient : public EngineDefaults
{
    static std::array<std::unique_ptr<Shader>, 2> Shaders;
    static std::unordered_map<Texture*, uint16_t, TextureHasher> TextureList;
    static GLuint UboTextures;
    static bool HasBuiltTextureUbo;
    static bool HasInit;
    static void Init();
    static FontManager MainFont;
    static void BuildTextureUbo();

public:
    static constexpr float MOUSE_SENSITIVITY = 0.15F;
    static Shader* GetShader();
    static Shader* GetShader(uint8_t index);
    static glm::ivec3 GetChunkLocalPositionFromIndex(uint16_t index);
    static uint16_t RegisterTexture(Texture* texture);
    static void InitTextures();
    static void ResetTextures();
    static const FontManager& GetFontManager();
};

