#pragma once
#include <SFML/Graphics.hpp>

class Tile {
public:
    Tile();
    bool revealed = false;
    bool flagged = false;
    int val = 0;
    sf::RectangleShape tile;
    sf::RectangleShape overlay;
};