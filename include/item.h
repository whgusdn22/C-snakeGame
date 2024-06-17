#ifndef ITEM_H
#define ITEM_H

#include <vector>
#include "../include/point.h"
#include "map.h"

class ItemManager
{
public:
    void SpawnItems(int width, int height, const GameMap &gameMap);
    bool IsItem(Point p);
    bool IsGrowthItem(Point p);
    bool IsSpeedItem(Point p);
    void RemoveItem(Point p);
    void Draw();
    bool ItemsDepleted();

private:
    std::vector<Point> growthItems;
    std::vector<Point> poisonItems;
    std::vector<Point> speedItems;
};

#endif // ITEM_H
