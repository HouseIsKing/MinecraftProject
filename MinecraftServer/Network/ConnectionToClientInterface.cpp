#include "ConnectionToClientInterface.h"

#include <utility>

ConnectionToClientInterface::ConnectionToClientInterface(std::string name) : RepresentingPlayer(std::move(name))
{
}

const std::string& ConnectionToClientInterface::GetPlayerName() const
{
    return RepresentingPlayer.GetName();
}
