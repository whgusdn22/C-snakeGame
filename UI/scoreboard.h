#include <curses.h>
#include "../include/game.h"

void DrawScoreBoard(int score, int snakeLength, int maxLength, int growthCount, int poisonCount, int gateCount, int width) {
    mvprintw(0, width + 4, "Score: %d", score);
    mvprintw(1, width + 4, "B: %d / %d", snakeLength, maxLength);
    mvprintw(2, width + 4, "+: %d", growthCount);
    mvprintw(3, width + 4, "-: %d", poisonCount);
    mvprintw(4, width + 4, "G: %d", gateCount);
}