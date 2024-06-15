#ifndef GAME_H
#define GAME_H

#include "../include/item.h"
#include "../include/gate.h"
#include "../include/map.h"
#include "../include/snake.h"
#include "../include/direction.h"
#include <chrono>
#include <vector>

class SnakeGame
{
public:
    SnakeGame();
    ~SnakeGame();
    void Run();
    std::vector<int> scores = {0};

private:
    void Initialize();
    void Draw();
    void Input();
    void Logic();
    void MoveCenter();
    void ChangeMap(const std::vector<std::string>& newMap);
    bool CheckMissionComplete(); // Add this line

    Snake snake;
    ItemManager itemManager;
    GateManager gateManager;
    GameMap gameMap;
    int score;
    int maxLength;
    int growthCount;
    int poisonCount;
    int gateCount;
    bool gameOver;
    Direction dir;
    int tick;
    int currentStage; // Add this line to track the current stage
    std::chrono::steady_clock::time_point lastMoveTime; // Add this line
};

#endif // GAME_H
