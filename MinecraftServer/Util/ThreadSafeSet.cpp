#include "ThreadSafeSet.h"

#include "Network/ConnectionToClient.h"

size_t ThreadSafeSet::GetSize()
{
    std::lock_guard<std::mutex> lock(SetMutex);
    return Clients.size();
}

std::shared_ptr<ConnectionToClient> ThreadSafeSet::Emplace(asio::io_context& context,
                                                           ServerNetworkManager* networkManager)
{
    std::lock_guard<std::mutex> lock(SetMutex);
    return std::dynamic_pointer_cast<ConnectionToClient>(Clients.emplace(new ConnectionToClient(context, networkManager)).first._Ptr->_Myval);
}

std::shared_ptr<ConnectionToClient> ThreadSafeSet::GetConnectionToClient(const std::shared_ptr<ConnectionToClientInterface>& searchClient)
{
    std::lock_guard<std::mutex> lock(SetMutex);
    return std::dynamic_pointer_cast<ConnectionToClient>(Clients.find(searchClient)._Ptr->_Myval);
}

void ThreadSafeSet::Remove(const std::shared_ptr<ConnectionToClientInterface>& clientToRemove)
{
    std::lock_guard<std::mutex> lock(SetMutex);
    Clients.erase(clientToRemove);
}
