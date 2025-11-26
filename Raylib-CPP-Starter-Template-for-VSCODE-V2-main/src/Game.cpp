#include "Game.h"
#include <random>
#include <iostream>

bool Game::IsBlockOutside()
{
    std::vector<Position> tiles = currentBlock.GetCellPositions();
    for (Position item : tiles){
        if (grid.IsCellOutside(item.row, item.column)){
            return true;
        }
    }
    return false;
}

void Game::RotateBlock(){
    if (!GameOver){
        currentBlock.Rotate();
        if (IsBlockOutside() || BlockFist() == false){
            currentBlock.UndoRotation();
        }
    }
}

void Game::LockBlock(){
    std::vector<Position> tiles = currentBlock.GetCellPositions();
    for (Position item: tiles){
        grid.grid[item.row][item.column] = currentBlock.id;
    }
    currentBlock = nextBlock;
    if (BlockFist() == false){
        GameOver = true;
        PlaySound(gameOverSound);
        lastScore = score; // שמירת הציון לפני איפוס
        state = GAME_OVER; // מעבר למסך Game Over
    }
    UpdateScore(0, 1);
    nextBlock = GetRandomBlock();
    int rowsCleared = grid.ClearFullRows();
    if (rowsCleared > 0){
        PlaySound(clearSound);
        UpdateScore(rowsCleared, 0);
    }
}

// Game.cpp

void Game::UpdateScore(int linesCleared, int moveDownPoints){
    int pointsPerLine = 0;
    
    // בסיס הניקוד (כמו בטטריס קלאסי)
    switch (linesCleared)
    {
    case 1:
        pointsPerLine = 40;
        break;

    case 2:
        pointsPerLine = 100;
        break;

    case 3:
        pointsPerLine = 300;
        break;

    case 4: // Tetris!
        pointsPerLine = 1200;
        break;
    
    default:
        pointsPerLine = 0;
        break;
    }
    
    // הניקוד הסופי הוא (בסיס הניקוד) * (הרמה הנוכחית)
    score += pointsPerLine * currentLevel;
    
    // ניקוד על ירידה רכה (soft drop)
    score += moveDownPoints;
    
    // --- לוגיקה לעדכון מהירות/רמה ---
    
    // 1. עדכון מונה השורות הכללי (בהנחה ש totalLinesCleared קיים ב-Game.h)
    totalLinesCleared += linesCleared;

    // 2. חישוב רמה חדשה (כל 10 שורות = עליית רמה)
    int newLevel = (totalLinesCleared / 10) + 1; 
    
    // הגבלת הרמה המקסימלית ל-15
    if (newLevel > 15) {
        newLevel = 15;
    }
    
    // 3. עדכון הרמה בפועל אם היא השתנתה
    if (newLevel > currentLevel) {
        currentLevel = newLevel; 
    }
}

bool Game::BlockFist(){
    std::vector<Position> tiles = currentBlock.GetCellPositions();
    for (Position item : tiles){
        if (grid.IsCellEmpty(item.row, item.column) == false){
            return false;
        }
    }
    return true;

}

Game::Game()
{
    grid = Grid();
    blocks = GetAllBlocks();
    currentBlock = GetRandomBlock();
    nextBlock = GetRandomBlock();
    GameOver = false;
    currentLevel = 1;
    score = 0;
    currentLevel = 1; // <--- מתחיל מרמה 1
    totalLinesCleared = 0;
    state = LEVEL_SELECT; // <--- מתחיל במסך בחירת רמה
    logoTexture = LoadTexture("resources/logo.png");
    InitAudioDevice();
    clearSound = LoadSound("Sounds/clear.mp3");
    gameOverSound = LoadSound("Sounds/gameOver.mp3");
    scoreManager = ScoreManager("highscores.txt");
    playerName = "";
    lastScore = 0;

}

Game::~Game(){
    UnloadSound(clearSound);
    UnloadSound(gameOverSound);
    UnloadTexture(logoTexture); // משחרר את התמונה מהזיכרון
    CloseAudioDevice();
}

Block Game::GetRandomBlock(){
    if (blocks.empty()){
        blocks = GetAllBlocks();
    }
    int randonIndex = rand() % blocks.size();
    Block block = blocks[randonIndex];
    blocks.erase(blocks.begin() + randonIndex);
    return block;
}

std::vector<Block> Game::GetAllBlocks(){
    return {IBlock(), JBlock(), LBlock(), OBlock(), SBlock(), TBlock(), ZBlock()};
}

void Game::Draw(){
    grid.Draw();
    currentBlock.Draw(12, 12);
    switch (nextBlock.id){
    case 3:
        nextBlock.Draw(280.5f, 319);
        break;

    case 4:
        nextBlock.Draw(280.5f, 308);
        break;

    default:
        nextBlock.Draw(297, 297);
        break;
    }
}

