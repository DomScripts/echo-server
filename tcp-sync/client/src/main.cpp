#include <iostream>
#include <sys/socket.h> // socket(), AF_INET, SOCK_STREAM, etc.
#include <cerrno> // errno
#include <cstring> // std::strerror
#include <netinet/in.h> // IPPROTO_TCP, htons, sockaddr_in, etc.
#include <arpa/inet.h> // inet_pton
#include <unistd.h> // close()
                    
#include <fraction.h>
#include <fractionwire.h>

int main()
{
    // ---- Create Socket ----
    int clientSocket{ };
    clientSocket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == -1)
    {
        std::cout << "Error at socket():" << errno
            << " (" << std::strerror(errno) << ")\n";
    }
    else
    {
        std::cout << "socket() is OK!\n";
    }

    // ---- Connect Socket ----
    sockaddr_in clientService;
    clientService.sin_family = AF_INET;
    clientService.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &clientService.sin_addr);

    if (::connect(clientSocket, reinterpret_cast<sockaddr*>(&clientService), sizeof(clientService)) == -1)
    {
        std::cout << "Error at socket():" << errno
            << " (" << std::strerror(errno) << ")\n";
    }
    else
    {
        std::cout << "Client: connect() is OK!\n";
        std::cout << "Client: Can start sending and receiving data...\n";
    }

    // ---- Sending TCP Data ----
    Fraction frac{ 1, 2 };

    FractionWire wire{ };
    wire.m_numerator = htonl(frac.getNumerator());
    wire.m_denominator = htonl(frac.getDenominator());

    int byteCount = ::send(clientSocket, &wire, sizeof(wire), 0);

    if (byteCount > 0)
    {
        std::cout << "Sent message: ";
        frac.print();
    }
    else
    {
        std::cout << "Message failed to send.\n";
    }

    // ---- Recieve Confirmation Message ----
    char buffer[200];
    byteCount = ::recv(clientSocket, buffer, 200,0);

    if (byteCount > 0)
    {
        std::cout << "Message received: " << buffer << '\n';
    }   

    ::close(clientSocket);
    return 0;
}
