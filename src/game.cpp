#include "../include/game.h"
#include "../UI/scoreboard.h"
#include "../include/map/map1.h"
#include "../include/map/map2.h"
#include "../include/map/map3.h"
#include "../include/map/map4.h"
#include <cstdlib>
#include <ctime>
#include <curses.h>
#include "../include/direction.h"
#include <chrono>
const int midx = 61 / 2;
const int midy = 22 / 2;
void SnakeGame::MoveCenter()
{
    snake.body.clear(); // snake vector를 {}로 초기화시킴

    snake.body.push_back(Point(midx, midy)); // snake vector에 맵 가운데 좌표를 넣어줌
    for (int i = 1; i < 4; ++i)
    {
        snake.body.push_back(Point(midx - i, midy));
    }
}
SnakeGame::SnakeGame()
    : gameMap(std::vector<std::string>(std::begin(map1), std::end(map1))),
      gateManager(gameMap),
      snake(gateManager), // Initialize snake with gateManager
      lastMoveTime(std::chrono::steady_clock::now())
{ // Initialize lastMoveTime
    score = 0;
    maxLength = 0;
    growthCount = 0;
    poisonCount = 0;
    gateCount = 0;
    gameOver = false;
    dir = RIGHT;
    tick = 200; // ms
    Initialize();
}

SnakeGame::~SnakeGame()
{
    endwin(); // End ncurses mode
}

void SnakeGame::Initialize()
{
    initscr(); // Initialize ncurses mode
    clear();
    noecho();
    cbreak();
    curs_set(0);
    keypad(stdscr, TRUE);
    timeout(tick);

    srand(time(0));

    // Spawn items and gates
    itemManager.SpawnItems(gameMap.getWidth(), gameMap.getHeight(), gameMap);
    gateManager.SpawnGates(gameMap.getWidth(), gameMap.getHeight());
}

void SnakeGame::Draw()
{
    clear();
    gameMap.Draw();
    itemManager.Draw();
    gateManager.Draw();
    const auto &snakeBody = snake.GetBody();
    for (size_t i = 0; i < snakeBody.size(); ++i)
    {
        mvprintw(snakeBody[i].y + 1, snakeBody[i].x + 1, i == 0 ? "@" : "o");
    }
    DrawScoreBoard(score, snakeBody.size(), maxLength, growthCount, poisonCount, gateCount, gameMap.getWidth());
    refresh();
}

void SnakeGame::Input()
{
    int ch = getch();
    switch (ch)
    {
    case KEY_LEFT:
        if (dir != RIGHT)
            dir = LEFT;
        break;
    case KEY_RIGHT:
        if (dir != LEFT)
            dir = RIGHT;
        break;
    case KEY_UP:
        if (dir != DOWN)
            dir = UP;
        break;
    case KEY_DOWN:
        if (dir != UP)
            dir = DOWN;
        break;
    case 'q':
        gameOver = true;
        break;
    }
}

void SnakeGame::Logic()
{
    auto now = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastMoveTime).count();

    if (duration >= tick)
    {
        snake.Move(dir);
        lastMoveTime = now;

        if (gameMap.IsWall(snake.GetHead()) || snake.IsCollision(snake.GetHead()))
        {
            gameOver = true;
        }

        // Handle item collision
        if (itemManager.IsItem(snake.GetHead()))
        {
            if (itemManager.IsGrowthItem(snake.GetHead()))
            {
                score += 50;
                growthCount++;
                snake.Grow();
            }
            else
            {
                score -= 10;
                poisonCount++;
                snake.Shrink();
                if (snake.GetBody().size() < 3)
                {
                    gameOver = true;
                }
            }
            itemManager.RemoveItem(snake.GetHead());
        }

        if (snake.IsGate(snake.GetHead()))
        {
            Point newHead = gateManager.GetOtherGate(snake.GetHead());
            snake.SetHead(newHead);
            gateCount++;
            score += 20; // 게이트 사용 시 추가 점수
        }

        // Update score and max length
        if (snake.GetBody().size() > maxLength)
        {
            maxLength = snake.GetBody().size();
        }

        // Change map based on score
        if (score % 100 == 0 and score != SnakeGame::scores.back())
        {
            SnakeGame::scores.push_back(score);
            if ((score / 100) % 4 == 1)
            {
                // snake.body.clear();

                // snake.body.push_back(Point(midx, midy));
                // for (int i = 1; i < 4; ++i)
                // {
                //     snake.body.push_back(Point(midx - i, midy));
                // }
                SnakeGame::MoveCenter();
                gameMap.ChangeMap(std::vector<std::string>(std::begin(map2), std::end(map2)));
            }
            else if ((score / 100) % 4 == 2)
            {
                SnakeGame::MoveCenter();
                gameMap.ChangeMap(std::vector<std::string>(std::begin(map3), std::end(map3)));
            }
            else if ((score / 100) % 4 == 3)
            {
                SnakeGame::MoveCenter();
                gameMap.ChangeMap(std::vector<std::string>(std::begin(map4), std::end(map4)));
            }
            else if ((score / 100) % 4 == 0)
            {
                SnakeGame::MoveCenter();
                gameMap.ChangeMap(std::vector<std::string>(std::begin(map1), std::end(map1)));
            }
        }

        // Respawn items if necessary
        if (itemManager.ItemsDepleted())
        {
            itemManager.SpawnItems(gameMap.getWidth(), gameMap.getHeight(), gameMap);
        }
    }
}

void SnakeGame::Run()
{
    while (!gameOver)
    {
        Draw();
        Input();
        Logic();
    }
    mvprintw(gameMap.getHeight() / 2, (gameMap.getWidth() - 9) / 2, "Game Over");
    refresh();
    getch();
}
