#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include "../include/point.h"
#include "../include/direction.h"
#include "../include/gate.h"

using namespace std;

class Snake
{
public:
    Snake(GateManager &gateManager);
    void Move(Direction dir);
    bool IsCollision(Point p);
    bool IsItem(Point p);
    bool IsGate(Point p);
    void Grow();
    void Shrink();
    Point GetHead() const;
    const vector<Point> &GetBody() const;
    void SetHead(Point newHead);
    void HandleGate(Direction& dir);

    vector<Point> body;
    GateManager &gateManager;
    bool growth = false;
};

#endif