#include "EngineDefaults.h"

#include <GLFW/glfw3.h>
#include <glm/trigonometric.hpp>

float EngineDefaults::ConvertLightLevelToAmbient(const int lightLevel)
{
    if (lightLevel == 0)
    {
        return 0.6F;
    }
    return 1.0F;
}

uint16_t EngineDefaults::GetChunkLocalIndex(int x, int y, int z)
{
    x %= CHUNK_WIDTH;
    y %= CHUNK_HEIGHT;
    z %= CHUNK_DEPTH;
    if (x < 0)
    {
        x += CHUNK_WIDTH;
    }
    if (y < 0)
    {
        y += CHUNK_HEIGHT;
    }
    if (z < 0)
    {
        z += CHUNK_DEPTH;
    }
    return static_cast<uint16_t>(x + z * CHUNK_WIDTH + y * CHUNK_WIDTH * CHUNK_DEPTH);
}

void ClientInputStatusStruct::SetKey(const EKeySet key, const bool pressed)
{
    switch(key)
    {
    case EKeySet::LeftMouseButton:
        KeySet1 = (KeySet1 & 0xFE) + static_cast<uint8_t>(pressed);
        break;
    case EKeySet::RightMouseButton:
        KeySet1 = (KeySet1 & 0xFD) + static_cast<uint8_t>(static_cast<uint8_t>(pressed) << 1);
        break;
    case EKeySet::Jump:
        KeySet1 = (KeySet1 & 0xFB) + static_cast<uint8_t>(static_cast<uint8_t>(pressed) << 2);
        break;
    case EKeySet::Reset:
        KeySet1 = (KeySet1 & 0xF7) + static_cast<uint8_t>(static_cast<uint8_t>(pressed) << 3);
        break;
    case EKeySet::SpawnZombie:
        KeySet1 = (KeySet1 & 0xEF) + static_cast<uint8_t>(static_cast<uint8_t>(pressed) << 4);
        break;
    case EKeySet::One:
        KeySet1 = (KeySet1 & 0xDF) + static_cast<uint8_t>(static_cast<uint8_t>(pressed) << 5);
        break;
    case EKeySet::Two:
        KeySet1 = (KeySet1 & 0xBF) + static_cast<uint8_t>(static_cast<uint8_t>(pressed) << 6);
        break;
    case EKeySet::Three:
        KeySet1 = (KeySet1 & 0x7F) + static_cast<uint8_t>(static_cast<uint8_t>(pressed) << 7);
        break;
    case EKeySet::Four:
        KeySet2 = (KeySet2 & 0xFE) + static_cast<uint8_t>(pressed);
        break;
    case EKeySet::Five:
        KeySet2 = (KeySet2 & 0xFD) + static_cast<uint8_t>(static_cast<uint8_t>(pressed) << 1);
        break;
    case EKeySet::Up:
        KeySet2 = (KeySet2 & 0xFB) + static_cast<uint8_t>(static_cast<uint8_t>(pressed) << 2);
        break;
    case EKeySet::Down:
        KeySet2 = (KeySet2 & 0xF7) + static_cast<uint8_t>(static_cast<uint8_t>(pressed) << 3);
        break;
    case EKeySet::Left:
        KeySet2 = (KeySet2 & 0xEF) + static_cast<uint8_t>(static_cast<uint8_t>(pressed) << 4);
        break;
    case EKeySet::Right:
        KeySet2 = (KeySet2 & 0xDF) + static_cast<uint8_t>(static_cast<uint8_t>(pressed) << 5);
        break;
    }
}

bool ClientInputStruct::IsKeyPressed(const EKeySet key) const
{
    switch (key)
    {
    case EKeySet::LeftMouseButton:
        return static_cast<bool>(KeySet1Pressed & 0x1);
    case EKeySet::RightMouseButton:
        return static_cast<bool>((KeySet1Pressed & 0x2) >> 1);
    case EKeySet::Jump:
        return static_cast<bool>((KeySet1Pressed & 0x4) >> 2);
    case EKeySet::Reset:
        return static_cast<bool>((KeySet1Pressed & 0x8) >> 3);
    case EKeySet::SpawnZombie:
        return static_cast<bool>((KeySet1Pressed & 0x10) >> 4);
    case EKeySet::One:
        return static_cast<bool>((KeySet1Pressed & 0x20) >> 5);
    case EKeySet::Two:
        return static_cast<bool>((KeySet1Pressed & 0x40) >> 6);
    case EKeySet::Three:
        return static_cast<bool>((KeySet1Pressed & 0x80) >> 7);
    case EKeySet::Four:
        return static_cast<bool>(KeySet2Pressed & 0x1);
    case EKeySet::Five:
        return static_cast<bool>((KeySet2Pressed & 0x2) >> 1);
    case EKeySet::Up:
        return static_cast<bool>((KeySet2Pressed & 0x4) >> 2);
    case EKeySet::Down:
        return static_cast<bool>((KeySet2Pressed & 0x8) >> 3);
    case EKeySet::Left:
        return static_cast<bool>((KeySet2Pressed & 0x10) >> 4);
    case EKeySet::Right:
        return static_cast<bool>((KeySet2Pressed & 0x20) >> 5);
    }
    return false;
}

