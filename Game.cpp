
#include "Game.h"

Game::Game(int bombs)
    : gameStateButton(11.5, this->texture.faceHappy)
    , debugButton(15.5, this->texture.debug)
    , test1(17.5, this->texture.test1)
    , test2(19.5, this->texture.test2)
    , test3(21.5, this->texture.test3)
    , bombCounter(-53, this->texture.digits)
    , userBombs{bombs}
{
    // debugButton button: 496
    this->setGame();
    // Sets position of tiles
    for (int y = 0; y < kHeight; ++y) {
        for (int x = 0; x < kWidth; ++x) {
            this->gameBoard[x][y].tile.setPosition(x * 32, y * 32);
            this->gameBoard[x][y].overlay.setPosition(x * 32, y * 32);
        }
    }

}

void Game::resetBoard() {
    this->gameState = 0;
    this->bombCount = this->userBombs;
    for (int y = 0; y < kHeight; ++y) {
        for (int x = 0; x < kWidth; ++x) {
            this->gameBoard[x][y].flagged = false;
            this->gameBoard[x][y].val = 0;
            this->gameBoard[x][y].revealed = false;
            this->gameBoard[x][y].tile.setTexture(&this->texture.tileHidden);
        }
    }
}

void Game::setGame()
{
    this->resetBoard();
    // Randomly generate bomb locations
    std::mt19937 rngX(std::chrono::steady_clock::now().time_since_epoch().count());
    std::mt19937 rngY(std::chrono::steady_clock::now().time_since_epoch().count());
    auto xLocation = std::bind(std::uniform_int_distribution<int>(0, kWidth - 1), rngX);
    auto yLocation = std::bind(std::uniform_int_distribution<int>(0, kWidth - 1), rngY);

    // Randomly place bombCount bombs
    int bombsToPlace = this->bombCount;
    while (bombsToPlace != 0) {
        int x = xLocation();
        int y = yLocation();
        if (this->gameBoard[x][y].val == 9)
            continue;

        this->gameBoard[x][y].val = 9;
        --bombsToPlace;
    }

    // Calculate number for all non-bombs
    for (int y = 0; y < kHeight; ++y) {
        for (int x = 0; x < kWidth; ++x) {
            if (this->gameBoard[x][y].val == 9)
                continue;

            this->gameBoard[x][y].val = this->calcNeighbours(x, y);
        }
    }
}

void Game::setGame(std::string fileName)
{
    this->resetBoard();

    std::ifstream board(fileName);
    if (!board) {
        std::cerr << "Could not open .brd file: " << fileName << std::endl;
    }

    std::string temp;
    int count = 0, bombCount = 0;
    while (std::getline(board, temp)) {
        for (int i = 0; i < kWidth; ++i) {
            if (temp[i] == '0') {
                this->gameBoard[i][count].val = 0;
            } else {
                this->gameBoard[i][count].val = 9;
                bombCount++;
            }
        }
        ++count;
    }
    this->bombCount = bombCount;

    // Calculate number for all non-bombs
    for (int y = 0; y < kHeight; ++y) {
        for (int x = 0; x < kWidth; ++x) {
            if (this->gameBoard[x][y].val == 9)
                continue;

            this->gameBoard[x][y].val = this->calcNeighbours(x, y);
        }
    }
}

void Game::run()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Minesweeper");
    // First draw
    this->draw(window);

    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::MouseButtonPressed:
                this->handleMouse(event.mouseButton, window);
            }
        }
    }
}

void Game::handleMouse(sf::Event::MouseButtonEvent mouse, sf::RenderWindow& window)
{
    if (mouse.y < kHeight * 32) {
        this->handleTileClick(mouse);
    }

    if (this->debugButton.isClicked(mouse.x, mouse.y)) {
        this->debugMode = !this->debugMode;
    }

    if (this->gameStateButton.isClicked(mouse.x, mouse.y)) {
        this->setGame();
    }
    
    if (this->test1.isClicked(mouse.x, mouse.y)) {
        this->setGame(this->testFile1);
    }
    
    if (this->test2.isClicked(mouse.x, mouse.y)) {
        this->setGame(this->testFile2);
    }
    
    if (this->test3.isClicked(mouse.x, mouse.y)) {
        this->setGame(this->testFile3);
    }

    this->draw(window);
}

bool Game::checkWin() {
    for (int y = 0; y < kHeight; ++y) {
        for (int x = 0; x < kWidth; ++x) {
            if (this->gameBoard[x][y].val != 9 && !this->gameBoard[x][y].revealed)
                return false;
        }
    }

    return true;
}

