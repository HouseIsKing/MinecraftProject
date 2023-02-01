#include "CustomFileManager.h"

CustomFileManager::CustomFileManager(const std::string& file, const std::string& mode) : FileStream(gzopen(file.c_str(), mode.c_str()))
{
}

CustomFileManager::~CustomFileManager()
{
    gzclose(FileStream);
}

CustomFileManager& CustomFileManager::operator<<(const int& number)
{
    gzwrite(FileStream, &number, sizeof(int));
    return *this;
}

CustomFileManager& CustomFileManager::operator<<(const uint8_t& blockType)
{
    gzwrite(FileStream, &blockType, sizeof(uint8_t));
    return *this;
}

CustomFileManager& CustomFileManager::operator<<(const EBlockType& blockType)
{
    gzwrite(FileStream, &blockType, sizeof(EBlockType));
    return *this;
}

CustomFileManager& CustomFileManager::operator<<(const std::size_t& size)
{
    gzwrite(FileStream, &size, sizeof(std::size_t));
    return *this;
}

CustomFileManager& CustomFileManager::operator>>(int& number)
{
    gzread(FileStream, &number, sizeof(int));
    return *this;
}

CustomFileManager& CustomFileManager::operator>>(uint8_t& blockType)
{
    gzread(FileStream, &blockType, sizeof(uint8_t));
    return *this;
}

CustomFileManager& CustomFileManager::operator>>(EBlockType& blockType)
{
    gzread(FileStream, &blockType, sizeof(EBlockType));
    return *this;
}

CustomFileManager& CustomFileManager::operator>>(std::size_t& size)
{
    gzread(FileStream, &size, sizeof(std::size_t));
    return *this;
}

CustomFileManager& CustomFileManager::operator<<(const ChunkCoords& coords)
{
    *this << coords.X << coords.Y << coords.Z;
    return *this;
}

CustomFileManager& CustomFileManager::operator>>(ChunkCoords& coords)
{
    *this >> coords.X >> coords.Y >> coords.Z;
    return *this;
}

CustomFileManager& CustomFileManager::operator<<(const Chunk& chunk)
{
    *this << chunk.GetChunkPos();
    for (uint16_t i = 0; i < static_cast<uint16_t>(EngineDefaults::CHUNK_WIDTH * EngineDefaults::CHUNK_HEIGHT * EngineDefaults::CHUNK_DEPTH); i++)
    {
        *this << chunk.GetBlockTypeAt(i);
    }
    return *this;
}

CustomFileManager& CustomFileManager::operator>>(Chunk& chunk)
{
    for (uint16_t i = 0; i < static_cast<uint16_t>(EngineDefaults::CHUNK_WIDTH * EngineDefaults::CHUNK_HEIGHT * EngineDefaults::CHUNK_DEPTH); i++)
    {
        EBlockType blockType;
        *this >> blockType;
        chunk.SetBlockTypeAt(i, blockType);
    }
    return *this;
}

