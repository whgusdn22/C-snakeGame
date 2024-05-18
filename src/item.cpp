#include "../include/item.h"
#include <cstdlib>
#include <curses.h>

void ItemManager::SpawnItems(int width, int height) {
    growthItems.clear();
    poisonItems.clear();
    for (int i = 0; i < 3; ++i) {
        Point growth(rand() % width, rand() % height);
        growthItems.push_back(growth);
        Point poison(rand() % width, rand() % height);
        poisonItems.push_back(poison);
    }
}

bool ItemManager::IsItem(Point p) {
    for (const auto& item : growthItems) {
        if (item.x == p.x && item.y == p.y) return true;
    }
    for (const auto& item : poisonItems) {
        if (item.x == p.x && item.y == p.y) return true;
    }
    return false;
}

void ItemManager::RemoveItem(Point p) {
    // Implement item removal logic
}

void ItemManager::Draw() {
    for (const auto& item : growthItems) {
        mvprintw(item.y + 1, item.x + 1, "G");
    }
    for (const auto& item : poisonItems) {
        mvprintw(item.y + 1, item.x + 1, "P");
    }
}
