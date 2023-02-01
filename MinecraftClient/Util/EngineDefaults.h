#pragma once
#include "CustomRandomEngine.h"
#include "FontManager.h"
#include "Shaders/Shader.h"
#include "Textures/Texture.h"

class EngineDefaults
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
    static constexpr uint8_t CHUNK_WIDTH = 16;
    static constexpr uint8_t CHUNK_HEIGHT = 16;
    static constexpr uint8_t CHUNK_DEPTH = 16;
    static constexpr float TICK_RATE = 0.5F;
    static constexpr size_t ROLLBACK_COUNT = 1 * static_cast<size_t>(1.0F / TICK_RATE);
    static float ConvertLightLevelToAmbient(int lightLevel);
    static Shader* GetShader();
    static Shader* GetShader(uint8_t index);
    static uint16_t GetChunkLocalIndex(int x, int y, int z);
    static glm::ivec3 GetChunkLocalPositionFromIndex(uint16_t index);
    static uint16_t RegisterTexture(Texture* texture);
    static void InitTextures();
    static void ResetTextures();
    static const FontManager& GetFontManager();
    static void EmplaceDataInVector(std::vector<uint8_t>& vector, const uint8_t* data, size_t size);
};

inline uint16_t EngineDefaults::GetChunkLocalIndex(const int x, const int y, const int z)
{
    const auto convertedX = static_cast<uint16_t>(x % CHUNK_WIDTH);
    const auto convertedY = static_cast<uint16_t>(y % CHUNK_HEIGHT);
    const auto convertedZ = static_cast<uint16_t>(z % CHUNK_DEPTH);
    return convertedX + convertedZ * CHUNK_WIDTH + convertedY * CHUNK_WIDTH * CHUNK_DEPTH;
}

enum class BlockFaces : uint8_t
{
    Top,
    Bottom,
    West,
    East,
    North,
    South
};

enum class DrawType : uint8_t
{
    Default,
    Sapling
};

enum class EBlockType : uint8_t
{
    Air,
    Stone,
    Grass,
    Dirt,
    Cobblestone,
    Planks,
    Sapling
};

enum class EPacketType : uint8_t
{
    PlayerId,
    WorldData,
    ClientInput
};

enum class EChangeTypeEntity : uint8_t
{
    Position,
    Rotation,
    Scale,
    Velocity,
    IsGrounded,
    JumpRequested,
    HorizontalInput,
    VerticalInput,
    PlayerChange,
};

enum class EChangeTypePlayer : uint8_t
{
    CameraPitch,
    MouseX,
    MouseY,
    LeftMouseButton,
    RightMouseButton,
    ForwardAxis,
    RightAxis,
    Jump,
    ResetPos,
    SpawnZombie,
    OnePressed,
    TwoPressed,
    ThreePressed,
    FourPressed,
    FivePressed,
};

enum class EChangeType : uint8_t
{
    WorldTime,
    EntityState,
    EntityEnterWorld,
    EntityLeaveWorld,
    ChunkState,
    ChunkEnterWorld,
    ChunkLeaveWorld,
    LightState,
    LightEnterWorld,
    LightLeaveWorld,
    RandomState,
};

enum class EEntityType : uint8_t
{
    Player,
    BlockBreakParticle,
    Zombie,
};

struct IVec2Comparator
{
    bool operator()(const glm::ivec2& a, const glm::ivec2& b) const
    {
        if (a.x < b.x)
        {
            return true;
        }
        if (a.x == b.x)
        {
            return a.y < b.y;
        }
        return false;
    }
};

