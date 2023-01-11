#include "ConnectionToClient.h"

ConnectionToClient::ConnectionToClient(asio::io_context& ioContext) : Socket(ioContext)
{
}
