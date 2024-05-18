#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include "../include/point.h"
#include "../include/direction.h"


class Snake {
public:
    Snake();
    void Move(Direction dir);
    bool IsCollision(Point p);
    bool IsItem(Point p);
    bool IsGate(Point p);
    void Grow();
    void Shrink();
    Point GetHead() const;
    const std::vector<Point>& GetBody() const;
    void SetHead(Point newHead);

private:
    std::vector<Point> body;
};

#endif