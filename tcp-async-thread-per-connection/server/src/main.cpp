#include <iostream>
#include <unistd.h>
#include <cstring>
#include <thread>
#include <vector>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <mutex>

#include <fraction.h>
#include <fractionwire.h>

std::mutex clientID_lock;
int clientID = 0;

void openConnection(int acceptedSocket)
{
    FractionWire wire;
    int clientID_copy;

    clientID_lock.lock();
    {
        ++clientID;
        clientID_copy = clientID;
    }
    clientID_lock.unlock();

    while (true)
    {

        int byteCount = ::recv(acceptedSocket, &wire, sizeof(wire), 0);

        if (byteCount == 0)
        {
            std::cout << "[server] Client " << clientID_copy << " has disconnected.\n";
            break;
        }

        if (byteCount > 0)
        {
            std::cout << "[server] Message received from clientID " << clientID_copy << " : ";    
            
            Fraction frac(ntohl(wire.m_numerator), ntohl(wire.m_denominator));
            frac.print();
        }
        else 
        { 
            std::cout << "[server] Error accepting message.\n"; 
            break;
        }

    }
}

int main()
{
    // Create socket
    int serverSocket = ::socket(PF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1)
    {
        std::cerr << "[server] Error at socket():" << errno
            << " (" << std::strerror(errno) << ")\n";
    } else { std::cout << "[server] socket() is OK!\n"; }


    // Bind the socket
    sockaddr_in address{ };
    address.sin_family = AF_INET;
    address.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &address.sin_addr);

    if (::bind(serverSocket, reinterpret_cast<sockaddr*>(&address), sizeof(address)) == -1)
    {
        std::cerr << "[server] Error at bind():" << errno
            << " (" << std::strerror(errno) << ")\n";
        ::close(serverSocket);
        return 0;
    } else { std::cout << "[server] bind() is OK!\n";  }

    
    // Listen out on the socket
    if (::listen(serverSocket, 1) == -1)
    {
        std::cerr << "[server] Error at listen():" << errno
            << " (" << std::strerror(errno) << ")\n";
    } else { std::cout << "[server] listen is OK!\n"; }

    
    //std::vector<std::thread*> threads;
    for (;;)
    {
        int acceptedSocket = ::accept(serverSocket, NULL, NULL);
        if (acceptedSocket == -1)
        {
            std::cerr << "[server] Error at accept():" << errno
                << " (" << std::strerror(errno) << ")\n";
        }
        else
        {
            //threads.push_back(new std::thread(openConnection, acceptedSocket));
            std::thread t(openConnection, acceptedSocket);
            
            if (t.joinable())
                t.detach();
        }
    }

    return 0;
}
