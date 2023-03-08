#include "chatServer.h"

Server::Server(unsigned short port) : _PORT(port)
{
    std::cout << "Listening...\n";
    if (_LISTENER.listen(_PORT) != sf::Socket::Done)
    {
        std::cout << "Cannot listen port\n";
        return;
    }
}
bool Server::receivePacket(sf::TcpSocket *client, size_t iterator)
{
    sf::Packet packet;
    int recResult = client->receive(packet);
    if (recResult == sf::Socket::Disconnected)
    {
        clientDisconnected(client, iterator);
        return 0;
    }

    if (recResult != sf::Socket::Done)
        return 1;

    if (packet.getDataSize() > 0)
    {
        std::string receivedMessage;
        packet >> receivedMessage;
        packet.clear();

        packet << receivedMessage << client->getRemoteAddress().toString() << client->getRemotePort();
        broadcastMessage(packet, client->getRemoteAddress(), client->getRemotePort());
    }
    return 0;
}

bool Server::clientDisconnected(sf::TcpSocket *client, size_t iterator)
{
    std::cout << "Disconnected " << client->getRemoteAddress() <<':'<< client->getRemotePort() << std::endl;
    client->disconnect();
    delete client;
    connections.erase(connections.begin() + iterator);
    return 0;
}

void Server::run()
{
    std::thread connectionThread(&Server::connectClients, this, &connections);

    managePackets();
}

void Server::managePackets()
{
    while (true)
    {

        for (size_t iterator = 0; iterator < connections.size(); iterator++)
        {
            receivePacket(connections[iterator], iterator);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
    }
}

void Server::connectClients(std::vector<sf::TcpSocket *> *connections)
{
    while (true)
    {
        sf::TcpSocket *newClient = new sf::TcpSocket;
        if (_LISTENER.accept(*newClient) == sf::Socket::Done)
        {
            newClient->setBlocking(false);
            connections->push_back(newClient);
            std::cout << "Added client " << newClient->getRemoteAddress().toString() <<':'<< newClient->getRemotePort()
                      << std::endl;
        }
        else
        {
            std::cout << "Server listener error, restart server\n";
            delete newClient;
            break;
        }
    }
}

void Server::broadcastMessage(sf::Packet packet, sf::IpAddress excludeAddress, unsigned short port)
{
    for (size_t iterator = 0; iterator < connections.size(); iterator++)
    {
        sf::TcpSocket *client = connections[iterator];

        if (client->getRemoteAddress() != excludeAddress || client->getRemotePort() != port)
            if (client->send(packet) != sf::Socket::Done)
                std::cout << "Could not send packet on broadcast\n";
    }
}
