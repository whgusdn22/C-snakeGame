#ifndef GATE_H
#define GATE_H

#include "../include/point.h"

class GateManager {
public:
    void SpawnGates(int width, int height);
    bool IsGate(Point p);
    Point GetOtherGate(Point p);
    void Draw();

private:
    Point gateA;
    Point gateB;
};

#endif // GATE_H
