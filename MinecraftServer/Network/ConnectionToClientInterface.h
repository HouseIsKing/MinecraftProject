#pragma once
#include <memory>
#include <string>

class ConnectionToClientInterface
{
public:
    std::string ClientName;
    ConnectionToClientInterface() = default;
    virtual ~ConnectionToClientInterface() = default;
    ConnectionToClientInterface(const ConnectionToClientInterface&) = default;
    ConnectionToClientInterface(ConnectionToClientInterface&&) = default;
    ConnectionToClientInterface& operator=(const ConnectionToClientInterface&) = default;
    ConnectionToClientInterface& operator=(ConnectionToClientInterface&&) = default;
};

struct ConnectionHasher
{
    std::size_t operator()(const std::shared_ptr<ConnectionToClientInterface>& connection) const
    {
        return std::hash<std::string>{}(connection->ClientName);
    }
};

struct ConnectionEqual
{
    bool operator()(const std::shared_ptr<ConnectionToClientInterface>& lhs, const std::shared_ptr<ConnectionToClientInterface>& rhs) const
    {
        return lhs->ClientName == rhs->ClientName;
    }
};
