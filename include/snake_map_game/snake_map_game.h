#include <iostream>
#include <vector>
#include <ncurses.h>
#include "../snake_map/snake_map.h"
#ifndef SNAKEMAP_GAME_H
#define SNAKEMAP_GAME_H

class Snake_map_game : protected Snake_map
{
protected:
    int startx = 3, starty = 1, width = 65, height = 35;
    const char** map;

public:
    Snake_map_game();
    ~Snake_map_game();
    void Snake_stage_init(std::vector<std::string> stage);
};

#endif