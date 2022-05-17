//
// Created by amit on 4/22/2022.
//

#include "BlockTypeList.h"
#include "../../Util/EngineDefaults.h"
#include "AirBlock.h"
#include "CobblestoneBlock.h"
#include "GrassBlock.h"

using std::piecewise_construct;
using std::forward_as_tuple;

std::istream& operator>>(std::istream& is, EBlockType& blockType)
{
    uint8_t temp = 0;
    is >> temp;
    blockType = static_cast<EBlockType>(temp);
    return is;
}

const Block* BlockTypeList::GetBlockTypeData(const EBlockType type)
{
    InitBlockTypes();
    return BlockTypes.at(type).get();
}

uint16_t BlockTypeList::RegisterTexture(Texture* texture)
{
    TextureList.push_back(texture);
    texture->Resident();
    return static_cast<uint16_t>(TextureList.size() - 1);
}

void BlockTypeList::InitBlockTypes()
{
    if (Init)
    {
        return;
    }
    glGenBuffers(1, &Ubo);
    Init = true;
    for (const Texture* texture : TextureList)
    {
        texture->NonResident();
    }
    TextureList.clear();
    BlockTypes.clear();
    BlockTypes.emplace(piecewise_construct, forward_as_tuple(EBlockType::Air), forward_as_tuple(new AirBlock()));
    BlockTypes.emplace(piecewise_construct, forward_as_tuple(EBlockType::Grass), forward_as_tuple(new GrassBlock()));
    BlockTypes.emplace(piecewise_construct, forward_as_tuple(EBlockType::Cobblestone), forward_as_tuple(new CobblestoneBlock()));
    vector<GLuint64> helper;
    helper.reserve(TextureList.size());
    for (const Texture* texture : TextureList)
    {
        helper.push_back(texture->GetHandle());
    }
    glBindBuffer(GL_UNIFORM_BUFFER, Ubo);
    glBufferData(GL_UNIFORM_BUFFER, static_cast<GLintptr>(sizeof(GLuint64) * helper.size()), helper.data(), GL_STATIC_DRAW);
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, Ubo, 0, static_cast<GLintptr>(sizeof(GLuint64) * helper.size()));
}

void BlockTypeList::ResetBlockTypes()
{
    Init = false;
    glDeleteBuffers(1, &Ubo);
    InitBlockTypes();
}

bool BlockTypeList::Init = false;

unordered_map<EBlockType, unique_ptr<Block>> BlockTypeList::BlockTypes{};
std::vector<Texture*> BlockTypeList::TextureList{};
GLuint BlockTypeList::Ubo = 0;
