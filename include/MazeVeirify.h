#pragma once

#include <Arduino.h>
#include "Maze.h"
#include "MazeDrawer.h"

void mazeVeirify()
{
    Maze maze;

    drawMaze(maze, MAZE_WIDTH, MAZE_HEIGHT);

    maze.setWall(Vec2{1, 1}, Maze::Cell{.left = Maze::OPEN, .down = Maze::OPEN, .up = Maze::OPEN, .right = Maze::OPEN});
    maze.setWall(Vec2{6, 2}, Maze::Cell{.left = Maze::WALL, .down = Maze::OPEN, .up = Maze::WALL, .right = Maze::OPEN});
    maze.setWall(Vec2{3, 1}, Maze::Cell{.left = Maze::WALL, .down = Maze::WALL, .up = Maze::WALL, .right = Maze::WALL});
    maze.setWall(Vec2{2, 3}, Maze::Cell{.left = Maze::OPEN, .down = Maze::OPEN, .up = Maze::OPEN, .right = Maze::OPEN});
    maze.setWall(Vec2{4, 3}, Maze::Cell{.left = Maze::WALL, .down = Maze::WALL, .up = Maze::WALL, .right = Maze::WALL});

    Serial.println();

    drawMaze(maze, MAZE_WIDTH, MAZE_HEIGHT);

    uint32_t start = micros();
    for(int i = 0; i < 10000; i++)
    {
        maze.setWall(Vec2{4, 3}, Maze::Cell{.left = Maze::WALL, .down = Maze::WALL, .up = Maze::WALL, .right = Maze::WALL});
    }
    uint32_t end = micros();

    Serial.print("10000x setWall time [us]: ");
    Serial.println(end - start);
    Serial.print("setWall time [us]: ");
    Serial.println((end - start) / 10000.0);

    start = micros();
    for(int i = 0; i < 10000; i++)
    {
        Vec2 coord = {4, 3};
        Maze::Cell cell_walls = maze.getWalls(coord);
    }
    end = micros();
    Serial.print("10000x getWalls time [us]: ");
    Serial.println(end - start);
    Serial.print("getWalls time [us]: ");
    Serial.println((end - start) / 10000.0);
}
