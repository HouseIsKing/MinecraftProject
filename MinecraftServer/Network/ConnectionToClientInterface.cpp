#include "ConnectionToClientInterface.h"

#include <utility>

ConnectionToClientInterface::ConnectionToClientInterface(std::string name) : ClientName(std::move(name))
{
}

const std::string& ConnectionToClientInterface::GetPlayerName() const
{
    return ClientName.GetName();
}
