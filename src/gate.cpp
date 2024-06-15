#include "../include/gate.h"
#include <cstdlib>
#include <ctime>
#include <vector>
#include <curses.h>

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

bool GateManager::IsGatePosition(Point p) {
    return (p.x == gateA.x && p.y == gateA.y) || (p.x == gateB.x && p.y == gateB.y);
}

Point GateManager::GetOtherGate(Point p, Direction& newDir) {
    Point otherGate = (p.x == gateA.x && p.y == gateA.y) ? gateB : gateA;

    int mapHeight = gameMap.getHeight();
    int mapWidth = gameMap.getWidth();

    // 게이트 위치와 현재 방향에 따라 새로운 방향 설정
    if (otherGate.y == 0) { // 나오는 게이트가 최상단에 있을 경우
        newDir = DOWN;
    } else if (otherGate.y == mapHeight - 1) { // 나오는 게이트가 최하단에 있을 경우
        newDir = UP;
    } else if (otherGate.x == 0) { // 나오는 게이트가 가장 왼쪽에 있을 경우
        newDir = RIGHT;
    } else if (otherGate.x == mapWidth - 1) { // 나오는 게이트가 가장 오른쪽에 있을 경우
        newDir = LEFT;
    } else if (gateA.y == gateB.y) { // 게이트가 같은 y 좌표에 있을 경우
        if (p.x < otherGate.x) {
            newDir = RIGHT;
        } else {
            newDir = LEFT;
        }
    } else if (gateA.x == gateB.x) { // 게이트가 같은 x 좌표에 있을 경우
        if (p.y < otherGate.y) {
            newDir = DOWN;
        } else {
            newDir = UP;
        }
    } else { // 게이트가 서로 다른 x, y 좌표에 있을 경우
        if (abs(p.x - otherGate.x) > abs(p.y - otherGate.y)) {
            if (p.x < otherGate.x) {
                newDir = RIGHT;
            } else {
                newDir = LEFT;
            }
        } else {
            if (p.y < otherGate.y) {
                newDir = DOWN;
            } else {
                newDir = UP;
            }
        }
    }

    return otherGate;
}

void GateManager::Draw() {
    mvprintw(gateA.y + 1, gateA.x + 1, "A");
    mvprintw(gateB.y + 1, gateB.x + 1, "B");
}
