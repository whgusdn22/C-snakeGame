#include "../include/snake.h"
#include "../include/point.h"
#include "../include/game.h"
#include "../include/direction.h"
#include "../include/gate.h"
#include "snake.h"

using namespace std;

Snake::Snake(GateManager &gateManager) : gateManager(gateManager)
{
    body.push_back(Point(30, 11));
    for (int i = 1; i < 4; ++i)
    {
        body.push_back(Point(30 - i, 11));
    }
}

void Snake::Move(Direction dir)
{
    Point prev = body[0];
    Point prev2;
    switch (dir)
    {
    case LEFT:
        --body[0].x;
        break;
    case RIGHT:
        ++body[0].x;
        break;
    case UP:
        --body[0].y;
        break;
    case DOWN:
        ++body[0].y;
        break;
    }

    for (size_t i = 1; i < body.size(); ++i)
    {
        prev2 = body[i];
        body[i] = prev;
        prev = prev2;
    }
}

bool Snake::IsCollision(Point p)
{
    for (size_t i = 1; i < body.size(); ++i)
    {
        if (body[i].x == p.x && body[i].y == p.y)
            return true;
    }
    return false;
}

bool Snake::IsGate(Point p)
{
    return (p.x == gateManager.gateA.x && p.y == gateManager.gateA.y) ||
           (p.x == gateManager.gateB.x && p.y == gateManager.gateB.y);
}

void Snake::Grow(vector<Point> &p)
{
    // Add a new segment at the position of the last segment
    p.push_back(Point());
    p.back() = (p.back() + Point(1, 0));
}

void Snake::Shrink()
{
    body.pop_back();
}

Point Snake::GetHead() const
{
    return body[0];
}

const std::vector<Point> &Snake::GetBody()
{
    return body;
}

void Snake::SetHead(Point newHead)
{
    body[0] = newHead;
}

void Snake::HandleGate(Direction &dir)
{
    if (IsGate(GetHead()))
    {
        Point newHead = gateManager.GetOtherGate(GetHead(), dir);
        SetHead(newHead);
        if (body.size() > 1)
        {
            body[1] = newHead; // 두 번째 몸통을 새 머리 위치로 설정
        }
    }
}
