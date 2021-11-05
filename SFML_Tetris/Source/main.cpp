#include <iostream>
#include <SFML/Graphics.hpp>

#include "BlockyShape.h"
#include "main.h"

int windowWidth = 1080;
int windowHeight = 720;

unsigned const int tileSize      = 8;
unsigned const int columns       = 10;  // Amount of blocks along X
unsigned const int rows          = 20;  // Amount of blocks along Y
unsigned const int screenResize  = 6;

// Set RenderWindow size based on the amount of columns, rows, the size of the tiles, * the screen resize.
sf::RenderWindow window(sf::VideoMode(tileSize* columns* screenResize, tileSize* rows* screenResize), "C++ Tetris Game");

bool leftPressed, rightPressed, upPressed, downPressed; 
bool rotatePressed;
unsigned short updateSpeed = 0300.0f; // 1000 = 1 second
unsigned int offset = 5;              // Offset of the block upon initialization
// CURRENTLY UNUSED ^^^ --------------------------------------------------------------------------------------------------------------------------------------------

bool gameStarted;

int blockPosX, blockPosY;
unsigned int tileArray[columns][rows];

// Block coordinates
sf::Vector2i blockPos[4];

sf::Texture tRandom;   // Random Texture
sf::Texture tMainMenu; // Main Menu Texture

sf::Texture tBlack;
sf::Texture tBlue;
sf::Texture tGreen;
sf::Texture tOrange;
sf::Texture tPink;
sf::Texture tRed;
sf::Texture tCyan;

int main()
{
    // Random seed generation for random blocks in spawnShape()
    std::srand(time(nullptr));
    
    // Reserve memory for tileArray
    std::memset(tileArray, 0, sizeof(tileArray));

    sf::Clock clock;

    // Set the view (kinda like setting the camera)
    window.setView(sf::View(sf::FloatRect(0, 0, tileSize * columns, tileSize * rows)));

    loadTextures();

    // Display Main Intro Menu
    window.clear();
    sf::Sprite mainMenuSprite;
    mainMenuSprite.setTexture(tMainMenu);
    mainMenuSprite.setPosition(0,0);
    window.draw(mainMenuSprite);
    window.display();

    spawningShape();
    //tileArray[blockPosX][blockPosY] = 1;

    while (window.isOpen())
    {
        // Start clock if the game has started
        if (gameStarted == true) {
            // Calling update based on updateSpeed float veriable
            sf::Time elapsedTime = clock.getElapsedTime();
            if (elapsedTime >= sf::milliseconds(updateSpeed)) {
                elapsedTime = sf::milliseconds(0.0f);
                clock.restart();
                std::cout << "Updating Game..." << std::endl;
                update();
            }
        }

        // Eventmanager
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Left) {
                leftPressed = true; rightPressed = false; downPressed = false;
            }
            if (event.key.code == sf::Keyboard::Right) {
                leftPressed = false; rightPressed = true; downPressed = false;
            }
            if (event.key.code == sf::Keyboard::Down) {
                leftPressed = false; rightPressed = false; downPressed = true;
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            rotatePressed = true; gameStarted = true;
        }
    }
}

void rendering() 
{
    // Rendering Tiles
    sf::Sprite tileShape;
    tileShape.setTextureRect(sf::IntRect(0, 0, tileSize, tileSize));

    window.clear();

    // Drawing tiles on the tileArray board
    for (unsigned int y = 0; y < rows; y++) {
        for (unsigned int x = 0; x < columns; x++)
        {
            if (tileArray[x][y] == 0)   // 0 = Empty
            {
                tileShape.setTexture(tBlack);
                tileShape.setPosition(tileSize * x, tileSize * y);
            }
            if (tileArray[x][y] == 1)   // 1 = Dynamic
            {
                tileShape.setTexture(tRandom);
                tileShape.setPosition(tileSize * x, tileSize * y);
                tileArray[x][y] = 0;
            }
            if (tileArray[x][y] == 2)   // 2 = Static
            {
                tileShape.setTexture(tRed);
                tileShape.setPosition(tileSize * x, tileSize * y);
            }
            window.draw(tileShape);
        }
    }
    window.display();
}

// Load textures from disk and store them in memory so we don't have to re-read from disk every time we want a new texture
void loadTextures() {
    if (!tMainMenu  .loadFromFile("Sprites/mainmenu.png")) {}

    if (!tBlack     .loadFromFile("Sprites/block_black.png")) {}
    if (!tBlue      .loadFromFile("Sprites/block_blue.png")) {}
    if (!tGreen     .loadFromFile("Sprites/block_green.png")) {}
    if (!tOrange    .loadFromFile("Sprites/block_orange.png")) {}
    if (!tPink      .loadFromFile("Sprites/block_pink.png")) {}
    if (!tRed       .loadFromFile("Sprites/block_red.png")) {}
    if (!tCyan      .loadFromFile("Sprites/block_cyan.png")) {}
}

BlockyShape spawnShape() 
{
    // Select random blocktype
    BlockyShape blockyShape((BlockType)(std::rand() % (int)BlockType::count));

    // Take vector positions from BlockyShape.cpp and store it locally
    blockPos[0] = blockyShape.mBlocks[0];
    blockPos[1] = blockyShape.mBlocks[1];
    blockPos[2] = blockyShape.mBlocks[2];
    blockPos[3] = blockyShape.mBlocks[3];

    std::cout << blockyShape.getColour().toInteger() << std::endl;  // Temporary function to detect if it spawns a new blockyshape
    return blockyShape;
}

