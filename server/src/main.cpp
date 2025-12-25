#include <iostream>
#include <sys/socket.h> // socket(), AF_INET, SOCK_STREAM, etc.
#include <cerrno> // errno
#include <cstring> // std::strerror
#include <netinet/in.h> // IPPROTO_TCP, htons, sockaddr_in, etc.
#include <arpa/inet.h> // inet_pton
#include <unistd.h> // close()

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
        std::cout << "Error at socket():" << errno
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

    return 0;
}
