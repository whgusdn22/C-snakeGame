#ifndef ITEM_H
#define ITEM_H

#include <vector>
#include "../include/point.h"

class ItemManager {
public:
    void SpawnItems(int width, int height);
    bool IsItem(Point p);
    void RemoveItem(Point p);
    void Draw();

private:
    std::vector<Point> growthItems;
    std::vector<Point> poisonItems;
};

#endif // ITEM_H
