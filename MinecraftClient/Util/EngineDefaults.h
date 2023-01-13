#pragma once
#include "../Shaders/Shader.h"
#include "CustomRandomEngine.h"
#include "FontManager.h"
#include "Textures/Texture.h"

class EngineDefaults
{
    static std::array<std::unique_ptr<Shader>, 2> Shaders;
    static std::unordered_map<Texture*, uint16_t, TextureHasher> TextureList;
    static GLuint UboTextures;
    static bool HasBuiltTextureUbo;
    static bool HasInit;
    static void Init();
    static CustomRandomEngine Engine;
    static FontManager MainFont;

public:
    static float ConvertLightLevelToAmbient(int lightLevel);
    static Shader* GetShader();
    static Shader* GetShader(uint8_t index);
    template <typename T>
    static T GetNext(T maxValue);
    template <typename T>
    static T GetNext(T minValue, T maxValue);
    static float GetNextFloat();
    static int GetChunkLocalIndex(int x, int y, int z);
    static uint16_t RegisterTexture(Texture* texture);
    static void BuildTextureUbo();
    static void ResetTextures();
    static const FontManager& GetFontManager();
};

template <typename T>
T EngineDefaults::GetNext(T maxValue)
{
    return GetNext<T>(0, maxValue);
}

template <typename T>
T EngineDefaults::GetNext(T minValue, T maxValue)
{
    T modulo = maxValue - minValue;
    T next = Engine.GetNext() % modulo;
    return next + minValue;
}
