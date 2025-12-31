#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <cstdint>
#include <cerrno>
#include <cstring>
#include <iostream>

#include <fraction.h>
#include <fractionwire.h>

int main()
{
    // Create client socket
    int clientSocket{ };
    clientSocket = ::socket(AF_INET, SOCK_DGRAM, 0);
    if (clientSocket == -1)
    {
        std::cout << "Error at socket():" << errno 
            << " (" << std::strerror(errno) << ")\n";
    } else { std::cout << "socket() is OK!\n"; }

    sockaddr_in server{ };
    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    ::inet_pton(AF_INET, "127.0.0.1", &server.sin_addr);

    // Send message to the sever
    Fraction frac{ 1, 2 };
    FractionWire wire{ };

    for (int i = 0; i <5; i++)
    {
        wire.m_numerator = htonl(frac.getNumerator());
        wire.m_denominator = htonl(frac.getDenominator());

        int byteCount = ::sendto(clientSocket, &wire, sizeof(wire), 0,
                                 reinterpret_cast<sockaddr*>(&server), sizeof(server));

        if (byteCount == -1)
        {
             std::cout << "Error at sendto():" << errno
                 << " (" << std::strerror(errno) << ")\n";
        } else { std::cout << "Message sent to server!\n"; }

        frac.setNumerator(frac.getNumerator() + 1);
        frac.setDenominator(frac.getDenominator() + 1);
    }

    ::close(clientSocket);
    return 0;
}
