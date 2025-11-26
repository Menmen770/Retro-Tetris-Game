#pragma once
#include "Grid.h"
#include "Blocks.cpp"
#include "ScoreManager.h"
#include <raylib.h>

// 1. הגדרת מצבי המשחק
enum GameState {
    LEVEL_SELECT, // מסך בחירת רמה
    PLAYING,      // משחק פעיל
    ENTER_NAME,   // הזנת שם לאחר השגת שיא
    HIGH_SCORES   // תצוגת טבלת השיאים
};
extern void ResetDropTimer();

class Game{
private:
    std::vector<Block> blocks;
    Block currentBlock;
    Block nextBlock;
    bool IsBlockOutside();
    void RotateBlock();
    void LockBlock();
    void UpdateScore(int linesCleared, int moveDownPoints);
    bool BlockFist();
    void Reset();
    Block GetRandomBlock();
    std::vector<Block> GetAllBlocks();
    void MoveBlockLeft();
    void MoveBlockRight();
    void HardDropBlock();
    Grid grid;
    // 2. משתנים חדשים לניהול המצב והרמה:
    int totalLinesCleared;
    Sound clearSound;
    Sound GameOverSound;
    
public:
    Game();
    ~Game();
    void Draw();
    void HandelInput();
    void MoveBlockDown();
    bool GameOver;
    int currentLevel; // <--- הרמה הנוכחית (1-5)
    Texture2D logoTexture;
    GameState state;  // <--- מצב המשחק הנוכחי
    int score;
    ScoreManager scoreManager;
    std::string playerName;
    int lastScore; // שמירת הציון האחרון לאחר game over
    
    
};