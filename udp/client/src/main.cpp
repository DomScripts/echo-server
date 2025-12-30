#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <cstdint>
#include <cerrno>
#include <cstring>
#include <iostream>

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
    const char* message = "Hello vio udp.";

    int byteCount = ::sendto(clientSocket, message, std::strlen(message), 0,
                            reinterpret_cast<sockaddr*>(&server), sizeof(server));

    if (byteCount == -1)
    {
        std::cout << "Error at sendto():" << errno
            << " (" << std::strerror(errno) << ")\n";
    } else { std::cout << "Message sent to server!\n"; }

    // Receive confirmation from server
    char buffer[2048];
    sockaddr_in from{ };
    socklen_t from_length = sizeof(from);

    byteCount = ::recvfrom(clientSocket, buffer, sizeof(buffer) - 1, 0,
                            reinterpret_cast<sockaddr*>(&from), &from_length);

    if (byteCount == -1)
    {
        std::cout << "Error at recvfrom():" << errno
            << " (" << std::strerror(errno) << ")\n";
    } 
    else 
    { 
        buffer[byteCount] = '\0';
        std::cout << "Message received: " << buffer << '\n'; 
    }

    ::close(clientSocket);
    return 0;
}