bool ClientInputStruct::IsKeyHold(const EKeySet key) const
{
    switch (key)
    {
    case EKeySet::LeftMouseButton:
        return static_cast<bool>(KeySet1Hold & 0x1);
    case EKeySet::RightMouseButton:
        return static_cast<bool>((KeySet1Hold & 0x2) >> 1);
    case EKeySet::Jump:
        return static_cast<bool>((KeySet1Hold & 0x4) >> 2);
    case EKeySet::Reset:
        return static_cast<bool>((KeySet1Hold & 0x8) >> 3);
    case EKeySet::SpawnZombie:
        return static_cast<bool>((KeySet1Hold & 0x10) >> 4);
    case EKeySet::One:
        return static_cast<bool>((KeySet1Hold & 0x20) >> 5);
    case EKeySet::Two:
        return static_cast<bool>((KeySet1Hold & 0x40) >> 6);
    case EKeySet::Three:
        return static_cast<bool>((KeySet1Hold & 0x80) >> 7);
    case EKeySet::Four:
        return static_cast<bool>(KeySet2Hold & 0x1);
    case EKeySet::Five:
        return static_cast<bool>((KeySet2Hold & 0x2) >> 1);
    case EKeySet::Up:
        return static_cast<bool>((KeySet2Hold & 0x4) >> 2);
    case EKeySet::Down:
        return static_cast<bool>((KeySet2Hold & 0x8) >> 3);
    case EKeySet::Left:
        return static_cast<bool>((KeySet2Hold & 0x10) >> 4);
    case EKeySet::Right:
        return static_cast<bool>((KeySet2Hold & 0x20) >> 5);
    }
    return false;
}

ClientInputStruct& ClientInputStruct::operator<<(const ClientInputStatusStruct& clientInputStatusStruct)
{
    MouseX = clientInputStatusStruct.MouseX;
    MouseY = clientInputStatusStruct.MouseY;
    const uint8_t prevKeySet1Hold = KeySet1Hold;
    const uint8_t prevKeySet2Hold = KeySet2Hold;
    KeySet1Hold = clientInputStatusStruct.KeySet1;
    KeySet2Hold = clientInputStatusStruct.KeySet2;
    KeySet1Pressed = KeySet1Hold & ~prevKeySet1Hold;
    KeySet2Pressed = KeySet2Hold & ~prevKeySet2Hold;
    return *this;
}

glm::vec3 TransformStruct::GetForwardVector() const
{
    return {
        glm::cos(glm::radians(Rotation.x)) * glm::cos(glm::radians(Rotation.y)), glm::sin(glm::radians(Rotation.x)), glm::cos(glm::radians(Rotation.x)) * glm::sin(glm::radians(Rotation.y))
    };
}

int ChunkCoords::GetX() const
{
    return X;
}

int ChunkCoords::GetY() const
{
    return Y;
}

int ChunkCoords::GetZ() const
{
    return Z;
}

bool ChunkCoords::operator<(const ChunkCoords& chunkCoords) const
{
    if (X < chunkCoords.X)
    {
        return true;
    }
    if (X > chunkCoords.X)
    {
        return false;
    }
    if (Y < chunkCoords.Y)
    {
        return true;
    }
    if (Y > chunkCoords.Y)
    {
        return false;
    }
    return Z < chunkCoords.Z;
}

ChunkCoords::ChunkCoords(const int x, const int y, const int z) : X(x / EngineDefaults::CHUNK_WIDTH), Y(y / EngineDefaults::CHUNK_HEIGHT), Z(z / EngineDefaults::CHUNK_DEPTH)
{
    if (x < 0)
    {
        this->X -= 1;
    }
    if (y < 0)
    {
        this->Y -= 1;
    }
    if (z < 0)
    {
        this->Z -= 1;
    }
}

ChunkCoords::ChunkCoords() : ChunkCoords(0, 0, 0)
{
}
