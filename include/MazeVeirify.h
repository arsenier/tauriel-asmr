#pragma once

#include <Arduino.h>
#include "Maze.h"
#include "MazeDrawer.h"

void mazeVeirify()
{
    Maze maze;

    drawMaze(maze, MAZE_WIDTH, MAZE_HEIGHT);

    {
    Vec2 coord = {1, 1};
    Maze::Cell cell_walls = {
        .left  = Maze::OPEN,
        .down  = Maze::OPEN,
        .up    = Maze::OPEN,
        .right = Maze::OPEN
    };
    maze.setWall(coord, cell_walls);
    }

    {
    Vec2 coord = {3, 1};
    Maze::Cell cell_walls = {
        .left  = Maze::WALL,
        .down  = Maze::WALL,
        .up    = Maze::WALL,
        .right = Maze::WALL
    };
    maze.setWall(coord, cell_walls);
    }

    {
    Vec2 coord = {2, 3};
    Maze::Cell cell_walls = {
        .left  = Maze::OPEN,
        .down  = Maze::OPEN,
        .up    = Maze::OPEN,
        .right = Maze::OPEN
    };
    maze.setWall(coord, cell_walls);
    }

    {
    Vec2 coord = {4, 3};
    Maze::Cell cell_walls = {
        .left  = Maze::WALL,
        .down  = Maze::WALL,
        .up    = Maze::WALL,
        .right = Maze::WALL
    };
    maze.setWall(coord, cell_walls);
    }
    Serial.println();

    drawMaze(maze, MAZE_WIDTH, MAZE_HEIGHT);

    uint32_t start = micros();
    for(int i = 0; i < 10000; i++)
    {
        Vec2 coord = {4, 3};
        Maze::Cell cell_walls = {
            .left  = Maze::WALL,
            .down  = Maze::WALL,
            .up    = Maze::WALL,
            .right = Maze::WALL
        };
        maze.setWall(coord, cell_walls);
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