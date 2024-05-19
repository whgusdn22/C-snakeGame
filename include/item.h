#ifndef ITEM_H
#define ITEM_H

#include <vector>
#include "../include/point.h"

class ItemManager {
public:
    void SpawnItems(int width, int height);
    bool IsItem(Point p);
    bool IsGrowthItem(Point p);
    void RemoveItem(Point p);
    void Draw();
    bool ItemsDepleted();

private:
    std::vector<Point> growthItems;
    std::vector<Point> poisonItems;
};

#endif // ITEM_H
