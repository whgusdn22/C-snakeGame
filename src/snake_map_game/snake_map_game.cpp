#include "../../include/snake_map_game/snake_map_game.h"
#include <ncurses.h>
#include <vector>
#include <iostream>

Snake_map_game::Snake_map_game()
{
    move(0, 5);
    printw("Snake Game by MinWoo, SinJi, DongHyeon");
}

Snake_map_game::~Snake_map_game()
{
    endwin();
}

void Snake_map_game::Snake_stage_init(std::vector<std::string> stage){
    for (int y = 0; y < 33; y++)
    {
        for (int x = 0; x < 61; x++)
        {
            mvaddch(y + 3, x + 10, stage[y][x]);
        }
    }
    refresh();
}