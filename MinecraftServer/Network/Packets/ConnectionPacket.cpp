#include "ConnectionPacket.h"


ConnectionPacket::ConnectionPacket(ConnectionToClient* connection, const PacketHeader& header) : ThePacket(header), Connection(connection)
{
}
