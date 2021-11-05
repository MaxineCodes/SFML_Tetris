#include "Board.h"
#include "main.h"
#include "BlockyShape.h"

const unsigned int rows = 20;
const unsigned int columns = 10;

// 10 = columns | 20 = rows
unsigned int StaticTileArray[rows][columns];

void storeStatic()
{
    for (unsigned int y = 0; y < rows; y++) {
        for (unsigned int x = 0; x < columns; x++) {
            if (StaticTileArray[x][y] == 0)
            {

            }
            if (StaticTileArray[x][y] == 1)
            {

            }
            else {
                //std::cout << tileArray[x][y] << std::endl;
            }
        }
    }
}



