#include <iostream>
#include <raylib.h>
#include "Game.h"
#include "Colors.h" 

double lastUpdateTime = 0;
void ResetDropTimer() { // <--- הוספת הפונקציה
    lastUpdateTime = GetTime(); 
}

bool EvenTriger(double interval){
    double currenTime = GetTime();
    if (currenTime - lastUpdateTime >= interval){
        lastUpdateTime = currenTime;
        return true;
    }
    return false;
}

double GetDropInterval(int level) {
    // טבלת מהירויות סטנדרטית לרמות 1 עד 5.
    // הערכים הם בשניות (ערך נמוך = מהיר יותר).
    static const double speedTable[] = {
        0.800, // רמה 1 (אינדקס 0)
        0.717, // רמה 2
        0.633, // רמה 3
        0.550, // רמה 4
        0.467, // רמה 5
        0.383, // רמה 6
        0.300, // רמה 7
        0.217, // רמה 8
        0.200, // רמה 9
        0.183, // רמה 10
        0.167, // רמה 11
        0.150, // רמה 12
    };
    
    // קביעת טווח הרמות (1 עד 5)
    const int MAX_LEVEL = 12;
    const int MIN_LEVEL = 1;

    // אם הרמה לא תקינה (למשל, 0 או מעל 5), נחזיר מהירות התחלה בטוחה (איטית).
    if (level < MIN_LEVEL || level > MAX_LEVEL) {
        return 0.800; 
    }
    
    // רמה 1 תשתמש באינדקס 0, רמה 5 תשתמש באינדקס 4.
    return speedTable[level - 1];
}

int main () {
    
    InitWindow(500, 620, "Retro Tetris");
    // טוען את התמונה מהקובץ (ודא שהנתיב "resources/logo.png" נכון)
    Image icon = LoadImage("resources/logo.png"); 
    
    // מגדיר את האייקון של החלון
    SetWindowIcon(icon);
    
    // משחרר את התמונה מהזיכרון (כי Raylib יצר עותק)
    UnloadImage(icon);
    SetTargetFPS(60);
    Font font = LoadFontEx("Font/monogram.ttf", 64, 0, 0); 
    Game game = Game();

    while (WindowShouldClose() == false)
    {
        game.HandelInput();
        
        if (game.state == PLAYING) {
            if (EvenTriger(GetDropInterval(game.currentLevel))){
                game.MoveBlockDown();
            }
        }
        
        BeginDrawing();
        ClearBackground(darkBlue);

        // --- ציור בהתאם למצב המשחק ---
        if (game.state == LEVEL_SELECT) {
            
            // --- 1. ציור התמונה (לוגו) - למעלה ובגדול ---
            Rectangle sourceRect = { 0.0f, 0.0f, (float)game.logoTexture.width, (float)game.logoTexture.height };
            Rectangle destRect = { 
                (500.0f - 450.0f) / 2.0f,  // X (ממורכז)
                40.0f,                     // Y (למעלה)
                450.0f,                    // רוחב (גדול)
                300.0f                     // גובה (גדול)
            };
            DrawTexturePro(game.logoTexture, sourceRect, destRect, {0,0}, 0.0f, WHITE);
            
            // --- 2. מיקום הטקסט (מתחת ללוגו, קטן וממורכז) ---
            
            // מיקום התחלה לטקסט, מתחת לתיבת התמונה
            float textY = 40.0f + 300.0f + 10.0f; // Y=350 

            // שם המשחק (כותרת) - הוקטן וממורכז
            float fontSize = 30.0f;
            Vector2 textSize = MeasureTextEx(font, "RETRO TETRIS", fontSize, 2);
            DrawTextEx(font, "RETRO TETRIS", {(500.0f - textSize.x) / 2.0f, textY}, fontSize, 2, WHITE);

            // "Select Level" - הוקטן וממורכז
            textY += 40.0f; // Y=390
            fontSize = 30.0f;
            textSize = MeasureTextEx(font, "Select Level", fontSize, 2);
            DrawTextEx(font, "Select Level", {(500.0f - textSize.x) / 2.0f, textY}, fontSize, 2, WHITE);
            
            // "LEVEL %d" - ממורכז
            textY += 40.0f; // Y=430
            fontSize = 40.0f;
            char levelText[10];
            sprintf(levelText, "LEVEL %d", game.currentLevel);
            textSize = MeasureTextEx(font, levelText, fontSize, 2);
            DrawTextEx(font, levelText, {(500.0f - textSize.x) / 2.0f, textY}, fontSize, 2, WHITE);

            // הוראות (קרוב לתחתית המסך) - הוקטנו וממורכזות
            fontSize = 20.0f;
            textSize = MeasureTextEx(font, "Use Up/Down to Select", fontSize, 2);
            DrawTextEx(font, "Use Up/Down to Select", {(500.0f - textSize.x) / 2.0f, 540.0f}, fontSize, 2, LIGHTGRAY); // Y=540
            
            fontSize = 24.0f;
            textSize = MeasureTextEx(font, "Press SPACE to Play", fontSize, 2);
            DrawTextEx(font, "Press SPACE to Play", {(500.0f - textSize.x) / 2.0f, 570.0f}, fontSize, 2, YELLOW); // Y=570

        } else if (game.state == PLAYING) {
            
            // --- ציור ממשק המשחק הרגיל ---
            DrawTextEx(font, "Score", {365, 15}, 38, 2, WHITE);
            DrawTextEx(font, "Next", {370, 175}, 38, 2, WHITE);
            
            char levelDisplay[15];
            sprintf(levelDisplay, "Level %d", game.currentLevel);
            DrawTextEx(font, levelDisplay, {360, 400}, 24, 2, WHITE); 

            if(game.GameOver){
                DrawTextEx(font, "Game", {360, 450}, 50, 2, red);
                DrawTextEx(font, "Over", {360, 500}, 50, 2, red);
            }
            
            DrawRectangleRounded({320, 55, 170, 60}, 0.3, 6, lightBlue);
            
            char scoreText[10];
            sprintf(scoreText, "%d", game.score);
            Vector2 textSize = MeasureTextEx(font, scoreText, 38, 2);

            DrawTextEx(font, scoreText, {320 + (170 - textSize.x)/2, 65}, 38, 2, WHITE);
            DrawRectangleRounded({320, 215, 170, 180}, 0.3, 6, lightBlue);
            
            game.Draw();
        }

        EndDrawing();
    }
    
    CloseWindow();
}