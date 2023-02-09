#pragma once
#include "EngineDefaults.h"
#include "World/Chunk.h"
#include "Zlib/zlib.h"
#include <string>

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
    CustomFileManager& operator<<(const ChunkCoords& coords);
    CustomFileManager& operator<<(const Chunk& chunk);
    CustomFileManager& operator>>(int& number);
    CustomFileManager& operator>>(uint8_t& blockType);
    CustomFileManager& operator>>(EBlockType& blockType);
    CustomFileManager& operator>>(std::size_t& size);
    CustomFileManager& operator>>(ChunkCoords& coords);
    CustomFileManager& operator>>(Chunk& chunk);
};
