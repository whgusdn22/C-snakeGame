#ifndef GATE_H
#define GATE_H

#include "../include/point.h"
#include "map.h" // Include GameMap header
#include "direction.h"

class GateManager {
public:
    GateManager(GameMap& gameMap) : gameMap(gameMap) {} // Constructor to accept GameMap reference
    void SpawnGates(int width, int height);
    bool IsGate(Point p);
    Point GetOtherGate(Point p, Direction& newDir);
    void Draw();
    Point gateA;
    Point gateB;
    GameMap& gameMap; // Reference to GameMap
};

#endif // GATE_H
