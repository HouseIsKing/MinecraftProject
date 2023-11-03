#pragma once
#include <deque>
#include <queue>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vector>
#include "BoundingBox.h"

struct TransformStruct;
struct ClientInputStruct;
struct ChunkCoords;

class EngineDefaults
{
public:
    static constexpr glm::vec3 PLAYER_SIZE = glm::vec3(0.3F, 0.9F, 0.3F);
    static constexpr glm::vec3 PARTICLE_SIZE = glm::vec3(0.1F, 0.1F, 0.1F);
    static constexpr float CAMERA_OFFSET = 1.62F;
    static constexpr char CHUNK_WIDTH = 16;
    static constexpr char CHUNK_HEIGHT = 16;
    static constexpr char CHUNK_DEPTH = 16;
    static constexpr float TICK_RATE = 0.05F;
    static constexpr size_t ROLLBACK_COUNT = 200;
    static float ConvertLightLevelToAmbient(int lightLevel);
    static uint16_t GetChunkLocalIndex(int x, int y, int z);
    template <typename T> requires std::is_fundamental_v<T> or std::is_enum_v<T> or std::is_same_v<glm::vec3, T> or std::is_same_v<glm::vec2, T> or std::is_same_v<ChunkCoords, T> or std::is_same_v<ClientInputStruct, T> or std::is_same_v<TransformStruct, T> or std::is_same_v<glm::ivec2, T>
    static const T& ReadDataFromVector(const std::vector<uint8_t>& vector, size_t& pos);
    template <class T> requires std::is_fundamental_v<T> or std::is_enum_v<T> or std::is_same_v<glm::vec3, T> or std::is_same_v<glm::vec2, T> or std::is_same_v<ChunkCoords, T> or std::is_same_v<ClientInputStruct, T> or std::is_same_v<TransformStruct, T> or std::is_same_v<glm::ivec2, T>
    static void EmplaceReplaceDataInVector(std::vector<uint8_t>& vector, const T* data);
    template <typename T> requires std::is_fundamental_v<T> or std::is_enum_v<T> or std::is_same_v<glm::vec3, T> or std::is_same_v<glm::vec2, T> or std::is_same_v<ChunkCoords, T> or std::is_same_v<ClientInputStruct, T> or std::is_same_v<TransformStruct, T> or std::is_same_v<glm::ivec2, T>
    static void EmplaceReplaceDataInVector(std::vector<uint8_t>& vector, const T* data, size_t pos);
    static glm::ivec3 GetChunkLocalPosition(unsigned short index);
    static glm::ivec3 GetChunkGlobalPosition(const ChunkCoords& chunkCoords, uint16_t index);
    static std::vector<uint8_t> Inflate(const std::vector<uint8_t>& dataToInflate);
    static std::vector<uint8_t> Deflate(const std::vector<uint8_t>& dataToDeflate);
    static BoundingBox GetBoundingBoxFromEntityState(const glm::vec3& pos, const glm::vec3& scale);
};

template <typename T> requires std::is_fundamental_v<T> or std::is_enum_v<T> or std::is_same_v<glm::vec3, T> or std::is_same_v<glm::vec2, T> or std::is_same_v<ChunkCoords, T> or std::is_same_v<ClientInputStruct, T> or std::is_same_v<TransformStruct, T> or std::is_same_v<glm::ivec2, T>
const T& EngineDefaults::ReadDataFromVector(const std::vector<uint8_t>& vector, size_t& pos)
{
    const T& result = *reinterpret_cast<const T*>(vector.data() + pos);
    pos += sizeof(T);
    return result;
}

template <typename T> requires std::is_fundamental_v<T> or std::is_enum_v<T> or std::is_same_v<glm::vec3, T> or std::is_same_v<glm::vec2, T> or std::is_same_v<ChunkCoords, T> or std::is_same_v<ClientInputStruct, T> or std::is_same_v<TransformStruct, T> or std::is_same_v<glm::ivec2, T>
void EngineDefaults::EmplaceReplaceDataInVector(std::vector<uint8_t>& vector, const T* data)
{
    EmplaceReplaceDataInVector(vector, data, vector.size());
}

