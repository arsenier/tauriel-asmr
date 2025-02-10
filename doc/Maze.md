# Компактный и эффективный способ хранения информации о лабиринте в памяти Arduino

## Постановка задачи

Одной из главных подзадач при разработке системы управления микромышью является запоминание лабиринта и его обработка. При исследовании лабиринта требуется запоминать расположение стенок в каждой клетке, а при построении маршрута требуется "вспоминать" какие стенки и где были обнаружены.

Для этой задачи имеет смысл разработать модуль хранения информации о лабиринте, предоставляющий удобный интерфейс взаимодействия с ним.

## Разработка интерфейса

Для начала: какие задачи модуль хранения информации о лабиринте должен решать?

1. Иметь память о сохраненных клетках и стенках.
2. Иметь возможность добавления новой информации о стенках.
3. Иметь возможность получения информации о стенках.

## Реализация

Каждая стенка может находится в одном из трех состояний:

- Не исследована
- Исследована и является стенкой
- Исследована и является проездом

Обозначим каждое из этих состояний как перечисление:

```c++
enum Wall
{
    UNKNOWN,
    WALL,
    OPEN
};
```

Тогда для хранения информации о стенках внутри одной клетки можно использовать следующую структуру:

```c++
struct Cell
{
    Wall left;
    Wall down;
    Wall up;
    Wall right;
};
```

> Порядок стенок в структуре в конечном итоге не имеет значения, т.к. к полям мы все равно будем обращатся по имени. Именно такой порядок я выбрал исходя из порядка расположения горячих клавиш движения в Vim на клавиатуре: `hjkl`, что транслируется в `влево`, `вниз`, `вверх`, `вправо`.

Эта структура определяет интерфейс взаимодействия с нашим классом. С помощью нее мы сможем передавать информацию о стенках и также ее получать обратно.

## Определение интерфейса

Для начала мы определим интерфейс взаимодействия с классом следующим образом:

```c++
class Maze
{
// ....... //
public:
    /**
     * @brief Установить стенку в клетке с данными координатами
     */
    void setWall(Vec2 coord, Cell cell_walls);

    /**
    * @brief Получить информацию о стенках в клетке с данными координатами
    */
    Cell getWalls(Vec2 coord);
// ....... //
};
```

Эти два метода будут нашим основным способом общения с классом.

## Пример использования

```cpp
#include "Maze.h"

int main()
{
    Maze maze;

    // Сохранение стенок
    Vec2 coord = {2, 3};
    Maze::Cell cell_walls = {
        .left  = Maze::OPEN,
        .down  = Maze::OPEN,
        .up    = Maze::OPEN,
        .right = Maze::OPEN
    };
    maze.setWall(coord, cell_walls);

    // Чтение стенок
    coord = {4, 3};
    Maze::Cell cell_walls_read = maze.getWalls(coord);
}
```

## Хранение лабиринта

Теперь, после того как мы определились с интерфейсом, поговорим о том, как мы можем хранить лабиринт в памяти. Есть несколько способов подойти к решению этой задачи. Подробный разбор всех вариантов пока проводить не будем, сфокусируемся на том, который мне показался самым эффективным по памяти:

Для каждой ячейки лабиринта будем хранить информацию правой и нижней стенках. Таким образом информация о каждой стенке будет хранится в единственном экзепляре и на ячейку будет отводится 4 бита. 

```
Один байт:
ячейка A | ячейка B
    [ssee|ssee]
      | |  | ` восточная (правая) стенка ячейки B
      | |  ` южная (нижняя) стенка ячейки B
      | ` восточная (правая) стенка ячейки A
      ` нижняя (южная) стенка ячейки A
```

При таком способе хранения информация о всем лабиринте Micromouse поместится в: `16x16x0.5 = 128Байт`.

### Хранение данных

Как же реализовать такое хранение данных?

