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
    if (dir == LEFT)
    {
        for (int i = 1; i < 4; ++i)
        {
            snake.body.push_back(Point(midx + i, midy));
        }
    }
    else if (dir == UP)
    {
        for (int i = 1; i < 4; ++i)
        {
            snake.body.push_back(Point(midx, midy + i));
        }
    }
    else if (dir == DOWN)
    {
        for (int i = 1; i < 4; ++i)
        {
            snake.body.push_back(Point(midx, midy - i));
        }
    }
    else
    {
        for (int i = 1; i < 4; ++i)
        {
            snake.body.push_back(Point(midx - i, midy));
        }
    }
}

SnakeGame::SnakeGame()
    : gameMap(std::vector<std::string>(std::begin(map1), std::end(map1))),
      gateManager(gameMap),
      snake(gateManager), // Initialize snake with gateManager
      lastMoveTime(std::chrono::steady_clock::now()),
      currentStage(1) // Initialize currentStage
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

    // Draw Score Board
    mvprintw(0, gameMap.getWidth() + 2, "Score Board");
    mvprintw(1, gameMap.getWidth() + 2, "B: (%d) / (%d)", snakeBody.size(), maxLength);
    mvprintw(2, gameMap.getWidth() + 2, "+: %d", growthCount);
    mvprintw(3, gameMap.getWidth() + 2, "-: %d", poisonCount);
    mvprintw(4, gameMap.getWidth() + 2, "G: %d", gateCount);

    // Draw Mission
    int missionLength = 10; // Example mission length, you can adjust as needed
    mvprintw(6, gameMap.getWidth() + 2, "Mission");
    mvprintw(7, gameMap.getWidth() + 2, "B: %d (%s)", missionLength, maxLength >= missionLength ? "v" : " ");
    mvprintw(8, gameMap.getWidth() + 2, "+: 5 (%s)", growthCount >= 5 ? "v" : " ");
    mvprintw(9, gameMap.getWidth() + 2, "-: 2 (%s)", poisonCount >= 2 ? "v" : " ");
    mvprintw(10, gameMap.getWidth() + 2, "G: 1 (%s)", gateCount >= 1 ? "v" : " ");

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

bool SnakeGame::CheckMissionComplete()
{
    int missionLength = 10; // Example mission length, you can adjust as needed
    return (maxLength >= missionLength && growthCount >= 5 && poisonCount >= 2 && gateCount >= 1);
}

void SnakeGame::Logic()
{
    auto now = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastMoveTime).count();

    if (duration >= tick)
    {
        snake.Move(dir);
        lastMoveTime = now;

        if ((gameMap.IsWall(snake.GetHead()) && !gateManager.IsGatePosition(snake.GetHead())) || snake.IsCollision(snake.GetHead()))
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

        // Handle gate collision and set new direction
        if (snake.IsGate(snake.GetHead()))
        {
            Direction newDir = dir;
            Point newHead = gateManager.GetOtherGate(snake.GetHead(), newDir);
            snake.SetHead(newHead);
            dir = newDir; // 새로운 방향 설정

            // 충돌 검사
            if ((gameMap.IsWall(snake.GetHead()) && !gateManager.IsGatePosition(snake.GetHead())) || snake.IsCollision(snake.GetHead())) {
                gameOver = true;
            } else {
                gateCount++;
                score += 20; // 게이트 사용 시 추가 점수
            }
        }

        // Update score and max length
        if (snake.GetBody().size() > maxLength)
        {
            maxLength = snake.GetBody().size();
        }

        // Check if all missions are completed
        if (CheckMissionComplete())
        {
            mvprintw(gameMap.getHeight() / 2, (gameMap.getWidth() - 3) / 2, "Win");
            refresh();
            getch();
            gameOver = true;
        }

        // Change map based on score
        if (score >= 100 && currentStage == 1)
        {
            currentStage = 2;
            tick -= 20;
            ChangeMap(std::vector<std::string>(std::begin(map2), std::end(map2)));
        }
        else if (score >= 200 && currentStage == 2)
        {
            currentStage = 3;
            tick -= 10;
            ChangeMap(std::vector<std::string>(std::begin(map3), std::end(map3)));
        }
        else if (score >= 300 && currentStage == 3)
        {
            currentStage = 4;
            tick -= 10;
            ChangeMap(std::vector<std::string>(std::begin(map4), std::end(map4)));
        }

        // Respawn items if necessary
        if (itemManager.ItemsDepleted())
        {
            itemManager.SpawnItems(gameMap.getWidth(), gameMap.getHeight(), gameMap);
        }
    }
}

void SnakeGame::ChangeMap(const std::vector<std::string>& newMap)
{
    gameMap.ChangeMap(newMap);
    itemManager.SpawnItems(gameMap.getWidth(), gameMap.getHeight(), gameMap);
    gateManager.SpawnGates(gameMap.getWidth(), gameMap.getHeight());
    MoveCenter();
}

void SnakeGame::Run()
{
    while (!gameOver)
    {
        Draw();
        Input();
        Logic();
    }
    if (!CheckMissionComplete()){
    mvprintw(gameMap.getHeight() / 2, (gameMap.getWidth() - 9) / 2, "Game Over");
    }
    refresh();
    getch();
}
