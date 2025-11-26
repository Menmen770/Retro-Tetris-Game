#pragma once
#include <string>
#include <vector>

// מבנה לשמירת רשומת ציון אחת
struct ScoreEntry {
    std::string name;
    int score;
    
    ScoreEntry() : name(""), score(0) {} // Default constructor
    ScoreEntry(std::string n, int s) : name(n), score(s) {}
};

class ScoreManager {
private:
    std::vector<ScoreEntry> highScores;
    std::string filename;
    
    void SortScores(); // מיון הציונים מהגבוה לנמוך
    
public:
    ScoreManager(std::string file = "highscores.txt");
    
    void LoadFromFile();  // טעינת ציונים מהקובץ
    void SaveToFile();    // שמירת ציונים לקובץ
    
    bool IsHighScore(int score); // בדיקה אם הציון נכנס לטופ 10
    void AddScore(std::string name, int score); // הוספת ציון חדש
    
    std::vector<ScoreEntry> GetTop10(); // קבלת רשימת 10 הציונים הגבוהים
    int GetLowestHighScore(); // קבלת הציון הנמוך ביותר בטופ 10
};
