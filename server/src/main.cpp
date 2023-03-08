#include <SFML/Network.hpp>
#include "chatServer.h"

int main()
{
    Server s(5000);
    s.run();
}
