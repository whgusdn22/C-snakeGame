#include <ncurses.h>

const int HEIGHT = 10;
const int WIDTH = 20;
char snake_map[HEIGHT][WIDTH];

void initialize_map() {
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            if (i == 0 || i == HEIGHT - 1 || j == 0 || j == WIDTH - 1) {
                snake_map[i][j] = '#'; 
            } else {
                snake_map[i][j] = ' '; 
            }
        }
    }
}

void draw_map() {
    clear(); 
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            mvaddch(i, j, snake_map[i][j]); 
        }
    }
    refresh(); 
}

int main() {
    initscr(); 
    noecho(); 
    curs_set(0); 

    initialize_map();
    draw_map();

    getch(); 
    endwin(); 

    return 0;
}
