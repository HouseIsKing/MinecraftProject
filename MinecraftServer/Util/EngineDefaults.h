#pragma once
#include <glm/vec2.hpp>
#include <vector>

class EngineDefaults
{
public:
    static constexpr char CHUNK_WIDTH = 16;
    static constexpr char CHUNK_HEIGHT = 16;
    static constexpr char CHUNK_DEPTH = 16;
    static constexpr float TICK_RATE = 0.5F;
    static constexpr size_t ROLLBACK_COUNT = 1 * static_cast<size_t>(1.0F / TICK_RATE);

    static float ConvertLightLevelToAmbient(int lightLevel);
    static int GetChunkLocalIndex(int x, int y, int z);
    static void EmplaceDataInVector(std::vector<uint8_t>& vector, const uint8_t* data, size_t size);
};

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
