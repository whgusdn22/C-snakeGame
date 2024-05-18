#ifndef MAP_H
#define MAP_H

#include <vector>
#include <string>
#include "../include/point.h"

class GameMap {
public:
    GameMap(const std::vector<std::string>& mapData);
    void Draw() const;
    bool IsWall(Point p);
    void ChangeMap(const std::vector<std::string>& newMapData);
    int getWidth() const;
    int getHeight() const;
    const std::vector<std::string>& GetMap() const;

    std::vector<std::string> mapData;
    int width;
    int height;
};

#endif // MAP_H
