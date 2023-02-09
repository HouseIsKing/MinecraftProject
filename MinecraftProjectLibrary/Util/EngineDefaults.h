#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vector>

class EngineDefaults
{
public:
    static constexpr glm::vec3 PLAYER_SIZE = glm::vec3(0.3F, 0.9F, 0.3F);
    static constexpr float CAMERA_OFFSET = 1.62F;
    static constexpr char CHUNK_WIDTH = 16;
    static constexpr char CHUNK_HEIGHT = 16;
    static constexpr char CHUNK_DEPTH = 16;
    static constexpr float TICK_RATE = 0.05F;
    static constexpr size_t ROLLBACK_COUNT = 1 * static_cast<size_t>(1.0F / TICK_RATE);

    static float ConvertLightLevelToAmbient(int lightLevel);
    static uint16_t GetChunkLocalIndex(int x, int y, int z);
    template <typename T>
    static const T& ReadDataFromVector(const std::vector<uint8_t>& vector, size_t& pos);
    template <class T>
    static void EmplaceReplaceDataInVector(std::vector<uint8_t>& vector, const T* data);
    template <typename T>
    static void EmplaceReplaceDataInVector(std::vector<uint8_t>& vector, const T* data, size_t pos);
};

template <typename T>
const T& EngineDefaults::ReadDataFromVector(const std::vector<uint8_t>& vector, size_t& pos)
{
    const T& result = *reinterpret_cast<const T*>(vector.data() + pos);
    pos += sizeof(T);
    return result;
}

template <typename T>
void EngineDefaults::EmplaceReplaceDataInVector(std::vector<uint8_t>& vector, const T* data)
{
    EmplaceReplaceDataInVector(vector, data, vector.size());
}

template <typename T>
void EngineDefaults::EmplaceReplaceDataInVector(std::vector<uint8_t>& vector, const T* data, const size_t pos)
{
    if (vector.size() == pos)
    {
        vector.insert(vector.end(), reinterpret_cast<const uint8_t*>(data), reinterpret_cast<const uint8_t*>(data) + sizeof(T));
    }
    else
    {
        if (vector.size() < pos + sizeof(T))
        {
            vector.resize(pos + sizeof(T));
        }
        std::memcpy(vector.data() + pos, data, sizeof(T));
    }
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
    PlayerCameraPitch,
    PlayerInputState,
};

enum class EChangeType : uint8_t
{
    WorldTime,
    PlayerEnterWorld,
    PlayerState,
    PlayerLeaveWorld,
    ChunkEnterWorld,
    ChunkState,
    ChunkLeaveWorld,
    LightEnterWorld,
    LightState,
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

struct ClientInputStruct
{
    float MouseX{0.0F};
    float MouseY{0.0F};
    bool LeftMouseButtonPressed{false};
    bool RightMouseButtonPressed{false};
    int8_t ForwardAxis{0};
    int8_t RightAxis{0};
    bool JumpPressed{false};
    bool ResetPressed{false};
    bool SpawnZombiePressed{false};
    bool OnePressed{false};
    bool TwoPressed{false};
    bool ThreePressed{false};
    bool FourPressed{false};
    bool FivePressed{false};
};

struct TransformStruct
{
    glm::vec3 Position{};
    glm::vec3 Rotation{};
    glm::vec3 Scale{};
    [[nodiscard]] glm::vec3 GetForwardVector() const;
};

struct State
{
};

struct ChunkCoords
{
private:
    int X;
    int Y;
    int Z;

public:
    [[nodiscard]] int GetX() const;
    [[nodiscard]] int GetY() const;
    [[nodiscard]] int GetZ() const;
    bool operator<(const ChunkCoords& chunkCoords) const;
    ChunkCoords(int x, int y, int z);
    ChunkCoords();

    constexpr bool operator==(const ChunkCoords& other) const
    {
        return X == other.X && Y == other.Y && Z == other.Z;
    }
};

struct BlockTypeHasher
{
    size_t operator()(const EBlockType& blockType) const noexcept
    {
        return static_cast<size_t>(blockType);
    }
};

struct EntityTypeHasher
{
    size_t operator()(const EEntityType& entityType) const noexcept
    {
        return static_cast<size_t>(entityType);
    }
};

struct ChunkCoordsHasher
{
    size_t operator()(const ChunkCoords& chunkCoords) const noexcept
    {
        const size_t x = chunkCoords.GetX() >= 0 ? static_cast<unsigned long long>(2 * chunkCoords.GetX()) : static_cast<unsigned long long>(-2 * chunkCoords.GetX() - 1);
        const size_t y = chunkCoords.GetY() >= 0 ? static_cast<unsigned long long>(2 * chunkCoords.GetY()) : static_cast<unsigned long long>(-2 * chunkCoords.GetY() - 1);
        const size_t z = chunkCoords.GetZ() >= 0 ? static_cast<unsigned long long>(2 * chunkCoords.GetZ()) : static_cast<unsigned long long>(-2 * chunkCoords.GetZ() - 1);
        const size_t xyPair = x >= y ? x * x + x + y : y * y + x;
        return xyPair >= z ? xyPair * xyPair + xyPair + z : z * z + xyPair;
    }
};
