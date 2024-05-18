#ifndef MAP_H
#define MAP_H

#include <vector>
#include "../include/point.h"

class GameMap {
public:
    GameMap(int width, int height);
    void Draw();
    bool IsWall(Point p);
    int width;
    int height;
};

#endif // MAP_H
