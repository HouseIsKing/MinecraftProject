#pragma once
#include <string>

class Player
{
    std::string Name;

public:
    explicit Player(std::string name);
    Player() = default;
    const std::string& GetName() const;
};
