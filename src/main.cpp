#include "./snake_map_game/snake_map_game.cpp"
#include "../snake_entity/snake_entity.cpp"
#include "../map/stage4.h"
#include <ncurses.h>

int main()
{
    initscr();   // Curses 초기화
    noecho();    // 키 입력을 화면에 표시 x
    curs_set(0); // 커서를 보이지 않게함
    // cbreak();    /* Line buffering disabled, Pass on everty thing to me */
    refresh();
    nodelay(stdscr, true); 
    keypad(stdscr, TRUE);

    Snake_map_game snake_map = Snake_map_game();
    snake_map.Snake_stage_init(stage1);
    getch();

    Snake_entity snake = Snake_entity();
    // getch();
    // Snake_entity();

    // while(getch() == KEY_F(1)){
    //     Snake_map_game snake2 = Snake_map_game(stage2, game_win);
    //     getch();
    // };

    // snake.move_snake();
    // getch();
    snake.loop_snake();
    return 0;
}