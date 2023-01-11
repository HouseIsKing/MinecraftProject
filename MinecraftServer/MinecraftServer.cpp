#include <iostream>

int main(int /*argc*/, char* /*argv*/[])
{
    error_code error;
    
    ip::tcp::endpoint endpoint(ip::make_address("127.0.0.1", error), 25565);
    ip::tcp::socket socket(context);
    socket.connect(endpoint, error);
    std::cout << error.message();
    return 0;
}
