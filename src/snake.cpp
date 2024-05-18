#include "../include/snake.h"
#include "../include/point.h"
#include "../include/game.h"
#include "../include/direction.h"
#include "../include/gate.h"

using namespace std;

Snake::Snake(GateManager& gateManager) : gateManager(gateManager) {
    body.push_back(Point(10, 10));
    for (int i = 1; i < 4; ++i) {
        body.push_back(Point(10 - i, 10));
    }
}

void Snake::Move(Direction dir) {  // Use Direction from direction.h
    Point prev = body[0];
    Point prev2;
    switch (dir) {
    case LEFT: --body[0].x; break;
    case RIGHT: ++body[0].x; break;
    case UP: --body[0].y; break;
    case DOWN: ++body[0].y; break;
    default: break;
    }
    for (size_t i = 1; i < body.size(); ++i) {
        prev2 = body[i];
        body[i] = prev;
        prev = prev2;
    }
}

bool Snake::IsCollision(Point p) {
    for (size_t i = 1; i < body.size(); ++i) {
        if (body[i].x == p.x && body[i].y == p.y) return true;
    }
    return false;
}

bool Snake::IsItem(Point p) {
    // Implement item collision logic
    return false;
}

bool Snake::IsGate(Point p) {
    return (p.x == gateManager.gateA.x && p.y == gateManager.gateA.y) || 
           (p.x == gateManager.gateB.x && p.y == gateManager.gateB.y);
}

void Snake::Grow() {
    body.push_back(Point());
}

void Snake::Shrink() {
    if (body.size() > 1) {
        body.pop_back();
    }
}

Point Snake::GetHead() const {
    return body[0];
}

const std::vector<Point>& Snake::GetBody() const {
    return body;
}

void Snake::SetHead(Point newHead) {
    body[0] = newHead;
}
