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

Direction GateManager::GetValidDirection(Point gate, Direction preferredDir) {
    Point newPos = gate;

    switch (preferredDir) {
        case UP:    newPos.y--; break;
        case DOWN:  newPos.y++; break;
        case LEFT:  newPos.x--; break;
        case RIGHT: newPos.x++; break;
        default: break;
    }

    // Check if preferred direction is valid
    if (newPos.x >= 0 && newPos.x < gameMap.getWidth() && newPos.y >= 0 && newPos.y < gameMap.getHeight() &&
        gameMap.GetMap()[newPos.y][newPos.x] != '1') {
        return preferredDir;
    }

    // If preferred direction is blocked, find an alternative
    switch (preferredDir) {
        case UP:
            if (gameMap.GetMap()[gate.y][gate.x - 1] != '1') return LEFT;
            if (gameMap.GetMap()[gate.y][gate.x + 1] != '1') return RIGHT;
            if (gameMap.GetMap()[gate.y - 1][gate.x] != '1') return UP;
            if (gameMap.GetMap()[gate.y + 1][gate.x] != '1') return DOWN;
            break;
        case DOWN:
            if (gameMap.GetMap()[gate.y][gate.x - 1] != '1') return LEFT;
            if (gameMap.GetMap()[gate.y][gate.x + 1] != '1') return RIGHT;
            if (gameMap.GetMap()[gate.y + 1][gate.x] != '1') return DOWN;
            if (gameMap.GetMap()[gate.y - 1][gate.x] != '1') return UP;
            break;
        case LEFT:
            if (gameMap.GetMap()[gate.y - 1][gate.x] != '1') return DOWN;
            if (gameMap.GetMap()[gate.y + 1][gate.x] != '1') return UP;
            if (gameMap.GetMap()[gate.y][gate.x - 1] != '1') return LEFT;
            if (gameMap.GetMap()[gate.y][gate.x + 1] != '1') return RIGHT;
            break;
        case RIGHT:
            if (gameMap.GetMap()[gate.y - 1][gate.x] != '1') return UP;
            if (gameMap.GetMap()[gate.y + 1][gate.x] != '1') return DOWN;
            if (gameMap.GetMap()[gate.y][gate.x + 1] != '1') return RIGHT;
            if (gameMap.GetMap()[gate.y][gate.x - 1] != '1') return LEFT;
            break;
        default:
            break;
    }

    // Default to preferred direction if no alternatives
    return preferredDir;
}

Point GateManager::GetOtherGate(Point p, Direction& newDir) {
    Point otherGate = (p.x == gateA.x && p.y == gateA.y) ? gateB : gateA;

    int mapHeight = gameMap.getHeight();
    int mapWidth = gameMap.getWidth();

    // 게이트 위치와 현재 방향에 따라 새로운 방향 설정
    if (otherGate.y == 0) { // 나오는 게이트가 최상단에 있을 경우
        newDir = GetValidDirection(otherGate, DOWN);
    } else if (otherGate.y == mapHeight - 1) { // 나오는 게이트가 최하단에 있을 경우
        newDir = GetValidDirection(otherGate, UP);
    } else if (otherGate.x == 0) { // 나오는 게이트가 가장 왼쪽에 있을 경우
        newDir = GetValidDirection(otherGate, RIGHT);
    } else if (otherGate.x == mapWidth - 1) { // 나오는 게이트가 가장 오른쪽에 있을 경우
        newDir = GetValidDirection(otherGate, LEFT);
    } else { // 일반적인 경우
        // 진출 가능 방향이 좌우일 때
        
        if (newDir == UP) {
            newDir = GetValidDirection(otherGate, RIGHT);
        } else if (newDir == LEFT) {
            newDir = GetValidDirection(otherGate, LEFT);
        } else if (newDir == RIGHT) {
            newDir = GetValidDirection(otherGate, RIGHT);
        } else {
            newDir = GetValidDirection(otherGate, DOWN);
        } 
        
    }

    return otherGate;
}

void GateManager::Draw() {
    mvprintw(gateA.y + 1, gateA.x + 1, "A");
    mvprintw(gateB.y + 1, gateB.x + 1, "B");
}
