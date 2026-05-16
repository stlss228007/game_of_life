#include "Game/Game.h"

int main() {
    const int GRID_WIDTH = 150;
    const int GRID_HEIGHT = 120;
    const int CELL_SIZE = 6;
    Game game(GRID_WIDTH, GRID_HEIGHT, CELL_SIZE);
    game.run();
    return 0;
}