#include "Player.h"

Player::Player(std::string name) : Name(std::move(name))
{
}

const std::string& Player::GetName() const
{
    return Name;
}
