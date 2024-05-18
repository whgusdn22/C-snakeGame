#include "../include/map.h"
#include <curses.h>

GameMap::GameMap(const std::vector<std::string>& mapData) : mapData(mapData), height(mapData.size()), width(mapData[0].length()) {}

void GameMap::Draw() {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            mvprintw(y + 1, x + 1, "%c", mapData[y][x]);
        }
    }
}

bool GameMap::IsWall(Point p) {
    return mapData[p.y][p.x] == '1';
}

void GameMap::ChangeMap(const std::vector<std::string>& newMapData) {
    mapData = newMapData;
    height = mapData.size();
    width = mapData[0].length();
}

int GameMap::getWidth() const {
    return width;
}

int GameMap::getHeight() const {
    return height;
}