```c
#define MAZE_WIDTH 8
#define MAZE_HEIGHT 4
#define MAZE_MEM_SIZE (MAZE_WIDTH * MAZE_HEIGHT / 2)

class Maze{
// ............ //
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
// ............ //
};
```

Вся информация хранится в массиве структур `CellStoreRaw`, хранящих информацию о двух соседних клетках.

### Запись информации о стенках

Для записи информации о стенках реализуем следующий метод:

```c++
class Maze{
// .......... //
public:
    /**
     * @brief Установить стенку в клетке с данными координатами
     */
    void setWall(Vec2 coord, Cell cell_walls)
    {
        // Определение индекса ячейки в массиве и ее относительного положения (LO - правая, HI - левая клетка из пары)
        int cell_id = coord.y * MAZE_WIDTH / 2 + coord.x / 2;
        int cell_offset = coord.x % 2;

        if(cell_offset == LO)
        {
            SET_WALL(map[cell_id].lodown, cell_walls.down);
            SET_WALL(map[cell_id].loright, cell_walls.right);
            SET_WALL(map[cell_id].hiright, cell_walls.left);

            // Краевой случай - если над клеткой ничего нет, то ничего не сохраняем
            if(cell_id >= MAZE_WIDTH / 2)
            {
                SET_WALL(map[cell_id - MAZE_WIDTH / 2].lodown, cell_walls.up);
            }
        }
        else
        {
            SET_WALL(map[cell_id].hidown, cell_walls.down);
            SET_WALL(map[cell_id].hiright, cell_walls.right);

            // Аналогично с самой левой клеткой. Слева нет ячейки, поэтмоу просто ничего не делаем
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
// ......... //
};
```

Здесь можно заметить макрос `SET_WALL`, который проверяет, нужно ли сохранять информацию о стенке в ячейке. Если была передана конкретная информация о стенке (не UNKNOWN), то сохраняем ее, иначе ничего не делаем.

```c
#define SET_WALL(stored_wall, wall) stored_wall = (wall == Maze::UNKNOWN ? stored_wall : wall)
```

### Получение информации о стенках

Для получения информации о стенках реализуем следующий метод:

```c++
class Maze{
// .......... //
public:
    /**
    * @brief Получить информацию о стенках в клетке с данными координатами
    */
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
// ......... //
};
```

В этом методе мы получаем индекс ячейки в массиве и ее относительного положения. Для упрощения работы со стенками были реализованы функции, которые позволяют найти соответствующую стенку в ячейке:

```c++
class Maze{
// .......... //
public:
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
// ......... //
};
```

Эти функции проверяют, находится ли соответствующая стенка в ячейке и возвращают соответствующий тип стенки.

## Результат

Таким образом, мы получили способ хранения информации о лабиринте в памяти Arduino. Полный код можно найти в репозитории [tauriel-asmr](https://github.com/arsenier/tauriel-asmr).

<details>
<summary>Полный код класса Maze</summary>
```c++
#pragma once

#include <inttypes.h>

#define MAZE_WIDTH 8
#define MAZE_HEIGHT 4

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

#define MAZE_MEM_SIZE (MAZE_WIDTH * MAZE_HEIGHT / 2)

#define SET_WALL(stored_wall, wall) stored_wall = (wall == Maze::UNKNOWN ? stored_wall : wall)

struct Vec2
{
    int16_t x;
    int16_t y;
};

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

    /**
     * @brief Установить стенку в клетке с данными координатами
     */
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

    /**
    * @brief Получить информацию о стенках в клетке с данными координатами
    */
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
```
</details>

<details>
<summary>Пример использования</summary>
`MazeVeirify.h`
```c++
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
```
`main.cpp`
```c++
#include <Arduino.h>

#include "MazeVeirify.h"

void setup()
{
    Serial.begin(9600);
    Serial.println("Setup done");

    mazeVeirify();
}

void loop()
{
}
```
</details>

