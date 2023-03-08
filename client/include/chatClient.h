#include <SFML/Network.hpp>
#include <iostream>
class Client
{
  public:
    Client()
    {
    }
    ~Client()
    {
        _SOCKET.disconnect();
    }
    sf::TcpSocket *getSocket()
    {
        return &_SOCKET;
    }
    bool establishConnection(const char * url, unsigned short port);
    void sendPacket(sf::Packet& packet);
    bool receivePacket(sf::TcpSocket *socket);
    void run();
    // void waitForOther();

  private:
    bool isConnected = false;
    sf::Packet lastPacket;
    sf::TcpSocket _SOCKET;
};