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
    bool IsGatePosition(Point p); // 추가
    Point GetOtherGate(Point p, Direction& newDir);
    Direction GetValidDirection(Point gate, Direction preferredDir); // 추가
    void Draw();
    Point gateA;
    Point gateB;
    GameMap& gameMap; // Reference to GameMap
};

#endif // GATE_H
