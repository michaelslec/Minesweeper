#pragma once
#include <SFML/Graphics.hpp>

class Textures {
public:
    Textures();
    sf::Texture tileHidden, tileRevealed, debug,
        digits, faceHappy, faceLose, faceWin,
        flag, mine, one, two, three, four, five,
        six, seven, eight, test1, test2, test3;
};