#pragma once
#include <vector>
#include <map>
#include "Postion.h"
#include "Colors.h"


class Block{
private:
   int cellSize;
   int rotationState;
   std::vector<Color> colors;
   int rowOffset;
   int columnOffset;

public:
    Block();
    void Draw(int offsetX, int offsetY);
    void Move(int rows, int columns);
    std::vector<Position> GetCellPositions();
    void Rotate();
    void UndoRotation();
    int id;
    std::map<int, std::vector<Position>> cells;
};