void Game::handleTileClick(sf::Event::MouseButtonEvent mouse)
{
    int x = mouse.x / 32,
        y = mouse.y / 32;

    if (this->gameState != 0)
        return;

    switch (mouse.button) {
    case sf::Mouse::Left:
        if (!this->gameBoard[x][y].flagged) {
            if (this->gameBoard[x][y].val == 9) {
                this->revealAllBombs();
                this->gameState = -1;
            } else if (this->gameBoard[x][y].val == 0) {
                this->floodFill(x, y);
            } else {
                this->gameBoard[x][y].revealed = true;
            }
        }
        break;
    case sf::Mouse::Right:
        if (!this->gameBoard[x][y].revealed) {
            if (this->gameBoard[x][y].flagged) {
                this->gameBoard[x][y].flagged = false;
                this->bombCount++;
            } else {
                this->gameBoard[x][y].flagged = true;
                this->bombCount--;
            }
        }
        break;
    }

    if (this->checkWin()) {
        this->gameState = 1;
        this->flagBombs();
    }
}

void Game::flagBombs()
{
    for (int y = 0; y < kHeight; ++y) {
        for (int x = 0; x < kWidth; ++x) {
            if (this->gameBoard[x][y].val == 9)
                this->gameBoard[x][y].flagged = true;
        }
    }
    this->bombCount = 0;
}

void Game::floodFill(int x, int y) {
    Tile* current = &this->gameBoard[x][y];
    
    if (!current->revealed) {
        current->revealed = true;

        if (current->val > 0)
            return;

        for (int i = -1; i <= 1; ++i) {
            if (y + i < 0 || y + i >= kHeight)
                continue;
            for (int j = -1; j <= 1; ++j) {
                if (x + j < 0 || x + j >= kWidth)
                    continue;

                if (this->gameBoard[x + j][y + i].val != 9 && !this->gameBoard[x +j][y + i].flagged)
                    floodFill(x + j, y + i);
            }
        }
    }
}

void Game::revealAllBombs()
{
    for (int y = 0; y < kHeight; ++y) {
        for (int x = 0; x < kWidth; ++x) {
            if (this->gameBoard[x][y].val == 9) {
                this->gameBoard[x][y].revealed = true;
                this->gameBoard[x][y].flagged = false;
            }
        }
    }
}

void Game::draw(sf::RenderWindow& window)
{
    window.clear(sf::Color::White);
    this->drawGameBoard(window);
    this->drawMineCounter(window);
    this->drawGameState(window);
    this->drawButtons(window);
    window.display();
}

// TODO
void Game::drawGameBoard(sf::RenderWindow& window)
{
    Tile* current;
    for (int y = 0; y < kHeight; ++y) {
        for (int x = 0; x < kWidth; ++x) {
            current = &this->gameBoard[x][y];
            if (current->revealed) {
                current->tile.setTexture(&this->texture.tileRevealed);
            }
            window.draw(current->tile);

            if (this->debugMode && current->val == 9) {
                current->overlay.setTexture(&this->texture.mine);
                window.draw(current->overlay);
            } else if (current->flagged) {
                current->overlay.setTexture(&this->texture.flag);
                window.draw(current->overlay);
            } else if (current->revealed) {
                this->setValueOverlay(current);
                window.draw(current->overlay);
            }
        }
    }
}

void Game::setValueOverlay(Tile* current)
{
    switch (current->val) {
    case 0:
        current->overlay.setTexture(current->tile.getTexture());
        break;
    case 1:
        current->overlay.setTexture(&this->texture.one);
        break;
    case 2:
        current->overlay.setTexture(&this->texture.two);
        break;
    case 3:
        current->overlay.setTexture(&this->texture.three);
        break;
    case 4:
        current->overlay.setTexture(&this->texture.four);
        break;
    case 5:
        current->overlay.setTexture(&this->texture.five);
        break;
    case 6:
        current->overlay.setTexture(&this->texture.six);
        break;
    case 7:
        current->overlay.setTexture(&this->texture.seven);
        break;
    case 8:
        current->overlay.setTexture(&this->texture.eight);
        break;
    case 9:
        current->overlay.setTexture(&this->texture.mine);
        break;
    }
}

void Game::drawMineCounter(sf::RenderWindow& window)
{
    this->bombCounter.setCounter(this->bombCount);
    window.draw(this->bombCounter.digit1);
    window.draw(this->bombCounter.digit2);
    window.draw(this->bombCounter.digit3);
}

void Game::drawGameState(sf::RenderWindow& window)
{
    switch (this->gameState) {
    case -1:
        this->gameStateButton.setTexture(&this->texture.faceLose);
        break;
    case 0:
        this->gameStateButton.setTexture(&this->texture.faceHappy);
        break;
    case 1:
        this->gameStateButton.setTexture(&this->texture.faceWin);
        break;
    }
    window.draw(this->gameStateButton);
}

void Game::drawButtons(sf::RenderWindow& window)
{
    window.draw(this->debugButton);
    window.draw(this->test1);
    window.draw(this->test2);
    window.draw(this->test3);
}

int Game::calcNeighbours(int x, int y)
{
    int bombCount = 0;
    for (int i = -1; i <= 1; ++i) {
        if (y + i < 0 || y + i >= kHeight)
            continue;
        for (int j = -1; j <= 1; ++j) {
            if (x + j < 0 || x + j >= kWidth)
                continue;
            if (this->gameBoard[x + j][y + i].val == 9)
                bombCount++;
        }
    }

    return bombCount;
}
