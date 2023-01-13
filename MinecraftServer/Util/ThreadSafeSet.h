#pragma once
#include "Network/ConnectionToClientInterface.h"
#include <asio/io_context.hpp>
#include <mutex>
#include <unordered_set>

#include "Network/ConnectionToClient.h"


class ClientNetworkManager;

class ThreadSafeSet
{
    std::unordered_set<std::shared_ptr<ConnectionToClientInterface>, ConnectionHasher, ConnectionEqual> Clients;
    std::mutex SetMutex;

public:
    size_t GetSize();
    std::shared_ptr<ConnectionToClient> Emplace(asio::io_context& context, ClientNetworkManager* networkManager);
    std::shared_ptr<ConnectionToClient> GetConnectionToClient(const std::shared_ptr<ConnectionToClientInterface>& searchClient);
    void Remove(const std::shared_ptr<ConnectionToClientInterface>& clientToRemove);
};
