#pragma once
#include <asio/ip/tcp.hpp>

#include "Entities/Player.h"

class ConnectionToClient
{
    asio::ip::tcp::socket Socket;
    Player RepresentingPlayer;

public:
    explicit ConnectionToClient(asio::io_context& ioContext);
};
