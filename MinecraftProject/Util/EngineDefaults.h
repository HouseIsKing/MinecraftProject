//
// Created by amit on 4/22/2022.
//
#pragma once
#include "../Shaders/Shader.h"
#include "CustomRandomEngine.h"
#include <array>

#include "Textures/Texture.h"

using std::array;
using std::unique_ptr;

class EngineDefaults
{
    static array<unique_ptr<Shader>, 2> Shaders;
    static std::unordered_map<Texture*, uint16_t, TextureHasher> TextureList;
    static GLuint UboTextures;
    static bool HasBuiltTextureUbo;
    static bool HasInit;
    static void Init();
    static CustomRandomEngine Engine;

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
