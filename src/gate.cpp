#include "../include/gate.h"
#include <vector>
#include <cstdlib>
#include <curses.h>
#include "../map/map.cpp"

<<<<<<< Updated upstream
// void GateManager::FindWalls(GameMap)

void GateManager::SpawnGates(int width, int height)
{
    gateA = Point(rand() % width, rand() % height);
    gateB = Point(rand() % width, rand() % height);
=======
void GateManager::SpawnGates(int width, int height)
{
    std::vector<Point> wallPositions;

    // Collect all wall positions (where map value is '1')
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            if (gameMap.GetMap()[y][x] == '1')
            {
                wallPositions.push_back(Point(x, y));
            }
        }
    }

    if (wallPositions.size() >= 2)
    {
        int idxA = rand() % wallPositions.size();
        gateA = wallPositions[idxA];
        wallPositions.erase(wallPositions.begin() + idxA);

        int idxB = rand() % wallPositions.size();
        gateB = wallPositions[idxB];
    }
>>>>>>> Stashed changes
}

bool GateManager::IsGate(Point p)
{
    return (p.x == gateA.x && p.y == gateA.y) || (p.x == gateB.x && p.y == gateB.y);
}

Point GateManager::GetOtherGate(Point p)
{
    if (p.x == gateA.x && p.y == gateA.y)
    {
        return gateB;
    }
    else
    {
        return gateA;
    }
}

void GateManager::Draw()
{
    mvprintw(gateA.y + 1, gateA.x + 1, "A");
    mvprintw(gateB.y + 1, gateB.x + 1, "B");
}
