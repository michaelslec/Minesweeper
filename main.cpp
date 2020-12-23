#include "Game.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Textures.h"

int main(int argc, char* argv[])
{
    int bombs = 50;
    if (argc == 2) bombs = atoi(argv[1]);

    Game game(bombs);
    game.run();

    return 0;
}
