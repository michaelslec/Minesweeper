#include "Counter.h"


/**
 * 
 * set texture for each digit
 * set position & size
 * setCounter() which will set proper digit for each rectangle
 */ 
Counter::Counter(int initVal, sf::Texture& texture) {
    this->digit1.setTexture(&texture);
    this->digit1.setPosition(this->digitWidth * 0, this->height);
    this->digit1.setSize(sf::Vector2f(this->digitWidth, 32));

    this->digit2.setTexture(&texture);
    this->digit2.setPosition(this->digitWidth * 1, this->height);
    this->digit2.setSize(sf::Vector2f(this->digitWidth, 32));

    this->digit3.setTexture(&texture);
    this->digit3.setPosition(this->digitWidth * 2, this->height);
    this->digit3.setSize(sf::Vector2f(this->digitWidth, 32));

    this->setCounter(initVal);
}

void Counter::setCounter(int val) {
    // 482
    int oneMult   = abs(val) / 100,
        twoMult   = abs(val) % 100 / 10,
        threeMult = abs(val) % 100 % 10;
    
    if (val < 0)
        this->digit1.setTextureRect(sf::IntRect(this->digitWidth * 10, 0, 21, 32));
    else
        this->digit1.setTextureRect(sf::IntRect(this->digitWidth * oneMult, 0, 21, 32));

    this->digit2.setTextureRect(sf::IntRect(this->digitWidth * twoMult, 0, 21, 32));
    this->digit3.setTextureRect(sf::IntRect(this->digitWidth * threeMult, 0, 21, 32));
}