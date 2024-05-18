#include "game.h"
#include "../UI/scoreboard.h"
#include <cstdlib>
#include <ctime>
#include <curses.h>
#include "../include/direction.h"

SnakeGame::SnakeGame() : gameMap(21, 21) {
    score = 0;
    maxLength = 0;
    growthCount = 0;
    poisonCount = 0;
    gateCount = 0;
    gameOver = false;
    dir = STOP;
    tick = 100; // ms
    Initialize();
}

SnakeGame::~SnakeGame() {
    endwin(); // End ncurses mode
}

void SnakeGame::Initialize() {
    initscr(); // Initialize ncurses mode
    clear();
    noecho();
    cbreak();
    curs_set(0);
    keypad(stdscr, TRUE);
    timeout(tick);

    srand(time(0));

    // Initialize snake
    snake = Snake();
    // Spawn items and gates
    itemManager.SpawnItems(gameMap.width, gameMap.height);
    gateManager.SpawnGates(gameMap.width, gameMap.height);
}

void SnakeGame::Draw() {
    clear();
    gameMap.Draw();
    itemManager.Draw();
    gateManager.Draw();
    const auto& snakeBody = snake.GetBody();
    for (size_t i = 0; i < snakeBody.size(); ++i) {
        mvprintw(snakeBody[i].y + 1, snakeBody[i].x + 1, i == 0 ? "@" : "o");
    }
    DrawScoreBoard(score, snakeBody.size(), maxLength, growthCount, poisonCount, gateCount, gameMap.width);
    refresh();
}

void SnakeGame::Input() {
    int ch = getch();
    switch (ch) {
    case KEY_LEFT:
        if (dir != RIGHT) dir = LEFT;
        break;
    case KEY_RIGHT:
        if (dir != LEFT) dir = RIGHT;
        break;
    case KEY_UP:
        if (dir != DOWN) dir = UP;
        break;
    case KEY_DOWN:
        if (dir != UP) dir = DOWN;
        break;
    case 'q':
        gameOver = true;
        break;
    }
}

void SnakeGame::Logic() {
    snake.Move(dir);

    if (gameMap.IsWall(snake.GetHead()) || snake.IsCollision(snake.GetHead())) {
        gameOver = true;
    }

    if (snake.IsItem(snake.GetHead())) {
        // Handle item logic
    }

    if (snake.IsGate(snake.GetHead())) {
        Point newHead = gateManager.GetOtherGate(snake.GetHead());
        snake.SetHead(newHead);
        gateCount++;
    }

    // Update score and max length
    if (snake.GetBody().size() > maxLength) {
        maxLength = snake.GetBody().size();
    }

    // Respawn items if necessary
    // if (itemManager.ItemsDepleted()) {
    //     itemManager.SpawnItems(gameMap.width, gameMap.height);
    // }
}

void SnakeGame::Run() {
    while (!gameOver) {
        Draw();
        Input();
        Logic();
    }
    mvprintw(gameMap.height / 2, (gameMap.width - 9) / 2, "Game Over");
    refresh();
    getch();
}
