#include <SFML/Network.hpp>
#include <iostream>
#include <thread>
#include <chrono>

class Server
{
  public:
    Server(unsigned short port);
    ~Server(){}

    bool receivePacket(sf::TcpSocket* client, size_t iterator);
    bool clientDisconnected(sf::TcpSocket *client, size_t iterator);
    void broadcastMessage(sf::Packet packet, sf::IpAddress excludeAddress, unsigned short port);
    void run();
    void managePackets();
    void connectClients(std::vector<sf::TcpSocket *> *connections);


  private:
    unsigned short _PORT;
    sf::TcpListener _LISTENER;
    std::vector<sf::TcpSocket *> connections;

};