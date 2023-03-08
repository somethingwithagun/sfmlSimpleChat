#include "chatClient.h"
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <thread>
#include <time.h>

using namespace sf;

int main()
{
    std::string address;
    std::cout << "Enter the room ip: ";
    std::cin >> address;
    // TODO: make address check with std::regex

    Client c;

    c.establishConnection(address.c_str(), 5000);
    c.run();
    /* auto mouse_pos = sf::Mouse::getPosition(app);          // Mouse position relative to the window
    auto translated_pos = app.mapPixelToCoords(mouse_pos); // Mouse position translated into world coordinates
    if (s.getGlobalBounds().contains(translated_pos))      // Rectangle-contains-point check
    {
        std::cout << "pressed\n";
    } */
}
