#ifndef GATE_H
#define GATE_H

#include "../include/point.h"
#include <vector>

class GateManager {
public:
    void SpawnGates(int width, int height);
    bool IsGate(Point p);
    Point GetOtherGate(Point p);
    void Draw();
    Point gateA;
    Point gateB;
    vector<Point> walls;
};

#endif // GATE_H
