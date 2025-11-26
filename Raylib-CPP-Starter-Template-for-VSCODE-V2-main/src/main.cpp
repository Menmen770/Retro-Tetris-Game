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
    
    InitWindow(550, 682, "Retro Tetris");
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
        game.HandleInput();
        
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
                (550.0f - 495.0f) / 2.0f,  // X (ממורכז)
                44.0f,                     // Y (למעלה)
                495.0f,                    // רוחב (גדול)
                330.0f                     // גובה (גדול)
            };
            DrawTexturePro(game.logoTexture, sourceRect, destRect, {0,0}, 0.0f, WHITE);
            
            // --- 2. מיקום הטקסט (מתחת ללוגו, קטן וממורכז) ---
            
            // מיקום התחלה לטקסט, מתחת לתיבת התמונה
            float textY = 44.0f + 330.0f + 11.0f; // Y=385

            // שם המשחק (כותרת) - הוקטן וממורכז
            float fontSize = 33.0f;
            Vector2 textSize = MeasureTextEx(font, "RETRO TETRIS", fontSize, 2);
            DrawTextEx(font, "RETRO TETRIS", {(550.0f - textSize.x) / 2.0f, textY}, fontSize, 2, WHITE);

            // "Select Level" - הוקטן וממורכז
            textY += 44.0f; // Y=429
            fontSize = 33.0f;
            textSize = MeasureTextEx(font, "Select Level", fontSize, 2);
            DrawTextEx(font, "Select Level", {(550.0f - textSize.x) / 2.0f, textY}, fontSize, 2, WHITE);
            
            // "LEVEL %d" - ממורכז
            textY += 44.0f; // Y=473
            fontSize = 44.0f;
            char levelText[10];
            sprintf(levelText, "LEVEL %d", game.currentLevel);
            textSize = MeasureTextEx(font, levelText, fontSize, 2);
            DrawTextEx(font, levelText, {(550.0f - textSize.x) / 2.0f, textY}, fontSize, 2, WHITE);

            // הוראות (קרוב לתחתית המסך) - הוקטנו וממורכזות
            fontSize = 22.0f;
            textSize = MeasureTextEx(font, "Use Up/Down to Select", fontSize, 2);
            DrawTextEx(font, "Use Up/Down to Select", {(550.0f - textSize.x) / 2.0f, 594.0f}, fontSize, 2, LIGHTGRAY);
            
            fontSize = 26.4f;
            textSize = MeasureTextEx(font, "Press SPACE to Play", fontSize, 2);
            DrawTextEx(font, "Press SPACE to Play", {(550.0f - textSize.x) / 2.0f, 627.0f}, fontSize, 2, YELLOW);
            
            // הוראה לצפייה בשיאים
            fontSize = 19.8f;
            textSize = MeasureTextEx(font, "Press CTRL or H for High Scores", fontSize, 2);
            DrawTextEx(font, "Press CTRL or H for High Scores", {(550.0f - textSize.x) / 2.0f, 660.0f}, fontSize, 2, DARKGRAY);

        } else if (game.state == GAME_OVER) {
            
            // --- מסך Game Over ---
            float fontSize = 66.0f;
            Vector2 textSize = MeasureTextEx(font, "GAME OVER", fontSize, 2);
            DrawTextEx(font, "GAME OVER", {(550.0f - textSize.x) / 2.0f, 220.0f}, fontSize, 2, red);
            
            // הצגת הציון הסופי
            char scoreText[50];
            sprintf(scoreText, "Final Score: %d", game.lastScore);
            fontSize = 38.5f;
            textSize = MeasureTextEx(font, scoreText, fontSize, 2);
            DrawTextEx(font, scoreText, {(550.0f - textSize.x) / 2.0f, 330.0f}, fontSize, 2, WHITE);
            
            // הוראות
            fontSize = 26.4f;
            textSize = MeasureTextEx(font, "Press SPACE to continue", fontSize, 2);
            DrawTextEx(font, "Press SPACE to continue", {(550.0f - textSize.x) / 2.0f, 440.0f}, fontSize, 2, LIGHTGRAY);

        } else if (game.state == ENTER_NAME) {
            
            // --- מסך הזנת שם לאחר השגת שיא ---
            float fontSize = 44.0f;
            Vector2 textSize;
            
            // כותרת
            textSize = MeasureTextEx(font, "NEW HIGH SCORE!", fontSize, 2);
            DrawTextEx(font, "NEW HIGH SCORE!", {(550.0f - textSize.x) / 2.0f, 165.0f}, fontSize, 2, YELLOW);
            
            // הצגת הציון
            char scoreText[50];
            sprintf(scoreText, "Score: %d", game.lastScore);
            fontSize = 38.5f;
            textSize = MeasureTextEx(font, scoreText, fontSize, 2);
            DrawTextEx(font, scoreText, {(550.0f - textSize.x) / 2.0f, 242.0f}, fontSize, 2, WHITE);
            
            // הנחיה
            fontSize = 27.5f;
            textSize = MeasureTextEx(font, "Enter Your Name:", fontSize, 2);
            DrawTextEx(font, "Enter Your Name:", {(550.0f - textSize.x) / 2.0f, 330.0f}, fontSize, 2, WHITE);
            
            // תיבת טקסט לשם
            DrawRectangleRounded({110, 385, 330, 66}, 0.3, 6, lightBlue);
            
            // הצגת השם שהוזן
            fontSize = 38.5f;
            const char* displayName = game.playerName.empty() ? "_" : game.playerName.c_str();
            textSize = MeasureTextEx(font, displayName, fontSize, 2);
            DrawTextEx(font, displayName, {275 - textSize.x / 2, 401.5f}, fontSize, 2, WHITE);
            
            // הוראות
            fontSize = 22.0f;
            textSize = MeasureTextEx(font, "Press ENTER to confirm", fontSize, 2);
            DrawTextEx(font, "Press ENTER to confirm", {(550.0f - textSize.x) / 2.0f, 528.0f}, fontSize, 2, LIGHTGRAY);
            
        } else if (game.state == HIGH_SCORES) {
            
            // --- מסך טבלת השיאים ---
            float fontSize = 49.5f;
            Vector2 textSize = MeasureTextEx(font, "HIGH SCORES", fontSize, 2);
            DrawTextEx(font, "HIGH SCORES", {(550.0f - textSize.x) / 2.0f, 33.0f}, fontSize, 2, YELLOW);
            
            // קבלת רשימת השיאים
            std::vector<ScoreEntry> topScores = game.scoreManager.GetTop10();
            
            float startY = 110.0f;
            fontSize = 27.5f;
            
            // הצגת הטבלה
            for (size_t i = 0; i < topScores.size() && i < 10; i++) {
                char line[100];
                sprintf(line, "%2d. %-12s %6d", (int)(i + 1), topScores[i].name.c_str(), topScores[i].score);
                
                // הדגשת הציון האחרון שנוסף
                Color textColor = WHITE;
                if (topScores[i].score == game.lastScore && topScores[i].name == game.playerName) {
                    textColor = YELLOW;
                }
                
                DrawTextEx(font, line, {55.0f, startY + i * 49.5f}, fontSize, 2, textColor);
            }
            
            // הודעה אם אין שיאים
            if (topScores.empty()) {
                fontSize = 33.0f;
                textSize = MeasureTextEx(font, "No scores yet!", fontSize, 2);
                DrawTextEx(font, "No scores yet!", {(550.0f - textSize.x) / 2.0f, 330.0f}, fontSize, 2, LIGHTGRAY);
            }
            
            // הוראות
            fontSize = 26.4f;
            textSize = MeasureTextEx(font, "Press SPACE to continue", fontSize, 2);
            DrawTextEx(font, "Press SPACE to continue", {(550.0f - textSize.x) / 2.0f, 627.0f}, fontSize, 2, LIGHTGRAY);
            
        } else if (game.state == PLAYING) {
            
            // --- ציור ממשק המשחק הרגיל ---
            DrawTextEx(font, "Score", {401.5f, 16.5f}, 41.8f, 2, WHITE);
            DrawTextEx(font, "Next", {407.0f, 192.5f}, 41.8f, 2, WHITE);
            
            char levelDisplay[15];
            sprintf(levelDisplay, "Level %d", game.currentLevel);
            DrawTextEx(font, levelDisplay, {396.0f, 440.0f}, 26.4f, 2, WHITE); 

            DrawRectangleRounded({352, 60.5f, 187, 66}, 0.3, 6, lightBlue);
            
            char scoreText[10];
            sprintf(scoreText, "%d", game.score);
            Vector2 textSize = MeasureTextEx(font, scoreText, 41.8f, 2);

            DrawTextEx(font, scoreText, {352 + (187 - textSize.x)/2, 71.5f}, 41.8f, 2, WHITE);
            DrawRectangleRounded({352, 236.5f, 187, 198}, 0.3, 6, lightBlue);
            
            game.Draw();
        }

        EndDrawing();
    }
    
    CloseWindow();
}