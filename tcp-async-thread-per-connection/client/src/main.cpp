#include <sys/socket.h>
#include <unistd.h>

int main()
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
    address.sin_port = htonl(8080);
    inet_pton(PF_INET, "127.0.0.1", &address.sin_addr);

    int socketConnected = ::connect(clientSocket, &address, sizeof(address));
    if (socketConnected == -1)
    {
        std::cout << "[client] Error at connect():" << errno
            << " (" << std::strerror(errno) << ")\n";

        ::close(clientSocket);
    } else { std::cout << "[client] connect() is OK!\n"; }

    // TODO
    // - Send data to server 

    return 0;
}
