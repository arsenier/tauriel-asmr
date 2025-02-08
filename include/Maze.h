#pragma once

#include <inttypes.h>

#define MAZE_WIDTH 8
#define MAZE_HEIGHT 4

#define MAZE_MEM_SIZE (MAZE_WIDTH * MAZE_HEIGHT / 2)

#define SET_WALL(stored_wall, wall) stored_wall = (wall == Maze::UNKNOWN ? stored_wall : wall)

struct Vec2
{
    int16_t x;
    int16_t y;
};

/**
 * How much information is needed to represent all of the maze?
 *
 * One byte:
 *   cell A | cell B
 *      ssee|ssee
 *       | |  | ` east wall of cell B
 *       | |  `south wall of cell B
 *       | `east wall of cell A
 *       `south wall of cell A
 * Each wall is represented by a Wall enum
 *
 * So, 4 bits per cell, MAZE_WIDTH * MAZE_HEIGHT cells,
 * MAZE_MEM_SIZE = MAZE_WIDTH * MAZE_HEIGHT / 2 bytes
 */

class Maze
{
public:
    enum Wall : uint8_t
    {
        UNKNOWN = 0,
        WALL,
        OPEN
        // TODO: add more wall types
    };
    struct Cell
    {
        Wall left : 2;
        Wall down : 2;
        Wall up : 2;
        Wall right : 2;
    };
private:

    struct CellStoreRaw
    {
        Wall hidown : 2;
        Wall hiright : 2;
        Wall lodown : 2;
        Wall loright : 2;
    };

    enum LoHi
    {
        LO = 1,
        HI = 0
    };

    CellStoreRaw map[MAZE_MEM_SIZE];

public:

    Maze()
    {
        for (int i = 0; i < MAZE_MEM_SIZE; i++)
        {
            map[i].lodown = UNKNOWN;
            map[i].loright = UNKNOWN;
            map[i].hidown = UNKNOWN;
            map[i].hiright = UNKNOWN;
        }
    }

    void setWall(Vec2 coord, Cell cell_walls)
    {
        int cell_id = coord.y * MAZE_WIDTH / 2 + coord.x / 2;
        int cell_offset = coord.x % 2;

        if(cell_offset == LO)
        {
            SET_WALL(map[cell_id].lodown, cell_walls.down);
            SET_WALL(map[cell_id].loright, cell_walls.right);
            SET_WALL(map[cell_id].hiright, cell_walls.left);

            if(cell_id >= MAZE_WIDTH / 2)
            {
                SET_WALL(map[cell_id - MAZE_WIDTH / 2].lodown, cell_walls.up);
            }
        }
        else
        {
            SET_WALL(map[cell_id].hidown, cell_walls.down);
            SET_WALL(map[cell_id].hiright, cell_walls.right);

            if(cell_id % (MAZE_WIDTH / 2) != 0)
            {
                SET_WALL(map[cell_id - 1].loright, cell_walls.left);
            }

            if(cell_id >= MAZE_WIDTH / 2)
            {
                SET_WALL(map[cell_id - MAZE_WIDTH / 2].hidown, cell_walls.up);
            }
        }
    }

    Wall getUpWall(int cell_id, int cell_offset)
    {
        if(cell_id >= MAZE_WIDTH / 2)
        {
            if(cell_offset == LO)
            {
                return map[cell_id - MAZE_WIDTH / 2].lodown;
            }
            return map[cell_id - MAZE_WIDTH / 2].hidown;
        }
        return WALL;
    }
    Wall getLeftWall(int cell_id, int cell_offset)
    {
        if(cell_offset == LO)
        {
            return map[cell_id].hiright;
        }

        if(cell_id % (MAZE_WIDTH / 2) == 0)
        {
            return WALL;
        }

        return map[cell_id - 1].loright;
    }

    Wall getDownWall(int cell_id, int cell_offset)
    {
        if(cell_offset == LO)
        {
            return map[cell_id].lodown;
        }
        return map[cell_id].hidown;
    }

    Wall getRightWall(int cell_id, int cell_offset)
    {
        if(cell_offset == LO)
        {
            return map[cell_id].loright;
        }
        return map[cell_id].hiright;
    }

    Cell getWalls(Vec2 coord)
    {
        int cell_id = coord.y * MAZE_WIDTH / 2 + coord.x / 2;
        int cell_offset = coord.x % 2;

        Cell cell =
        {
            .left  = getLeftWall(cell_id, cell_offset),
            .down  = getDownWall(cell_id, cell_offset),
            .up    = getUpWall(cell_id, cell_offset),
            .right = getRightWall(cell_id, cell_offset)
        };

        return cell;
    }
};