void Game::HandleInput(){
    int keyPressed = GetKeyPressed();
    
    // --- 1. טיפול במצב בחירת רמה ---
    if (state == LEVEL_SELECT) {
        if (keyPressed == KEY_UP) {
            // הגבלת הרמה המקסימלית ל-12
            currentLevel = std::min(12, currentLevel + 1); 
        } else if (keyPressed == KEY_DOWN) {
            // הגבלת הרמה המינימלית ל-1
            currentLevel = std::max(1, currentLevel - 1); 
        } else if (keyPressed == KEY_SPACE) {
            state = PLAYING; // מקש רווח מתחיל את המשחק
            Reset(); // איפוס הלוח והבלוקים
            ResetDropTimer();
        } else if (keyPressed == KEY_LEFT_CONTROL || keyPressed == KEY_RIGHT_CONTROL || keyPressed == KEY_H) {
            // CTRL או H פותח את טבלת השיאים
            state = HIGH_SCORES;
        }
        return; // סיים את הפונקציה אם אנחנו בבחירת רמה
    }

    // --- 2. טיפול במסך Game Over ---
    if (state == GAME_OVER) {
        if (keyPressed == KEY_SPACE || keyPressed == KEY_ENTER) {
            // בדיקה אם הציון נכנס לטופ 10
            if (scoreManager.IsHighScore(lastScore)) {
                state = ENTER_NAME; // מעבר למסך הזנת שם
                playerName = ""; // איפוס שם השחקן
            } else {
                state = HIGH_SCORES; // מעבר ישיר לטבלת השיאים
            }
        }
        return;
    }

    // --- 3. טיפול במסך הזנת שם ---
    if (state == ENTER_NAME) {
        // טיפול בקלט טקסט - אותיות, מספרים, מקום
        if (keyPressed >= 32 && keyPressed <= 125 && playerName.length() < 15) {
            playerName += (char)keyPressed;
        }
        
        // מחיקת תו אחרון
        if (keyPressed == KEY_BACKSPACE && playerName.length() > 0) {
            playerName.pop_back();
        }
        
        // אישור השם ושמירה
        if (keyPressed == KEY_ENTER && playerName.length() > 0) {
            scoreManager.AddScore(playerName, lastScore);
            state = HIGH_SCORES;
        }
        return;
    }

    // --- 4. טיפול במסך טבלת השיאים ---
    if (state == HIGH_SCORES) {
        if (keyPressed == KEY_SPACE || keyPressed == KEY_ENTER) {
            state = LEVEL_SELECT; // חזרה לתפריט
        }
        return;
    }
    
    // --- 5. טיפול במשחק פעיל (PLAYING) ---
    switch (keyPressed)
    {
    case KEY_LEFT:
        MoveBlockLeft();
        break;
    
    case KEY_RIGHT:
        MoveBlockRight();
        break;
    
    case KEY_DOWN: // <--- הטיפול ב-KEY_DOWN נשאר כקלט רגיל
        MoveBlockDown();
        break;

    case KEY_UP:
        RotateBlock();
        break;
        
    case KEY_SPACE: // <--- הפלה חזקה (Hard Drop)
        HardDropBlock();
        break;

    default:
        break;
    }
}

void Game::MoveBlockLeft(){
    if (!GameOver){
        currentBlock.Move(0, -1);
        if (IsBlockOutside() || BlockFist() == false)
            currentBlock.Move(0, 1);
    }
}

void Game::MoveBlockRight(){
    if (!GameOver){
        currentBlock.Move(0, 1);
        if (IsBlockOutside() || BlockFist() == false)
            currentBlock.Move(0, -1);
    }
}

void Game::MoveBlockDown(){
    if (!GameOver){
        currentBlock.Move(1, 0);
        if (IsBlockOutside() || BlockFist() == false){
            currentBlock.Move(-1, 0);
            LockBlock();
        }
    }
}

void Game::HardDropBlock(){
    if (GameOver) return; // אין מה להפיל אם המשחק נגמר

    // לולאה שמנסה להזיז את הבלוק שורה אחת למטה בכל איטרציה
    while (true) {
        currentBlock.Move(1, 0); // נסה להזיז למטה
        
        // אם המהלך נכשל (יצא מהגבולות או התנגש בבלוק אחר)
        if (IsBlockOutside() || BlockFist() == false) {
            
            // 1. החזר את הבלוק שורה אחת למעלה (למיקום הנכון האחרון)
            currentBlock.Move(-1, 0); 
            
            // 2. נעל את הבלוק וקדם את הבלוק הבא
            LockBlock();
            
            // 3. סיים את הפעולה
            return;
        }
        // אם המהלך הצליח, הלולאה ממשיכה לנסות שוב
    }
}

void Game::Reset(){
    grid.Initialize();
    blocks = GetAllBlocks();
    currentBlock = GetRandomBlock();
    nextBlock = GetRandomBlock();
    score = 0;
    totalLinesCleared = 0; // <--- איפוס מונה השורות
    GameOver = false; // <--- איפוס מצב Game Over
}
