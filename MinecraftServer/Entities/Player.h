#pragma once
#include <string>

class Player
{
    std::string Name;

    constexpr bool operator==(const Player& other) const
    {
        return Name == other.Name;
    }

public:
    const std::string& GetName() const;
};

struct PlayerHasher
{
    std::size_t operator()(const Player& player) const
    {
        return std::hash<std::string>{}(player.GetName());
    }
};
