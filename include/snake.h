#ifndef SNAKE_H
#define SNAKE_H

#include "point.h"
#include "direction.h"
#include "gate.h"
#include <vector>

class Snake
{
public:
    Snake(GateManager &gateManager);
    void Move(Direction dir);
    bool IsCollision(Point p);
    bool IsGate(Point p);
    void Grow(vector<Point> &p);
    void Shrink();
    Point GetHead() const;
    const std::vector<Point> &GetBody();
    void SetHead(Point newHead);
    void HandleGate(Direction &dir);
    std::vector<Point> body;
    GateManager &gateManager;
};

#endif // SNAKE_H
