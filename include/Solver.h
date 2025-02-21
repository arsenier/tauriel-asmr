#pragma once

#include "Maze.h"
#include "Config.h"


const int MAX_SIZE = 64; // Maximum size of the queue

template<typename T>
class Queue {
private:
    T arr[MAX_SIZE];
    int begin;
    int end;

public:
    Queue() {
        begin = 0;
        end = 0;
    }

    bool isEmpty()
    {
        return begin == end;
    }

    bool isFull()
    {
        return isEmpty();
    }

    bool push_back(T x)
    {
        arr[end] = x;
        end = (end + 1) % MAX_SIZE;
        return isFull();
    }

    T pop_front()
    {
        T x = arr[begin];
        begin = (begin + 1) % MAX_SIZE;
        return x;
    }

    void clear()
    {
        begin = 0;
        end = 0;
    }
};

class Solver
{
private:

    enum class WhereFrom
    {
        UNKNOWN,
        LEFT,
        DOWN,
        UP,
        RIGHT
    };

    Queue<Vec2> queue;

    WhereFrom whereFrom[MAZE_WIDTH][MAZE_HEIGHT] = {WhereFrom::UNKNOWN};

public:
    void findPath(Vec2 start, Vec2 end, const Maze *maze)
    {
        for(int i = 0; i < MAZE_WIDTH; i++)
        {
            for(int j = 0; j < MAZE_HEIGHT; j++)
            {
                whereFrom[i][j] = WhereFrom::UNKNOWN;
            }
        }
        // Добавляем конечную точку в очередь
        queue.push_back(end);

        while(!queue.isEmpty())
        {
            // Берем текущую точку из очереди
            Vec2 current = queue.pop_front();

            Maze::CellWalls cell = maze->getWalls(current);

            if(cell.left != Maze::WALL)
            {
                Vec2 left = {current.x - 1, current.y};
                whereFrom[left.x][left.y] = WhereFrom::RIGHT;
                queue.push_back(left);
            }
            if(cell.right != Maze::WALL)
            {
                Vec2 right = {current.x + 1, current.y};
                whereFrom[right.x][right.y] = WhereFrom::LEFT;
                queue.push_back(right);
            }
            if(cell.up != Maze::WALL)
            {
                Vec2 up = {current.x, current.y - 1};
                whereFrom[up.x][up.y] = WhereFrom::DOWN;
                queue.push_back(up);
            }
            if(cell.down != Maze::WALL)
            {
                Vec2 down = {current.x, current.y + 1};
                whereFrom[down.x][down.y] = WhereFrom::UP;
                queue.push_back(down);
            }
        }
    }

    // FIX: Remove nafig
    int getWhereFrom(Vec2 coord)
    {
        return (int)whereFrom[coord.x][coord.y];
    }
};
