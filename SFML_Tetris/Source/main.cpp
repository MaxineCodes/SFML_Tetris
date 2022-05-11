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


bool gameRunning;
bool restart;
unsigned int score;
int tileArray[columns][rows];
// Block coordinates
sf::Vector2i blockPos[4];

// Set RenderWindow size based on the amount of columns, rows, the size of the tiles, * the screen resize.
sf::RenderWindow window(sf::VideoMode(tileSize* columns* screenResize, tileSize* rows* screenResize), "C++ Tetris Game");

bool leftPressed, rightPressed, upPressed, downPressed; 
bool rotatePressed;
unsigned short updateSpeed = 0120.0f; // 1000 = 1 second   0320.0f
unsigned short restartDelay = 3500.0f;

// Offset of the block upon initialization
unsigned int offsetX = 4;
unsigned int offsetY = -1;

// Textures
sf::Texture tRandom;
sf::Texture tMainMenu;
sf::Texture tGameOverMenu;
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

    while (window.isOpen())
    {
        // Start clock if the game has started
        if (gameRunning) {
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

        // Input
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Left) {
                leftPressed = true; rightPressed = false; downPressed = false;
            }
            if (event.key.code == sf::Keyboard::Right) {
                leftPressed = false; rightPressed = true; downPressed = false;
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            rotatePressed = true; gameRunning = true;
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
    for (unsigned char y = 0; y < rows; y++) {
        for (unsigned char x = 0; x < columns; x++)
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
                // Cleans up last dynamic values so you dont get a funny trail
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
    if (!tMainMenu      .loadFromFile("Sprites/mainmenu.png")) {}
    if (!tGameOverMenu  .loadFromFile("Sprites/gameovermenu.png")) {}
    if (!tBlack         .loadFromFile("Sprites/block_black.png")) {}
    if (!tBlue          .loadFromFile("Sprites/block_blue.png")) {}
    if (!tGreen         .loadFromFile("Sprites/block_green.png")) {}
    if (!tOrange        .loadFromFile("Sprites/block_orange.png")) {}
    if (!tPink          .loadFromFile("Sprites/block_pink.png")) {}
    if (!tRed           .loadFromFile("Sprites/block_red.png")) {}
    if (!tCyan          .loadFromFile("Sprites/block_cyan.png")) {}
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
bool collisionDetection() 
{
    // Check for collision under the block
    for (unsigned char i = 0; i < 4; i++) {
        if (tileArray[blockPos[i].x][blockPos[i].y + 1] == 2 || blockPos[i].y >= (rows - 1)) {
            std::cout << "------- " << "Block Collision Detected" << std::endl;
            setDynamicToStatic();
            spawningShape();
            // Check if a static block is on the top row
            for (unsigned char i = 0; i < columns; i++) {
                if (tileArray[i][0] == 2) {
                    gameOver();
                }
            }
            return true;
        }
    }
    return false;
}
bool checkMovementPossible() 
{
    if (leftPressed == true) {
        for (unsigned char i = 0; i < 4; i++) {
            if (tileArray[blockPos[i].x - 1][blockPos[i].y] == 2 || blockPos[i].x <= 0) {
                return false;
            }
        }
    }
    if (rightPressed == true) {
        for (unsigned char i = 0; i < 4; i++) {
            if (tileArray[blockPos[i].x + 1][blockPos[i].y] == 2 || blockPos[i].x >= (columns - 1)) {
                return false;
            }
        }
    }
    return true;
}

void setDynamicToStatic() 
{
    std::cout << "------- " << "Trying to Set a Dynamic Object to Static..." << std::endl;
    for (unsigned char y = 0; y < rows; y++) {
        for (unsigned char x = 0; x < columns; x++) {
            if (tileArray[x][y] == 1) { // If tile == 1 (dynamic) set tile to 2 (static)
                tileArray[x][y] = 2;
                std::cout << "------- " << "Set a Dynamic Object to Static!" << std::endl;
            }
        }
    }
}

void checkAndClearFullRows() 
{
    // Thanks eebs for this code <3
    for (unsigned int y = 0; y < rows; y++) {
        // count the number of solid tiles for this row
        unsigned int tilesFull = 0;
        for (unsigned int x = 0; x < columns; ++x) {
            tilesFull += (tileArray[x][y] > 0) ? 1 : 0;
        }

        if (tilesFull >= columns) {
            std::cout << "row " << y << " is full" << std::endl;
            for (unsigned int x = 0; x < columns; ++x) {
                tileArray[x][y] = 0;
            }
            score++;

            // MOVE THE ROWS DOWN HERE BY MAKING EVERYTHING ABOVE THE CLEARED ROW DYNAMIC FOR 1 FRAME
        }
    }
}

void spawningShape() 
{
    std::cout << "------- " << "Spawning new block" << std::endl;

    // Giving block a random texture
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

    // Applying offset to the block that spawns
    blockPos[0].x = blockPos[0].x + offsetX;
    blockPos[0].y = blockPos[0].y + offsetY;
    blockPos[1].x = blockPos[1].x + offsetX;
    blockPos[1].y = blockPos[1].y + offsetY;
    blockPos[2].x = blockPos[2].x + offsetX;
    blockPos[2].y = blockPos[2].y + offsetY;
    blockPos[3].x = blockPos[3].x + offsetX;
    blockPos[3].y = blockPos[3].y + offsetY;
}


void update() 
{
    // Move dynamic block piece down every update
    blockPos[0].y++; blockPos[1].y++; blockPos[2].y++; blockPos[3].y++;

    // Checking keyboard input from gameInput() and allowing one to be active at every update
    if (leftPressed == true) {
        checkMovementPossible();
        std::cout << "Key 'left' pressed" << std::endl;
        if (checkMovementPossible()) {
            blockPos[0].x--; blockPos[1].x--; blockPos[2].x--; blockPos[3].x--;
        }
    }
    if (rightPressed == true) {
        checkMovementPossible();
        std::cout << "Key 'right' pressed" << std::endl;
        if (checkMovementPossible()) {
            blockPos[0].x++; blockPos[1].x++; blockPos[2].x++; blockPos[3].x++;
        }
    }
    if (rotatePressed == true) {
        rotatePressed = false;
        std::cout << "Key 'rotate' pressed" << std::endl;
    }
    leftPressed = rightPressed = upPressed = downPressed = false;

    rendering();

    // Setting current position of object in grid to 1 (Dynamic)
    tileArray[blockPos[0].x][blockPos[0].y] = 1;
    tileArray[blockPos[1].x][blockPos[1].y] = 1;
    tileArray[blockPos[2].x][blockPos[2].y] = 1;
    tileArray[blockPos[3].x][blockPos[3].y] = 1;

    collisionDetection();
    checkAndClearFullRows();
    if (collisionDetection()) {
        //checkFullRow();
    }
    //logging();
}

void gameOver() 
{
    gameRunning = false;
    restart = true;
    std::cout << "------- " << "Game Over" << std::endl;

    // Display Game Over Menu
    sf::Sprite gameOverSprite;
    gameOverSprite.setTexture(tGameOverMenu); 
    gameOverSprite.setPosition(0, 0);
    window.draw(gameOverSprite);
    window.display();

    sf::Clock clock;
    while (!gameRunning) {
        // Calling update based on updateSpeed float veriable
        sf::Time elapsedTime = clock.getElapsedTime();
        if (elapsedTime >= sf::milliseconds(restartDelay)) {
            elapsedTime = sf::milliseconds(0.0f);
            clock.restart();
            std::cout << "Restarting Game..." << std::endl;
            // Clearing entire board
            window.clear();
            for (unsigned char y = 0; y < rows; y++) {
                for (unsigned char x = 0; x < columns; x++)
                {
                    tileArray[x][y] = 0;
                }
            }
            window.display();
            main();
        }
    }
}

void logging() 
{
    std::cout << "x: " << blockPos[0].x << " " << blockPos[1].x << " " << blockPos[2].x << " " << blockPos[3].x << " " << std::endl;
    std::cout << "y: " << blockPos[0].y << " " << blockPos[1].y << " " << blockPos[2].y << " " << blockPos[3].y << " " << std::endl;

    if (collisionDetection() == true) {
        std::cout << "CollissionDetection: " << "True" << std::endl;
    }
    if (collisionDetection() == false) {
        std::cout << "CollissionDetection: " << "False" << std::endl;
    }
}