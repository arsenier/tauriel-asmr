#include <Arduino.h>

#include "MazeVeirify.h"
#include "Solver.h"

Maze maze;

void mazeTestCreate()
{
    maze.setWall(Vec2{1, 1}, Maze::CellWalls{.left = Maze::OPEN, .down = Maze::OPEN, .up = Maze::OPEN, .right = Maze::OPEN});
    maze.setWall(Vec2{6, 2}, Maze::CellWalls{.left = Maze::WALL, .down = Maze::OPEN, .up = Maze::WALL, .right = Maze::OPEN});
    maze.setWall(Vec2{3, 1}, Maze::CellWalls{.left = Maze::WALL, .down = Maze::WALL, .up = Maze::WALL, .right = Maze::WALL});
    maze.setWall(Vec2{2, 3}, Maze::CellWalls{.left = Maze::OPEN, .down = Maze::OPEN, .up = Maze::OPEN, .right = Maze::OPEN});
    maze.setWall(Vec2{4, 3}, Maze::CellWalls{.left = Maze::WALL, .down = Maze::WALL, .up = Maze::WALL, .right = Maze::WALL});

    Serial.println();

    drawMaze(maze, MAZE_WIDTH, MAZE_HEIGHT);
}

void solverVerify()
{
    Solver solver;

    Vec2 start = {1, 1};
    Vec2 end = {6, 2};

    uint32_t time0 = micros();
    solver.findPath(start, end, &maze);
    uint32_t dtime = micros() - time0;
    Serial.print("Solver time [us]: ");
    Serial.println(dtime);

    Serial.println();
    Serial.println("Solver result:");
    Serial.println(solver.getWhereFrom(start));
    Serial.println(solver.getWhereFrom(end));

    drawMaze(maze, solver, MAZE_WIDTH, MAZE_HEIGHT);
}

void setup()
{
    Serial.begin(9600);
    Serial.println("Setup done");

    mazeTestCreate();
    solverVerify();
}

void loop()
{
}
