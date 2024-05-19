#include "../include/map.h"
#include <curses.h>

GameMap::GameMap(const std::vector<std::string>& mapData) : mapData(mapData) {
    height = mapData.size();
    width = mapData[0].size();
}

void GameMap::Draw() const {
    for (int y = 0; y < height; ++y) {
    mvprintw(y + 1, 1, "%s", mapData[y].c_str());
    }
}

bool GameMap::IsWall(Point p) {
    return mapData[p.y][p.x] == '1';
}

void GameMap::ChangeMap(const std::vector<std::string>& newMapData) {
    mapData = newMapData;
    height = mapData.size();
    width = mapData[0].size();
}

int GameMap::getWidth() const {
    return width;
}

int GameMap::getHeight() const {
    return height;
}

const std::vector<std::string>& GameMap::GetMap() const { // Implement GetMap function
    return mapData;
}
