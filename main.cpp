#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <curses.h>
#include "map1.h"
#include "map2.h"
#include "map3.h"
#include "map4.h"

using namespace std;
using namespace chrono;

enum Direction
{
    STOP = 0,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

struct Point
{
    int x, y;
    Point(int col, int row) : x(col), y(row) {}
    Point() : x(0), y(0) {}
};

class SnakeGame
{
public:
    SnakeGame();
    ~SnakeGame();
    void Run();

private:
    void Initialize();
    void Draw();
    void Input();
    void Logic();
    void SpawnItem();
    void SpawnGates();
    void NextMap();
    bool IsCollision(Point p);
    bool IsItem(Point p);
    bool IsGate(Point p);
    void MoveThroughGate();
    Point GetExitPoint(Point gate, Direction entryDir);
    void GameOver();

    vector<Point> snake;
    vector<Point> growthItems;
    vector<Point> poisonItems;
    Point gateA, gateB;
    int width, height;
    int score;
    int maxLength;
    int growthCount, poisonCount, gateCount;
    bool gameOver;
    Direction dir, nextDir;
    int tick;
    string *map;
    int mapSize;
    int currentMap;
    time_point<steady_clock> startTime;
    time_t survivalTime;
};

SnakeGame::SnakeGame()
{
    width = 21;
    height = 21;
    score = 0;
    maxLength = 0;
    growthCount = 0;
    poisonCount = 0;
    gateCount = 0;
    gameOver = false;
    dir = RIGHT;     // 기본 방향을 오른쪽으로 설정
    nextDir = RIGHT; // 기본 방향을 오른쪽으로 설정
    tick = 300;      // ms

    // map1을 기본 맵으로 설정
    map = map1;
    mapSize = sizeof(map1) / sizeof(map1[0]);
    currentMap = 1;

    Initialize();
}

SnakeGame::~SnakeGame()
{
    endwin(); // curses 모드 종료
}

void SnakeGame::Initialize()
{
    initscr(); // curses 모드 초기화
    clear();
    noecho();
    cbreak();
    curs_set(0);
    keypad(stdscr, TRUE);
    timeout(tick);

    srand(time(0));

    snake.clear();
    snake.push_back(Point(width / 2, height / 2));
    for (int i = 1; i < 4; ++i)
    {
        snake.push_back(Point(width / 2 - i, height / 2));
    }

    startTime = steady_clock::now(); // 게임 시작 시간 저장

    SpawnItem();
    SpawnGates();
}

void SnakeGame::SpawnItem()
{
    growthItems.clear();
    poisonItems.clear();
    for (int i = 0; i < 3; ++i)
    {
        Point growth(rand() % width, rand() % height);
        while (IsCollision(growth) || IsItem(growth) || map[growth.y][growth.x] != ' ')
        {
            growth = Point(rand() % width, rand() % height);
        }
        growthItems.push_back(growth);

        Point poison(rand() % width, rand() % height);
        while (IsCollision(poison) || IsItem(poison) || map[poison.y][poison.x] != ' ')
        {
            poison = Point(rand() % width, rand() % height);
        }
        poisonItems.push_back(poison);
    }
}

void SnakeGame::SpawnGates()
{
    vector<Point> walls;
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            if (map[y][x] == '1')
            {
                walls.push_back(Point(x, y));
            }
        }
    }

    if (walls.size() >= 2)
    {
        int idxA = rand() % walls.size();
        gateA = walls[idxA];
        walls.erase(walls.begin() + idxA);

        int idxB = rand() % walls.size();
        gateB = walls[idxB];
    }
}

void SnakeGame::NextMap()
{
    currentMap++;
    if (currentMap > 4)
    {
        currentMap = 1;
    }
    switch (currentMap)
    {
    case 1:
        map = map1;
        mapSize = sizeof(map1) / sizeof(map1[0]);
        break;
    case 2:
        map = map2;
        mapSize = sizeof(map2) / sizeof(map2[0]);
        break;
    case 3:
        map = map3;
        mapSize = sizeof(map3) / sizeof(map3[0]);
        break;
    case 4:
        map = map4;
        mapSize = sizeof(map4) / sizeof(map4[0]);
        break;
    }
    Initialize();
}