// Detecting whether the block cannot move down anymore (and thus become static)
bool collissionDetection() 
{
    for (unsigned int i = 0; i < 4; i++) {
        if (tileArray[blockPos[i].x][blockPos[i].y + 1] == 2 || blockPos[i].y >= (rows - 1)) {
            std::cout << "------- " << "BlockPosY collission detected" << std::endl;
            setDynamicToStatic();
            spawningShape();
            return true;
        }
    }
    return false;
}
bool checkLeftMovementPossible() 
{
    for (unsigned int i = 0; i < 4; i++) {
        if (tileArray[blockPos[i].x - 1][blockPos[i].y] == 2 || blockPos[i].x <= 0) {
            return false;
        }
    }
    return true;
}
bool checkRightMovementPossible()
{
    for (unsigned int i = 0; i < 4; i++) {
        if (tileArray[blockPos[i].x + 1][blockPos[i].y] == 2 || blockPos[i].x >= (columns - 1)) {
            return false;
        }
    }
    return true;
}

void setDynamicToStatic() 
{
    std::cout << "------- " << "Trying to Set a Dynamic Object to Static..." << std::endl;
    for (unsigned int y = 0; y < rows; y++) {
        for (unsigned int x = 0; x < columns; x++) {
            if (tileArray[x][y] == 1) { // If tile == 1 (dynamic) set tile to 2 (static)
                tileArray[x][y] = 2;
                std::cout << "------- " << "Set a Dynamic Object to Static!" << std::endl;
            }
        }
    }
}

void spawningShape() 
{
    std::cout << "------- " << "Spawning new block" << std::endl;

    unsigned int randomNumber = rand() % 5;
    switch (randomNumber)
    {
    case 0: 
        tRandom = tBlue;
        break;
    case 1:
        tRandom = tGreen;
        break;
    case 2:
        tRandom = tOrange;
        break;
    case 3:
        tRandom = tPink;
        break;
    case 4:
        tRandom = tCyan;
        break;
    default:
        break;
    }

    BlockyShape blockyShape = spawnShape();
}


void update() 
{
    collissionDetection(); 

    // Move dynamic block piece down every update
    blockPos[0].y++; blockPos[1].y++; blockPos[2].y++; blockPos[3].y++;

    // Checking keyboard input from gameInput() and allowing one to be active at every update
    if (leftPressed == true) {
        leftPressed = rightPressed = upPressed = downPressed = false;
        std::cout << "Key 'left' pressed" << std::endl;
        checkLeftMovementPossible();
        if (checkLeftMovementPossible() == true) {
            blockPos[0].x--; blockPos[1].x--; blockPos[2].x--; blockPos[3].x--;

        }
    }
    if (rightPressed == true) {
        leftPressed = rightPressed = upPressed = downPressed = false;
        std::cout << "Key 'right' pressed" << std::endl;
        checkRightMovementPossible();
        if (checkRightMovementPossible() == true) {
            blockPos[0].x++; blockPos[1].x++; blockPos[2].x++; blockPos[3].x++;
        }
    }
    if (upPressed == true) {
        leftPressed = rightPressed = upPressed = downPressed = false;
        std::cout << "Key 'up' pressed" << std::endl;
        if (collissionDetection() == false) {
        }
    }
    if (downPressed == true) {
        leftPressed = rightPressed = upPressed = downPressed = false;
        std::cout << "Key 'down' pressed" << std::endl;
        if (collissionDetection() == false) {
            blockPos[0].y++; blockPos[1].y++; blockPos[2].y++; blockPos[3].y++;
        }
    }
    if (rotatePressed == true) {
        rotatePressed = false;
        std::cout << "Key 'rotate' pressed" << std::endl;
    }

    rendering();

    // Setting current position of object in grid to 1 (Dynamic)
    tileArray[blockPos[0].x][blockPos[0].y] = 1;
    tileArray[blockPos[1].x][blockPos[1].y] = 1;
    tileArray[blockPos[2].x][blockPos[2].y] = 1;
    tileArray[blockPos[3].x][blockPos[3].y] = 1;

    logging();
}

void gameOver() 
{
    std::cout << "------- " << "Game Over" << std::endl;
}

void logging() 
{
    //std::cout << "x: " << blockPosX << std::endl;
    //std::cout << "y: " << blockPosY << std::endl;

    std::cout << "x: " << blockPos[0].x << " " << blockPos[1].x << " " << blockPos[2].x << " " << blockPos[3].x << " " << std::endl;
    std::cout << "y: " << blockPos[0].y << " " << blockPos[1].y << " " << blockPos[2].y << " " << blockPos[3].y << " " << std::endl;

    if (collissionDetection() == true) {
        std::cout << "CollissionDetection: " << "True" << std::endl;
    }
    if (collissionDetection() == false) {
        std::cout << "CollissionDetection: " << "False" << std::endl;
    }

    if (checkLeftMovementPossible() == true) {
        std::cout << "LeftMovementPossible: " << "True" << std::endl;
    }
    if (checkLeftMovementPossible() == false) {
        std::cout << "LeftMovementPossible: " << "False" << std::endl;
    }

    if (checkRightMovementPossible() == true) {
        std::cout << "RightMovementPossible: " << "True" << std::endl;
    }
    if (checkRightMovementPossible() == false) {
        std::cout << "RightMovementPossible: " << "False" << std::endl;
    }
}