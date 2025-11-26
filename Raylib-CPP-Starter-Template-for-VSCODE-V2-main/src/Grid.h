#pragma once
#include <vector>
#include <raylib.h>

class Grid
{
private:
    int numRows;
    int numCols;
    int cellSize;
    bool IsRowFull(int row);
    void ClearRow(int row);
    void MoveRowDown(int row, int numRows);
    std::vector<Color> colors;
public:
    Grid();
    void Initialize();
    void print();
    void Draw();
    bool IsCellOutside(int row, int column);
    bool IsCellEmpty(int row, int column);
    int ClearFullRows();
    int grid[20][10];
};

