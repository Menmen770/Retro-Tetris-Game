#include "Grid.h"
#include "Colors.h"

bool Grid::IsRowFull(int row){
    for (int column = 0; column < numCols; column++){
        if (grid[row][column] == 0){
            return false;
        }
    }
    return true;
}

void Grid::ClearRow(int row){
    for (int column = 0; column < numCols; column++){
        grid[row][column] = 0;
    }
}

void Grid::MoveRowDown(int row, int numRows){
    for (int column = 0; column < numCols; column++){
        grid[row + numRows][column] = grid[row][column];
        grid[row][column] = 0;
    }
}

Grid::Grid()
{
    numRows = 20;
    numCols = 10;
    cellSize = 30;
    Initialize();
    colors = GetCellColors();
}

void Grid::Initialize(){
    for (int row = 0; row < numRows; row++)
    {
        for (int column = 0; column < numCols; column++)
        {
            grid[row][column] = 0;
        }
    }
}

// void Grid::print(){
//     for (int row = 0; row < numRows; row++)
//     {
//         for (int column = 0; column < numCols; column++)
//         {
//             std::cout << grid[row][column] << " ";
//         }
//         std::cout << std::endl;
//     }
// }
// ... שאר הפונקציות ...

void Grid::Draw(){
    // הגדר את צבעי הברק והצל גם כאן אם אתה משתמש בטכניקת ה-Draw המלאה
    const int shadeSize = 2; // נשאר אותו דבר כמו ב-Block.cpp

    for (int row = 0; row < numRows; row++){
        for (int column = 0; column < numCols; column++){
            int cellValue = grid[row][column];
            
            int x = column * cellSize + 11;
            int y = row * cellSize + 11;
            int size = cellSize - 2; // גודל המלבן המצויר
            
            if (cellValue != 0){
                // *** שינוי קטן: ציור בלוק ממוקם עם ברק ***
                Color baseColor = colors[cellValue];
                Color highlightColor = GetHighlightColor(baseColor); 
                Color shadowColor = GetShadowColor(baseColor);
                
                // 1. ציור מלבן הבסיס (הצבע המרכזי)
                DrawRectangle(x, y, size, size, baseColor);

                // 2. ציור הארה (Highlight) - למעלה ושמאל
                DrawRectangle(x, y, size, shadeSize, highlightColor); 
                DrawRectangle(x, y, shadeSize, size, highlightColor); 
                
                // 3. ציור הצל (Shadow) - למטה וימין
                DrawRectangle(x, y + size - shadeSize, size, shadeSize, shadowColor); 
                DrawRectangle(x + size - shadeSize, y, shadeSize, size, shadowColor); 
            }
            else {
                // *** הוספת ציור תא ריק (קווי הרשת) ***
                
                // כאן אנחנו מציירים את המלבן הריק בצבע הרשת.
                // נניח ש-gridLinesColor הוא הצבע שהגדרת לקווים.
                // אם gridLinesColor אינו גלובלי, תצטרך להגדיר אותו כמשתנה חבר ב-Grid או להביא אותו מ-Colors.h
                
                // אם השתמשת ב-darkGrey/gridLinesColor, ודא שהוא מוגדר ונגיש
                // דוגמה:
                DrawRectangle(x, y, size, size, gridLinesColor); 
            }
        }   
    }
}

bool Grid::IsCellOutside(int row, int column)
{
    if (row >= 0 && row < numRows && column >= 0 && column < numCols){
        return false;
    }
    return true;
}

bool Grid::IsCellEmpty(int row, int column)
{
    if (grid[row][column] == 0){
        return true;
    }
    return false;
}

int Grid::ClearFullRows()
{
    int comleted = 0;
    for (int row = numRows -1; row >= 0; row--){
        if (IsRowFull(row)){
            ClearRow(row);
            comleted++;
        }
        else if (comleted > 0){
            MoveRowDown(row, comleted);
        }
    }
    return comleted;
}
