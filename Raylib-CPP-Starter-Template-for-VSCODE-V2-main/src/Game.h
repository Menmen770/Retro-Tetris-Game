#pragma once
#include "Grid.h"
#include "Blocks.cpp"
#include "ScoreManager.h"
#include <raylib.h>

enum GameState {
    LEVEL_SELECT,
    PLAYING,
    GAME_OVER,
    ENTER_NAME,
    HIGH_SCORES
};

extern void ResetDropTimer();

class Game {
private:
    std::vector<Block> blocks;
    Block currentBlock;
    Block nextBlock;
    Grid grid;
    int totalLinesCleared;
    Sound clearSound;
    Sound gameOverSound;
    
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
    
public:
    Game();
    ~Game();
    
    void Draw();
    void HandleInput();
    void MoveBlockDown();
    
    bool GameOver;
    int currentLevel;
    int score;
    int lastScore;
    Texture2D logoTexture;
    GameState state;
    ScoreManager scoreManager;
    std::string playerName;
};
