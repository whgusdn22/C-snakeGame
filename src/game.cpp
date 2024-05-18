#include "game.h"
#include "../UI/scoreboard.h"
#include "../include/map/map1.h"
#include "../include/map/map2.h"
#include "../include/map/map3.h"
#include "../include/map/map4.h"
#include <cstdlib>
#include <ctime>
#include <curses.h>
#include "../include/direction.h"

SnakeGame::SnakeGame() : gameMap(std::vector<std::string>(std::begin(map1), std::end(map1))), snake(gateManager) {
    score = 0;
    maxLength = 0;
    growthCount = 0;
    poisonCount = 0;
    gateCount = 0;
    gameOver = false;
    dir = STOP;
    tick = 500; // ms
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

    // Spawn items and gates
    itemManager.SpawnItems(gameMap.getWidth(), gameMap.getHeight());
    gateManager.SpawnGates(gameMap.getWidth(), gameMap.getHeight());
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
    DrawScoreBoard(score, snakeBody.size(), maxLength, growthCount, poisonCount, gateCount, gameMap.getWidth());
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

    // Handle item collision
    if (itemManager.IsItem(snake.GetHead())) {
        if (itemManager.IsGrowthItem(snake.GetHead())) {
            score += 50;
            growthCount++;
            snake.Grow();
        } else {
            score -= 10;
            poisonCount++;
            snake.Shrink();
            if (snake.GetBody().size() < 3) {
                gameOver = true;
            }
        }
        itemManager.RemoveItem(snake.GetHead());
    }

    if (snake.IsGate(snake.GetHead())) {
        Point newHead = gateManager.GetOtherGate(snake.GetHead());
        snake.SetHead(newHead);
        gateCount++;
        score += 20; // 게이트 사용 시 추가 점수
    }

    // Update score and max length
    if (snake.GetBody().size() > maxLength) {
        maxLength = snake.GetBody().size();
    }

    // Change map based on score
    if (score >= 300) {
        gameMap.ChangeMap(std::vector<std::string>(std::begin(map4), std::end(map4)));
    } else if (score >= 200) {
        gameMap.ChangeMap(std::vector<std::string>(std::begin(map3), std::end(map3)));
    } else if (score >= 100) {
        gameMap.ChangeMap(std::vector<std::string>(std::begin(map2), std::end(map2)));
    } else {
        gameMap.ChangeMap(std::vector<std::string>(std::begin(map1), std::end(map1)));
    }

    // Respawn items if necessary
    if (itemManager.ItemsDepleted()) {
        itemManager.SpawnItems(gameMap.getWidth(), gameMap.getHeight());
    }
}

void SnakeGame::Run() {
    while (!gameOver) {
        Draw();
        Input();
        Logic();
    }
    mvprintw(gameMap.getHeight() / 2, (gameMap.getWidth() - 9) / 2, "Game Over");
    refresh();
    getch();
}