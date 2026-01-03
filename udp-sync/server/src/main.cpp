#include <sys/socket.h> // socket(), AF_INET, etc.
#include <arpa/inet.h> // inet_pton
#include <unistd.h> // close()

#include <cstdint>
#include <cerrno>
#include <cstring>
#include <iostream>

#include <fraction.h>
#include <fractionwire.h>

int main()
{
    // Create the socket
    int serverSocket{ };
    serverSocket = ::socket(AF_INET, SOCK_DGRAM, 0);
    if (serverSocket == -1)
    {
        std::cout << "Error at socket():" << errno
            << " (" << std::strerror(errno) << ")\n";
    } else { std::cout << "socket() is OK!\n"; }

    // Bind IP and Port to the socket
    sockaddr_in addr{ };
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
    if (::bind(serverSocket, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == -1)
    {
        std::cout << "Error at bind():" << errno
            << " (" << std::strerror(errno) << ")\n";

        ::close(serverSocket);
        return 0;
    } else { std::cout << "bind() is OK!\n"; }

    std::cout << "UDP echo server on 127.0.0.1:8080\n";

    // Waiting for data
    for (;;)
    {
        FractionWire wire{ };

        sockaddr_in peer{ };
        socklen_t peerLength = sizeof(peer);
        
        int32_t byteSize = ::recvfrom(serverSocket, &wire, sizeof(wire), 0,
                                  reinterpret_cast<sockaddr*>(&peer), &peerLength);

        if (byteSize == -1)
        {
            std::cout << "Error at recvfrom():" << errno
                << " (" << std::strerror(errno) << ")\n";
            continue;
        }

        Fraction frac{ };
        frac.setNumerator(ntohl(wire.m_numerator));
        frac.setDenominator(ntohl(wire.m_denominator));

        std::cout << "Reveived value: ";
        frac.print();
    }

    ::close(serverSocket);
    return 0;
}
