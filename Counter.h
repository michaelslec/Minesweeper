#pragma once
#include <SFML/Graphics.hpp>

class Counter {
public:
    Counter(int initVal, sf::Texture& texture);
    void setCounter(int value);
    sf::RectangleShape digit1, digit2, digit3;

private:
    int height = 16 * 32,
        digitWidth = 21;
};