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
    currentDirection = Direction::RIGHT; // Initialize default direction
}

void Snake::Move(Direction dir) {  // Use Direction from direction.h
    Point prev = body[0];
    Point prev2;

    // Check if the head is at a gate
    if (gateManager.IsGate(prev)) {
        Point newHead = gateManager.GetOtherGate(prev, dir); // Update direction if passing through a gate
        SetHead(newHead);
        SetDirection(dir);
    } else {
        switch (dir) {
            case Direction::LEFT: --body[0].x; break;
            case Direction::RIGHT: ++body[0].x; break;
            case Direction::UP: --body[0].y; break;
            case Direction::DOWN: ++body[0].y; break;
            default: ++body[0].x; break;
        }
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

Direction Snake::GetDirection() const {
    return currentDirection;
}

void Snake::SetDirection(Direction dir) {
    currentDirection = dir;
}
