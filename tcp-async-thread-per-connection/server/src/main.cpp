#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main()
{
    // Create socket
    int serverSocket = ::socket(PF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1)
    {
        std::cerr << "Error at socket():" << errno
            << " (" << std::strerror(errno) << ")\n";
    } else { std::cout "[server] socket() is OK!\n"; }


    // Bind the socket
    sockaddr_in address{ };
    address.sin_family = AF_INET;
    address.sin_port = htonsl(8080);
    inet_pton(AF_INET, "127.0.0.1", &address.sin_addr);

    if (::bind(serverSocket, reinterpret_cast<sockaddr*>(&address), sizeof(address)) == -1)
    {
        std::cerr << "Error at bind():" << errno
            << " (" << std::strerror(errno) << ")\n";
        ::close(serverSocket);
        return 0;
    } else { std::cout << "[server] bind() is OK!\n";  }

    
    // Listen out on the socket
    if (::listen(sockServer, 1) == -1)
    {
        std::cerr << "Error at listen():" << errno
            << " (" << std::strerror(errno) << ")\n";
    } else { std::cout << "[server] listen is OK!\n"; }

    
    // Accept incoming connection [update to accept many connections]
    int acceptedSocket = ::accept(serverSocket, NULL, NULL);
    if (acceptedSocket == -1)
    {
        std::cerr << "Error at accept():" << errno
            << " (" << std::strerror(errno) << ")\n";
    } else { std::cout << "[server] accept() is OK!\n"; }

    return 0;
}
