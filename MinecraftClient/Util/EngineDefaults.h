#pragma once
#include "CustomRandomEngine.h"
#include "FontManager.h"
#include "Shaders/Shader.h"
#include "Textures/Texture.h"
#include "World/Generic/Chunk.h"

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
    static void BuildTextureUbo();

public:
    static float ConvertLightLevelToAmbient(int lightLevel);
    static Shader* GetShader();
    static Shader* GetShader(uint8_t index);
    template <typename T>
    static T GetNext(T maxValue);
    template <typename T>
    static T GetNext(T minValue, T maxValue);
    static float GetNextFloat();
    template <typename T>
    static int GetChunkLocalIndex(int x, int y, int z);
    static uint16_t RegisterTexture(Texture* texture);
    static void InitTextures();
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

template <typename T>
int EngineDefaults::GetChunkLocalIndex(int x, int y, int z)
{
    x %= Chunk<T>::CHUNK_WIDTH;
    y %= Chunk<T>::CHUNK_HEIGHT;
    z %= Chunk<T>::CHUNK_DEPTH;
    return x + z * Chunk<T>::CHUNK_WIDTH + y * Chunk<T>::CHUNK_WIDTH * Chunk<T>::CHUNK_DEPTH;
}
