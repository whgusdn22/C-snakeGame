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
      lastItemSpawnTime(std::chrono::steady_clock::now()), // Add item spawn time
      startTime(std::chrono::steady_clock::now()),         // Initialize startTime
      currentStage(1)                                      // Initialize currentStage
{
    score = 0;
    maxLength = 0;
    growthCount = 0;
    poisonCount = 0;
    gateCount = 0;
    gameOver = false;
    dir = RIGHT;
    tick = 150; // ms
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

    // Spawn initial items and gates
    itemManager.SpawnItems(gameMap.getWidth(), gameMap.getHeight(), gameMap, snake.body);
    gateManager.SpawnGates(gameMap.getWidth(), gameMap.getHeight());
}

void SnakeGame::Draw()
{
    clear();
    gameMap.Draw();
    gateManager.Draw();
    const auto &snakeBody = snake.GetBody();
    for (size_t i = 0; i < snakeBody.size(); ++i)
    {
        mvprintw(snakeBody[i].y + 1, snakeBody[i].x + 1, i == 0 ? "@" : "o");
    }
    itemManager.Draw();
    // Draw Score Board box
    int boxWidth = 20;
    int boxHeight = 9; // Increase the height to fit the time display
    int startX = gameMap.getWidth() + 2;
    int startY = 1;
    mvprintw(startY, startX, "+------------------+");
    for (int i = 1; i <= boxHeight; ++i)
    {
        mvprintw(startY + i, startX, "|");
        mvprintw(startY + i, startX + boxWidth - 1, "|");
    }
    mvprintw(startY + boxHeight + 1, startX, "+------------------+");

    // Calculate the elapsed time
    auto now = std::chrono::steady_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(now - startTime).count();

    // Draw Score Board text inside the box
    mvprintw(startY + 1, startX + 2, "Score Board");
    mvprintw(startY + 2, startX + 2, "score: %d", score);
    mvprintw(startY + 3, startX + 2, "B: (%d) / (%d)", snakeBody.size(), maxLength);
    mvprintw(startY + 4, startX + 2, "+: %d", growthCount);
    mvprintw(startY + 5, startX + 2, "-: %d", poisonCount);
    mvprintw(startY + 6, startX + 2, "G: %d", gateCount);
    mvprintw(startY + 7, startX + 2, "tick: %d", tick);
    mvprintw(startY + 8, startX + 2, "Time: %lds", elapsedTime);

    // Draw Mission box
    int missionBoxWidth = 20;
    int missionBoxHeight = 6;
    int missionStartX = gameMap.getWidth() + 2;
    int missionStartY = boxHeight + 3;
    mvprintw(missionStartY, missionStartX, "+------------------+");
    for (int i = 1; i <= missionBoxHeight; ++i)
    {
        mvprintw(missionStartY + i, missionStartX, "|");
        mvprintw(missionStartY + i, missionStartX + missionBoxWidth - 1, "|");
    }
    mvprintw(missionStartY + missionBoxHeight + 1, missionStartX, "+------------------+");

    // Draw Mission text inside the box
    int missionLength = 10; // Example mission length, you can adjust as needed
    mvprintw(missionStartY + 1, missionStartX + 2, "Mission");
    mvprintw(missionStartY + 2, missionStartX + 2, "B: %d (%s)", missionLength, maxLength >= missionLength ? "v" : " ");
    mvprintw(missionStartY + 3, missionStartX + 2, "+: 5 (%s)", growthCount >= 5 ? "v" : " ");
    mvprintw(missionStartY + 4, missionStartX + 2, "-: 2 (%s)", poisonCount >= 2 ? "v" : " ");
    mvprintw(missionStartY + 5, missionStartX + 2, "G: 1 (%s)", gateCount >= 1 ? "v" : " ");

    refresh();
}

void SnakeGame::Input()
{
    int ch = getch();
    switch (ch)
    {
    case KEY_LEFT:
        dir = LEFT;
        break;
    case KEY_RIGHT:
        dir = RIGHT;
        break;
    case KEY_UP:
        dir = UP;
        break;
    case KEY_DOWN:
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
                score += 20;
                growthCount++;
                Point tail = snake.body.back();
                Point before_tail = *(&snake.body.back() - 1);
                if (before_tail + Point(1, 0) == tail)
                    snake.body.push_back(tail + Point(1, 0));
                else if (before_tail + Point(-1, 0) == tail)
                    snake.body.push_back(tail + Point(-1, 0));
                else if (before_tail + Point(0, 1) == tail)
                    snake.body.push_back(tail + Point(1, 0));
                else if (before_tail + Point(0, -1) == tail)
                    snake.body.push_back(tail + Point(-1, 0));
                Draw();
                // timeout(tick);
            }
            else if (itemManager.IsSpeedItem(snake.GetHead()))
            {
                if (tick > 50)
                {
                    score += 5;
                    tick -= 15;
                }
                else
                    score += 10;
                timeout(tick); // Update timeout when tick changes
            }
            else
            {
                score -= 10;
                poisonCount++;
                snake.Shrink();
                // timeout(tick);
                if (snake.GetBody().size() < 4)
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
            if ((gameMap.IsWall(snake.GetHead()) && !gateManager.IsGatePosition(snake.GetHead())) || snake.IsCollision(snake.GetHead()))
            {
                gameOver = true;
            }
            else
            {
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
            getchar();
            getch();
            gameOver = true;
            return;
        }

        // Respawn items every 6 seconds
        auto itemDuration = std::chrono::duration_cast<std::chrono::seconds>(now - lastItemSpawnTime).count();
        if (itemDuration >= 6)
        {
            itemManager.SpawnItems(gameMap.getWidth(), gameMap.getHeight(), gameMap, snake.body);
            lastItemSpawnTime = now;
        }
    }
    if (score >= 100 && currentStage == 1)
    {
        currentStage = 2;
        if (tick > 50)
            tick -= 20;
        timeout(tick); // Update timeout when tick changes
        ChangeMap(std::vector<std::string>(std::begin(map2), std::end(map2)));
    }
    else if (score >= 200 && currentStage == 2)
    {
        currentStage = 3;
        if (tick > 50)
            tick -= 10;
        timeout(tick); // Update timeout when tick changes
        ChangeMap(std::vector<std::string>(std::begin(map3), std::end(map3)));
    }
    else if (score >= 300 && currentStage == 3)
    {
        currentStage = 4;
        if (tick > 50)
            tick -= 10;
        timeout(tick); // Update timeout when tick changes
        ChangeMap(std::vector<std::string>(std::begin(map4), std::end(map4)));
    }
}

void SnakeGame::ChangeMap(const std::vector<std::string> &newMap)
{
    gameMap.ChangeMap(newMap);
    itemManager.SpawnItems(gameMap.getWidth(), gameMap.getHeight(), gameMap, snake.body);
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
    if (!CheckMissionComplete())
    {
        mvprintw(gameMap.getHeight() / 2, (gameMap.getWidth() - 9) / 2, "Game Over");
        refresh();
        getch();
    }
}
