#pragma once
#include "States/ChunkState.h"
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
    CustomFileManager& operator<<(const ChunkState& state);
    CustomFileManager& operator>>(int& number);
    CustomFileManager& operator>>(uint8_t& blockType);
    CustomFileManager& operator>>(EBlockType& blockType);
    CustomFileManager& operator>>(std::size_t& size);
    CustomFileManager& operator>>(ChunkState& state);
};
