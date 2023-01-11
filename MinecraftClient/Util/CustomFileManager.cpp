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
