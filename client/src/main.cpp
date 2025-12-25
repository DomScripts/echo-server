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

    return 0;
}