void SnakeGame::Draw()
{
    clear();

    // 맵 그리기
    for (int y = 0; y < mapSize; ++y)
    {
        mvprintw(y + 1, 1, "%s", map[y].c_str());
    }

    // 아이템 그리기
    for (const auto &item : growthItems)
    {
        mvprintw(item.y + 1, item.x + 1, "G");
    }
    for (const auto &item : poisonItems)
    {
        mvprintw(item.y + 1, item.x + 1, "P");
    }

    // 게이트 그리기
    mvprintw(gateA.y + 1, gateA.x + 1, "A");
    mvprintw(gateB.y + 1, gateB.x + 1, "B");

    // 뱀 그리기
    for (size_t i = 0; i < snake.size(); ++i)
    {
        mvprintw(snake[i].y + 1, snake[i].x + 1, i == 0 ? "@" : "o");
    }

    // 점수 표시
    auto currentTime = steady_clock::now();
    survivalTime = duration_cast<seconds>(currentTime - startTime).count();
    mvprintw(0, width + 4, "Score: %d", score);
    mvprintw(1, width + 4, "B: %d / %d", (int)snake.size(), maxLength);
    mvprintw(2, width + 4, "+: %d", growthCount);
    mvprintw(3, width + 4, "-: %d", poisonCount);
    mvprintw(4, width + 4, "G: %d", gateCount);
    mvprintw(5, width + 4, "Time: %ld", survivalTime);

    refresh();
}

void SnakeGame::Input()
{
    int ch = getch();
    switch (ch)
    {
    case KEY_LEFT:
        if (dir != RIGHT)
            nextDir = LEFT;
        break;
    case KEY_RIGHT:
        if (dir != LEFT)
            nextDir = RIGHT;
        break;
    case KEY_UP:
        if (dir != DOWN)
            nextDir = UP;
        break;
    case KEY_DOWN:
        if (dir != UP)
            nextDir = DOWN;
        break;
    case 'q':
        gameOver = true;
        break;
    }
}

void SnakeGame::Logic()
{
    if (nextDir != STOP && dir != nextDir)
    {
        dir = nextDir;
    }

    Point prev = snake[0];
    Point prev2;
    switch (dir)
    {
    case LEFT:
        --snake[0].x;
        break;
    case RIGHT:
        ++snake[0].x;
        break;
    case UP:
        --snake[0].y;
        break;
    case DOWN:
        ++snake[0].y;
        break;
    default:
        break;
    }

    for (size_t i = 1; i < snake.size(); ++i)
    {
        prev2 = snake[i];
        snake[i] = prev;
        prev = prev2;
    }

    if (IsCollision(snake[0]) || snake[0].x < 0 || snake[0].x >= width || snake[0].y < 0 || snake[0].y >= height || map[snake[0].y][snake[0].x] == '1')
    {
        gameOver = true;
    }

    if (IsItem(snake[0]))
    {
        for (auto it = growthItems.begin(); it != growthItems.end();)
        {
            if (snake[0].x == it->x && snake[0].y == it->y)
            {
                score += 100;
                growthCount++;
                snake.push_back(Point());
                it = growthItems.erase(it);
            }
            else
            {
                ++it;
            }
        }

        for (auto it = poisonItems.begin(); it != poisonItems.end();)
        {
            if (snake[0].x == it->x && snake[0].y == it->y)
            {
                snake.pop_back();
                score -= 5;
                poisonCount++;
                it = poisonItems.erase(it);
                if (snake.size() < 3)
                {
                    gameOver = true;
                }
            }
            else
            {
                ++it;
            }
        }
    }

    if (IsGate(snake[0]))
    {
        MoveThroughGate();
        gateCount++;
        score += 20; // 게이트 사용 시 추가 점수
    }

    if (snake.size() > maxLength)
    {
        maxLength = snake.size();
    }

    if (growthItems.empty() || poisonItems.empty())
    {
        SpawnItem();
    }

    // 100점마다 다음 맵으로 넘어감
    if (score >= 100)
    {
        score -= 100;
        NextMap();
    }
}

bool SnakeGame::IsCollision(Point p)
{
    for (size_t i = 1; i < snake.size(); ++i)
    {
        if (snake[i].x == p.x && snake[i].y == p.y)
            return true;
    }
    return false;
}

