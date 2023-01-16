#pragma once
#include "ChunkCoords.h"
#include "World/Generic/Blocks/BlockTypeList.h"
#include "Zlib/zlib.h"

class CustomFileManager
{
    gzFile FileStream;

public:
    explicit CustomFileManager(const std::string& file, const std::string& mode);
    ~CustomFileManager();
    CustomFileManager(const CustomFileManager&) = delete;
    CustomFileManager& operator=(const CustomFileManager&) = delete;
    CustomFileManager(CustomFileManager&&) = delete;
    CustomFileManager& operator=(CustomFileManager&&) = delete;
    CustomFileManager& operator<<(const int& number);
    CustomFileManager& operator<<(const uint8_t& blockType);
    CustomFileManager& operator<<(const EBlockType& blockType);
    CustomFileManager& operator<<(const std::size_t& size);
    CustomFileManager& operator>>(int& number);
    CustomFileManager& operator>>(uint8_t& blockType);
    CustomFileManager& operator>>(EBlockType& blockType);
    CustomFileManager& operator>>(std::size_t& size);
    template <typename T>
    CustomFileManager& operator<<(const ChunkCoords<T>& coords);
    template <typename T>
    CustomFileManager& operator>>(ChunkCoords<T>& coords);
    template <class T>
    CustomFileManager& operator<<(Chunk<T>& chunk);
    template <class T>
    CustomFileManager& operator>>(Chunk<T>& chunk);
};

template <typename T>
CustomFileManager& CustomFileManager::operator<<(const ChunkCoords<T>& coords)
{
    *this << coords.X << coords.Y << coords.Z;
    return *this;
}

template <typename T>
CustomFileManager& CustomFileManager::operator>>(ChunkCoords<T>& coords)
{
    *this >> coords.X >> coords.Y >> coords.Z;
    return *this;
}

template <typename T>
CustomFileManager& CustomFileManager::operator<<(Chunk<T>& chunk)
{
    *this << chunk.GetChunkPos();
    for (const auto& block : chunk.GetBlocks())
    {
        *this << block;
    }
    return *this;
}

template <typename T>
CustomFileManager& CustomFileManager::operator>>(Chunk<T>& chunk)
{
    for (auto& block : chunk.GetBlocks())
    {
        *this >> block;
    }
    return *this;
}
