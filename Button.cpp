#include "Button.h"

Button::Button(float xOffset, sf::Texture& texture)
{
    // texture, position, size
    this->setTexture(&texture);
    this->setPosition(xOffset * 32, this->height);
    this->setSize(sf::Vector2f(this->size, this->size));
    this->calcCoords(xOffset);
}

void Button::calcCoords(float xOffset) {
    this->xMin = static_cast<int>(xOffset * 32.0f);
    this->xMax = this->xMin + this->size;
    this->yMin = this->height;
    this->yMax = this->yMin + this->size;
}

bool Button::isClicked(int x, int y)
{
    return x >= xMin && x < xMax && y > yMin && y <= yMax;
}