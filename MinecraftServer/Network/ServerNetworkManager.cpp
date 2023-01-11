#include "ServerNetworkManager.h"

ServerNetworkManager::ServerNetworkManager() : Acceptor(Context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), 25565))
{
}

ServerNetworkManager::~ServerNetworkManager()
{
}
