#pragma once
#include <SFML/Graphics.hpp>


/* xOffset, &sf::Texture
 * isClicked(int x, int y)
 * xMin, xMax, yMin, yMax
 * 
 *
 */

class Button : public sf::RectangleShape {
public:
    Button(float xOffset, sf::Texture& texture);
    bool isClicked(int x ,  int y);

private:
    void calcCoords(float xOffset);
    int xMin, xMax,
        yMin, yMax;
    const int height = 16 * 32,
              size = 64;
};