bool SnakeGame::IsItem(Point p)
{
    for (const auto &item : growthItems)
    {
        if (item.x == p.x && item.y == p.y)
            return true;
    }
    for (const auto &item : poisonItems)
    {
        if (item.x == p.x && item.y == p.y)
            return true;
    }
    return false;
}

bool SnakeGame::IsGate(Point p)
{
    return (p.x == gateA.x && p.y == gateA.y) || (p.x == gateB.x && p.y == gateB.y);
}

void SnakeGame::MoveThroughGate()
{
    if (snake[0].x == gateA.x && snake[0].y == gateA.y)
    {
        snake[0] = GetExitPoint(gateB, dir);
    }
    else if (snake[0].x == gateB.x && snake[0].y == gateB.y)
    {
        snake[0] = GetExitPoint(gateA, dir);
    }
}

Point SnakeGame::GetExitPoint(Point gate, Direction entryDir)
{
    vector<Direction> directions;

    // 가장자리 벽에 있는 경우
    if (gate.y == 0)
    {
        return Point(gate.x, gate.y + 1); // 상단 벽 => 아래 방향
    }
    else if (gate.y == height - 1)
    {
        return Point(gate.x, gate.y - 1); // 하단 벽 => 위 방향
    }
    else if (gate.x == 0)
    {
        return Point(gate.x + 1, gate.y); // 좌측 벽 => 오른쪽 방향
    }
    else if (gate.x == width - 1)
    {
        return Point(gate.x - 1, gate.y); // 우측 벽 => 왼쪽 방향
    }

    // 가운데 벽에 있는 경우
    directions.push_back(entryDir);
    switch (entryDir)
    {
    case LEFT:
        directions.push_back(UP);
        directions.push_back(DOWN);
        directions.push_back(RIGHT);
        break;
    case RIGHT:
        directions.push_back(DOWN);
        directions.push_back(UP);
        directions.push_back(LEFT);
        break;
    case UP:
        directions.push_back(LEFT);
        directions.push_back(RIGHT);
        directions.push_back(DOWN);
        break;
    case DOWN:
        directions.push_back(RIGHT);
        directions.push_back(LEFT);
        directions.push_back(UP);
        break;
    default:
        break;
    }

    for (Direction d : directions)
    {
        int newX = gate.x;
        int newY = gate.y;
        switch (d)
        {
        case LEFT:
            newX -= 1;
            break;
        case RIGHT:
            newX += 1;
            break;
        case UP:
            newY -= 1;
            break;
        case DOWN:
            newY += 1;
            break;
        default:
            break;
        }
        if (map[newY][newX] == ' ')
        {
            return Point(newX, newY);
        }
    }

    // 모든 방향이 막힌 경우 진입 방향으로 진출
    switch (entryDir)
    {
    case LEFT:
        return Point(gate.x - 1, gate.y);
    case RIGHT:
        return Point(gate.x + 1, gate.y);
    case UP:
        return Point(gate.x, gate.y - 1);
    case DOWN:
        return Point(gate.x, gate.y + 1);
    default:
        return gate;
    }
}

void SnakeGame::GameOver()
{
    // 생존 기간 계산
    auto endTime = steady_clock::now();
    survivalTime = duration_cast<seconds>(endTime - startTime).count();
    score += survivalTime; // 생존 기간에 따른 추가 점수

    clear();
    mvprintw(height / 2, (width - 9) / 2, "Game Over");
    mvprintw(height / 2 + 1, (width - 9) / 2 - 4, "Final Score: %d", score);
    mvprintw(height / 2 + 2, (width - 9) / 2 - 4, "Total Time: %ld sec", survivalTime);
    mvprintw(height / 2 + 3, (width - 9) / 2 - 4, "Gates Used: %d", gateCount);
    mvprintw(height / 2 + 5, (width - 9) / 2 - 4, "Press any key to exit...");
    refresh();
    nodelay(stdscr, FALSE); // 키 입력을 기다림
    getch();
}

void SnakeGame::Run()
{
    while (!gameOver)
    {
        Draw();
        Input();
        Logic();
    }
    GameOver(); // 게임 오버 시 호출
}

int main()
{
    SnakeGame game;
    game.Run();
    return 0;
}
