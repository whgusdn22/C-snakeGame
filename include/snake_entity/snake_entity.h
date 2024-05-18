#include <ncurses.h>
#include <vector>
#ifndef SNAKEENTITY_H
#define SNAKEENTITY_H

struct map_position
{
private:
    int x, y;
public:
    map_position(int x, int y);
    std::pair<int, int> get_position();
};

class Snake_entity
{
private:
    std::vector<map_position> snake;
    char dir = 'l';
public:
    Snake_entity();
    void move_snake();
    void set_snake();
    void loop_snake();
};

#endif