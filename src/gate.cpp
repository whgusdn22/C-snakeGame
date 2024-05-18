#include "../include/gate.h"
#include <cstdlib>
#include <curses.h>

void GateManager::SpawnGates(int width, int height) {
    gateA = Point(rand() % width, rand() % height);
    gateB = Point(rand() % width, rand() % height);
}

bool GateManager::IsGate(Point p) {
    return (p.x == gateA.x && p.y == gateA.y) || (p.x == gateB.x && p.y == gateB.y);
}

Point GateManager::GetOtherGate(Point p) {
    if (p.x == gateA.x && p.y == gateA.y) {
        return gateB;
    } else {
        return gateA;
    }
}

void GateManager::Draw() {
    mvprintw(gateA.y + 1, gateA.x + 1, "A");
    mvprintw(gateB.y + 1, gateB.x + 1, "B");
}
