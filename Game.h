#pragma once
#include <SFML/Graphics.hpp>
#include <chrono>
#include <fstream>
#include <functional>
#include <iostream>
#include <random>
#include <string>

#include "Button.h"
#include "Counter.h"
#include "Textures.h"
#include "Tile.h"

class Game {
public:
    explicit Game(int bombs);
    void draw(sf::RenderWindow& window);
    void run();
    void setGame();
    void setGame(std::string fileName);

    static const size_t kWidth = 25,
                        kHeight = 16;

private:
    Textures texture;
    bool debugMode = false;
    int bombCount = 0;
    int gameState = 0;
    int userBombs = 0;
    // public:
    Tile gameBoard[kWidth][kHeight];

private:
    int calcNeighbours(int, int);
    void drawGameBoard(sf::RenderWindow&);
    void handleMouse(sf::Event::MouseButtonEvent mouse, sf::RenderWindow& window);
    void drawMineCounter(sf::RenderWindow&);
    void drawGameState(sf::RenderWindow&);
    void drawButtons(sf::RenderWindow&);
    void setValueOverlay(Tile*);
    void resetBoard();
    void handleTileClick(sf::Event::MouseButtonEvent mouse);
    void revealAllBombs();
    bool checkWin();
    void floodFill(int x, int y);
    void flagBombs();

private:
    Button gameStateButton, debugButton,
        test1, test2, test3;
    Counter bombCounter;
    const std::string testFile1 = "boards/testboard1.brd";
    const std::string testFile2 = "boards/testboard2.brd";
    const std::string testFile3 = "boards/testboard3.brd";
};
