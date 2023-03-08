#include "chatClient.h"
#include <SFML/Network.hpp>
#include <iostream>
#include <thread>
#include <chrono>

bool Client::establishConnection(const char *url, unsigned short port)
{
    std::cout << "Connecting...\n";
    if (_SOCKET.connect(sf::IpAddress(url), port) != sf::Socket::Done)
    {
        std::cout << "Could not establish connection\n";
    }
    else
    {
        isConnected = true;
        std::cout << "Connected!\n";
    }
    return true;
}
void Client::sendPacket(sf::Packet &packet)
{
    if (packet.getDataSize() > 0 && _SOCKET.send(packet) != sf::Socket::Done)
    {
        std::cout << "Error while sending data! Connection corrupted" << std::endl;

        // 80200053 is connection corruption error exit code
        exit(80200053);
    }
}

bool Client::receivePacket(sf::TcpSocket *socket)
{
    while (true)
    {
        if (socket->receive(lastPacket) == sf::Socket::Done)
        {
            std::string receivedString;
            std::string senderAddress;
            unsigned short senderPort;
            lastPacket >> receivedString >> senderAddress >> senderPort;
            std::cout << "From (" << senderAddress << ':' << senderPort << "): " << receivedString << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
    }
}
void Client::run()
{
    std::thread receptionThread(&Client::receivePacket, this, &_SOCKET);

    while (true) // app runtime loop
    {
        if (isConnected)
        {
            std::string userInput;
            std::getline(std::cin, userInput);

            if (userInput.size() < 1)
                continue;

            sf::Packet reply;
            reply << userInput;
            sendPacket(reply);
        }
    }
}
