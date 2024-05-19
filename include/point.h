#ifndef POINT_H
#define POINT_H

struct Point {
    int x, y;
    Point(int col, int row) : x(col), y(row) {}
    Point() : x(0), y(0) {}
};

#endif // POINT_H
