#include <iostream>
#include <string>
#include <sys/socket.h>
#include <cerrno> // errno
#include <cstring> // std::strerror
#include <netinet/in.h> // IPPROTO_TCP, htons, sockaddr_in, etc.
#include <arpa/inet.h>
#include <unistd.h>

#include "fractionwire.h"

int main(int argc, char* argv[])
{
    // Create the client socket
    int clientSocket = ::socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == -1)
    {
        std::cout << "[client] Error at socket():" << errno
            << " (" << std::strerror(errno) << ")\n";
    } else { std::cout << "[client] socket() is OK!\n"; }

    // Connect the socket to server
    sockaddr_in address;
    address.sin_family = PF_INET;
    address.sin_port = htons(8080);
    inet_pton(PF_INET, "127.0.0.1", &address.sin_addr);

    int socketConnected = ::connect(clientSocket, reinterpret_cast<sockaddr*>(&address), sizeof(address));
    if (socketConnected == -1)
    {
        std::cout << "[client] Error at connect():" << errno
            << " (" << std::strerror(errno) << ")\n";

        ::close(clientSocket);
    } else { std::cout << "[client] connect() is OK!\n"; }

    // Send data to server
    int32_t numerator = static_cast<int32_t>(std::stoi(argv[1]));
    int32_t denominator = static_cast<int32_t>(std::stoi(argv[2]));

    for (int i = 0; i < std::stoi(argv[3]); ++i)
    {
        FractionWire wire{htonl(numerator), htonl(denominator)};

        int byteCount = ::send(clientSocket, &wire, sizeof(wire), 0);

        if (byteCount > 0)
        {
            std::cout << "[client] Sent a message to the server!: ";
            wire.printReadable();
        }
        else 
        {
            std::cout << "[client] Error, failed to send the message to the server!\n";
        }

        ++numerator;
        ++denominator;
        sleep(3);
    }

    return 0;
}
