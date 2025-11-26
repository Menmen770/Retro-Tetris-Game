#include "Block.h"

Block::Block(){
    cellSize = 33;
    rotationState = 0;
    colors = GetCellColors();
    rowOffset = 0;
    columnOffset = 0;
}

void Block::Draw(int offsetX, int offsetY){
    std::vector<Position> tiles = GetCellPositions();
    Color baseColor = colors[id]; // הצבע הראשי
    
    Color highlightColor = GetHighlightColor(baseColor); 
    Color shadowColor = GetShadowColor(baseColor);
    
    const int shadeSize = 2; // גודל קו הצל וההארה
    // size = cellSize - 2: שומר על מרווח של פיקסל 1 מכל צד מהגריד, כמו ב-cellSize - 1 המקורי, 
    // אבל משאיר מקום לקווי הצל/הארה להיות בתוך הגבולות.

    for (Position item: tiles){
        // חישוב מיקום הבלוק על המסך
        int x = item.column * cellSize + offsetX;
        int y = item.row * cellSize + offsetY;
        int size = cellSize - 2; 
        
        // 1. ציור מלבן הבסיס (הצבע המרכזי)
        DrawRectangle(x, y, size, size, baseColor);

        // 2. ציור הארה (Highlight) - למעלה ושמאל
        DrawRectangle(x, y, size, shadeSize, highlightColor); // קו עליון
        DrawRectangle(x, y, shadeSize, size, highlightColor); // קו שמאלי
        
        // 3. ציור הצל (Shadow) - למטה וימין
        DrawRectangle(x, y + size - shadeSize, size, shadeSize, shadowColor); // קו תחתון
        DrawRectangle(x + size - shadeSize, y, shadeSize, size, shadowColor); // קו ימני
    }
}

void Block::Move(int rows, int columns){
    rowOffset += rows;
    columnOffset += columns;
}

std::vector<Position> Block::GetCellPositions(){
    std::vector<Position> tiles = cells[rotationState];
    std::vector<Position> movedTiles;
    for (Position item : tiles){
        Position newPos = Position(item.row + rowOffset, item.column + columnOffset);
        movedTiles.push_back(newPos);
    }
    return movedTiles;
}

void Block::Rotate(){
    rotationState++;
    if (rotationState == (int)cells.size()){
        rotationState = 0;
    }
}

void Block::UndoRotation(){
    rotationState--;
    if (rotationState == -1){
        rotationState = cells.size()-1;
    }
}
