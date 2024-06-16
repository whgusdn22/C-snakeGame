#ifndef POINT_H
#define POINT_H

struct Point
{
    int x, y;
    Point(int col, int row) : x(col), y(row) {}
    Point() : x(0), y(0) {}

    // == 연산자 구현
    bool operator==(const Point &other) const
    {
        return x == other.x && y == other.y;
    }

    // != 연산자 구현
    bool operator!=(const Point &other) const
    {
        return !(*this == other);
    }

    // + 연산자 구현
    Point operator+(const Point &other) const
    {
        return Point(x + other.x, y + other.y);
    }

    // - 연산자 구현
    Point operator-(const Point &other) const
    {
        return Point(x - other.x, y - other.y);
    }
};

#endif // POINT_H