template <typename T> requires std::is_fundamental_v<T> or std::is_enum_v<T> or std::is_same_v<glm::vec3, T> or std::is_same_v<glm::vec2, T> or std::is_same_v<ChunkCoords, T> or std::is_same_v<ClientInputStruct, T> or std::is_same_v<TransformStruct, T> or std::is_same_v<glm::ivec2, T>
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
    BlockParticleLifeTime,
    BlockParticleAge,
    BlockParticleBlockType,
    PlayerCameraPitch,
    PlayerInputState,
    PlayerMode,
    PlayerCurrentSelectedBlock,
};

enum class EChangeType : uint8_t
{
    WorldTime,
    BlockParticleEnterWorld,
    BlockParticleState,
    BlockParticleLeaveWorld,
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

enum class EKeySet : uint8_t
{
    LeftMouseButton,
    RightMouseButton,
    Jump,
    Reset,
    SpawnZombie,
    One,
    Two,
    Three,
    Four,
    Five,
    Up,
    Down,
    Left,
    Right,
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

struct ClientInputStatusStruct
{
    float MouseX{0.0F};
    float MouseY{0.0F};
    uint8_t KeySet1{0}; // bit 0 = LeftMouseButtonPressed, bit 1 = RightMouseButtonPressed, bit 2 = JumpPressed, bit 3 = ResetPressed, bit 4 = SpawnZombiePressed, bit 5 = OnePressed, bit 6 = TwoPressed, bit 7 = ThreePressed
    uint8_t KeySet2{0}; // bit 0 = FourPressed, bit 1 = FivePressed, bit 2 = ForwardPressed, bit 3 = BackwardPressed, bit 4 = LeftPressed, bit 5 = RightPressed
    uint8_t KeySet3{0};
    uint8_t KeySet4{0};
    void SetKey(EKeySet key, bool pressed);
};

struct ClientInputStruct
{
    float MouseX{0.0F};
    float MouseY{0.0F};
    uint8_t KeySet1Pressed{0}; // bit 0 = LeftMouseButtonPressed, bit 1 = RightMouseButtonPressed, bit 2 = JumpPressed, bit 3 = ResetPressed, bit 4 = SpawnZombiePressed, bit 5 = OnePressed, bit 6 = TwoPressed, bit 7 = ThreePressed
    uint8_t KeySet1Hold{0}; // bit 0 = LeftMouseButtonHold, bit 1 = RightMouseButtonHold, bit 2 = JumpHold, bit 3 = ResetHold, bit 4 = SpawnZombieHold, bit 5 = OneHold, bit 6 = TwoHold, bit 7 = ThreeHold
    uint8_t KeySet2Pressed{0}; // bit 0 = FourPressed, bit 1 = FivePressed, bit 2 = ForwardPressed, bit 3 = BackwardPressed, bit 4 = LeftPressed, bit 5 = RightPressed
    uint8_t KeySet2Hold{0}; // bit 0 = FourHold, bit 1 = FiveHold, bit 2 = ForwardHold, bit 3 = BackwardHold, bit 4 = LeftHold, bit 5 = RightHold
    [[nodiscard]] bool IsKeyPressed(EKeySet key) const;
    [[nodiscard]] bool IsKeyHold(EKeySet key) const;
    ClientInputStruct& operator<<(const ClientInputStatusStruct& clientInputStatusStruct);
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
    State() = default;
    virtual ~State() = default;
    State(const State&) = default;
    State& operator=(const State&) = default;
    State(State&&) = default;
    State& operator=(State&&) = default;
    virtual void Serialize(std::vector<uint8_t>& changes) const = 0;
    virtual void Deserialize(const std::vector<uint8_t>& changes, size_t& pos) = 0;
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

template <typename T, int MaxLen, typename Container=std::deque<T>>
class FixedQueue : public std::queue<T, Container>
{
public:
    // ReSharper disable once CppInconsistentNaming
    void push(const T& value)
    {
        if (this->size() == MaxLen)
        {
            this->c.pop_front();
        }
        std::queue<T, Container>::push(value);
    }
};
