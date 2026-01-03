include <iostream>
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
    int serverSocket{ };
    serverSocket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == -1)
    {
        std::cout << "Error at socket():" << errno
            << " (" << std::strerror(errno) << ")\n";
    }
    else
    {
        std::cout << "socket() is OK!\n";
    }

    // ---- Bind Socket ----
    sockaddr_in service{ };
    service.sin_family = AF_INET;
    service.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &service.sin_addr);

    if (::bind(serverSocket, reinterpret_cast<sockaddr*>(&service), sizeof(service)) == -1)
    {
        std::cout << "Error at bind():" << errno
            << " (" << std::strerror(errno) << ")\n";
        ::close(serverSocket);
        return 0;
    }
    else
    {
        std::cout << "bind() is OK!\n";
    }

    // ---- Listen Socket ----
    if (listen(serverSocket, 1) == -1)
    {
        std::cout << "Error at listen():" << errno
            << " (" << std::strerror(errno) << ")\n";
    }
    else
    {
        std::cout << "listen() is OK! Waiting for connections.\n";
    }

    // ---- Accept Socket ----
    int acceptSocket = accept(serverSocket, NULL, NULL);
    if (acceptSocket == -1)
    {
        std::cout << "Error at accept():" << errno
            << " (" << std::strerror(errno) << ")\n";
    }
    else
    {
        std::cout << "Accepted connection\n";
    }

    // ---- Recieve Client Data ----
    FractionWire wire{ };

    int byteCount = ::recv(acceptSocket, &wire, sizeof(wire), 0);
    
    Fraction frac{ };
    frac.setNumerator(ntohl(wire.m_numerator));
    frac.setDenominator(ntohl(wire.m_denominator));

    if (byteCount > 0)
    {
        std::cout << "Recieved message: ";
        frac.print();
    }

    // ---- Send Data Back ----
    char confirmation[200] = "Message Recieved!";

    byteCount = ::send(acceptSocket, confirmation, 200, 0);

    if (byteCount == -1)
    {
        std::cout << "Error at send():" << errno
            << " (" << std::strerror(errno) << ")\n";
    }
    else if (byteCount > 0)
    {
        std::cout << "Automated confirmation message send.\n";
    }
    else 
    {
        std::cout << "Failed to send automated confirmation message.\n";
    }

    ::close(serverSocket);
    return 0;
}
