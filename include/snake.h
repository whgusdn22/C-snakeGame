#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include "../include/point.h"
#include "../include/direction.h"
#include "../include/gate.h"

class Snake {
public:
    Snake(GateManager& gateManager);
    void Move(Direction dir);
    bool IsCollision(Point p);
    bool IsItem(Point p);
    bool IsGate(Point p);
    void Grow();
    void Shrink();
    Point GetHead() const;
    const std::vector<Point>& GetBody() const;
    void SetHead(Point newHead);
    Direction GetDirection() const;
    void SetDirection(Direction dir);

private:
    std::vector<Point> body;
    Direction currentDirection; // To store the current direction of the snake
    GateManager& gateManager;
};

#endif // SNAKE_H
