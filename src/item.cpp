#include "../include/item.h"
#include <cstdlib>
#include <curses.h>
#include "map.h"
#include <iostream>
#include <vector>
#include <algorithm>

void ItemManager::SpawnItems(int width, int height, const GameMap &gameMap, vector<Point> snakeBody)
{
    growthItems.clear();
    poisonItems.clear();
    speedItems.clear();

    auto is_valid_position = [&gameMap](Point p, vector<Point> snakeBody)
    {
        char cell = gameMap.GetMap()[p.y][p.x];
        if (std::find(snakeBody.begin(), snakeBody.end(), Point(p.x, p.y)) == snakeBody.end())
            return cell == ' ';
        else
            return false;
    };

    for (int i = 0; i < 3; ++i)
    {
        Point growth;
        Point poison;
        do
        {
            growth = Point(rand() % width, rand() % height);
        } while (!is_valid_position(growth, snakeBody));
        growthItems.push_back(growth);
        do
        {
            poison = Point(rand() % width, rand() % height);
        } while (!is_valid_position(poison, snakeBody));
        poisonItems.push_back(poison);
    }
    for (int i = 0; i < 2; ++i)
    {
        Point speed;
        do
        {
            speed = Point(rand() % width, rand() % height);
        } while (!is_valid_position(speed, snakeBody));
        speedItems.push_back(speed);
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
    for (const auto &item : speedItems)
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
bool ItemManager::IsSpeedItem(Point p)
{
    for (const auto &item : speedItems)
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
    for (auto it = speedItems.begin(); it != speedItems.end(); ++it)
    {
        if (it->x == p.x && it->y == p.y)
        {
            speedItems.erase(it);
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
    for (const auto &item : speedItems)
    {
        mvprintw(item.y + 1, item.x + 1, "S");
    }
}

// bool ItemManager::ItemsDepleted()
// {
//     return growthItems.empty() || poisonItems.empty();
// }
