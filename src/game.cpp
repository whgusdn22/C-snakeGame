#include "../include/gate.h"
#include <cstdlib>
#include <ctime>
#include <vector>
#include <ncurses.h>

void GateManager::SpawnGates(int width, int height) {
    std::vector<Point> wallPositions;

    // Collect all wall positions (where map value is '1')
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (gameMap.GetMap()[y][x] == '1') {
                wallPositions.push_back(Point(x, y));
            }
        }
    }

    if (wallPositions.size() >= 2) {
        int idxA = rand() % wallPositions.size();
        gateA = wallPositions[idxA];
        wallPositions.erase(wallPositions.begin() + idxA);

        int idxB = rand() % wallPositions.size();
        gateB = wallPositions[idxB];
    }
}

bool GateManager::IsGate(Point p) {
    return (p.x == gateA.x && p.y == gateA.y) || (p.x == gateB.x && p.y == gateB.y);
}

Point GateManager::GetOtherGate(Point p, Direction& dir) {
    if (p.x == gateA.x && p.y == gateA.y) {
        adjustDirection(gateB, dir);
        return gateB;
    } else {
        adjustDirection(gateA, dir);
        return gateA;
    }
}

void GateManager::adjustDirection(Point gate, Direction& dir) {
    if (gate.x == 0) {
        dir = Direction::RIGHT;
    } else if (gate.x == gameMap.getWidth() - 1) {
        dir = Direction::LEFT;
    } else if (gate.y == 0) {
        dir = Direction::DOWN;
    } else if (gate.y == gameMap.getHeight() - 1) {
        dir = Direction::UP;
    }
}

void GateManager::Draw() {
    mvprintw(gateA.y + 1, gateA.x + 1, "A");
    mvprintw(gateB.y + 1, gateB.x + 1, "B");
}
