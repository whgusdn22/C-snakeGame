#include "../include/item.h"
#include <cstdlib>
#include <curses.h>
#include "map.h"


void ItemManager::SpawnItems(int width, int height, const GameMap& gameMap) {
    growthItems.clear();
    poisonItems.clear();

    auto is_valid_position = [&gameMap](Point p) {
        char cell = gameMap.GetMap()[p.y][p.x];
        return cell != '1' && cell != '2';
    };

    for (int i = 0; i < 3; ++i) {
        Point growth, poison;
        do {
            growth = Point(rand() % width, rand() % height);
        } while (!is_valid_position(growth));
        growthItems.push_back(growth);

        do {
            poison = Point(rand() % width, rand() % height);
        } while (!is_valid_position(poison));
        poisonItems.push_back(poison);
    }
}

bool ItemManager::IsItem(Point p)
{
    for (const auto &item : growthItems)
    {
        if (item.x == p.x && item.y == p.y)
            return true;
    }
    for (const auto &item : poisonItems)
    {
        if (item.x == p.x && item.y == p.y)
            return true;
    }
    return false;
}

bool ItemManager::IsGrowthItem(Point p)
{
    for (const auto &item : growthItems)
    {
        if (item.x == p.x && item.y == p.y)
            return true;
    }
    return false;
}

void ItemManager::RemoveItem(Point p)
{
    for (auto it = growthItems.begin(); it != growthItems.end(); ++it)
    {
        if (it->x == p.x && it->y == p.y)
        {
            growthItems.erase(it);
            return;
        }
    }
    for (auto it = poisonItems.begin(); it != poisonItems.end(); ++it)
    {
        if (it->x == p.x && it->y == p.y)
        {
            poisonItems.erase(it);
            return;
        }
    }
}

void ItemManager::Draw()
{
    for (const auto &item : growthItems)
    {
        mvprintw(item.y + 1, item.x + 1, "G");
    }
    for (const auto &item : poisonItems)
    {
        mvprintw(item.y + 1, item.x + 1, "P");
    }
}

bool ItemManager::ItemsDepleted()
{
    return growthItems.empty() || poisonItems.empty();
}
