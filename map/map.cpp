#include "../include/map.h"
#include <curses.h>

GameMap::GameMap(int width, int height) : width(width), height(height) {}

void GameMap::Draw() {
    // Draw boundary
    for (int i = 0; i < width + 2; ++i)
        mvprintw(0, i, "#");
    for (int i = 0; i < width + 2; ++i)
        mvprintw(height + 1, i, "#");
    for (int i = 1; i < height + 1; ++i) {
        mvprintw(i, 0, "#");
        mvprintw(i, width + 1, "#");
    }
}

bool GameMap::IsWall(Point p) {
    return p.x <= 0 || p.x >= width + 1 || p.y <= 0 || p.y >= height + 1;
}