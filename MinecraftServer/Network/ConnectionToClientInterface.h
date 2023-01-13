#pragma once
#include <memory>

#include "Entities/Player.h"

class ConnectionToClientInterface
{
protected:
    Player RepresentingPlayer;

public:
    explicit ConnectionToClientInterface(std::string name);
    ConnectionToClientInterface() = default;
    virtual ~ConnectionToClientInterface() = default;
    ConnectionToClientInterface(const ConnectionToClientInterface&) = default;
    ConnectionToClientInterface(ConnectionToClientInterface&&) = default;
    ConnectionToClientInterface& operator=(const ConnectionToClientInterface&) = default;
    ConnectionToClientInterface& operator=(ConnectionToClientInterface&&) = default;
    const std::string& GetPlayerName() const;
};

struct ConnectionHasher
{
    std::size_t operator()(const std::shared_ptr<ConnectionToClientInterface>& connection) const
    {
        return std::hash<std::string>{}(connection->GetPlayerName());
    }
};

struct ConnectionEqual
{
    bool operator()(const std::shared_ptr<ConnectionToClientInterface>& lhs, const std::shared_ptr<ConnectionToClientInterface>& rhs) const
    {
        return lhs->GetPlayerName() == rhs->GetPlayerName();
    }
};
