#include "ScoreManager.h"
#include <fstream>
#include <algorithm>
#include <iostream>

ScoreManager::ScoreManager(std::string file) : filename(file) {
    LoadFromFile();
    
    // אם הקובץ ריק או לא קיים, נאתחל עם רשימת שיאים מוכנה מראש
    if (highScores.empty()) {
        highScores.push_back(ScoreEntry("mot.n.t", 4812300));
        highScores.push_back(ScoreEntry("menmen", 4110900));
        highScores.push_back(ScoreEntry("Igor", 3998440));
        highScores.push_back(ScoreEntry("Shmuel", 3502780));
        highScores.push_back(ScoreEntry("Oded", 2944600));
        highScores.push_back(ScoreEntry("Eitan", 2881300));
        highScores.push_back(ScoreEntry("Nattai", 2770400));
        highScores.push_back(ScoreEntry("Dvir", 2556900));
        highScores.push_back(ScoreEntry("Shneor", 2410200));
        highScores.push_back(ScoreEntry("LeviYitzchak", 2288750));
        
        // שמירת הרשימה המוכנה מראש לקובץ
        SaveToFile();
    }
}

void ScoreManager::LoadFromFile() {
    highScores.clear();
    std::ifstream inFile(filename);
    
    if (!inFile.is_open()) {
        // אם הקובץ לא קיים, נתחיל עם רשימה ריקה
        return;
    }
    
    std::string name;
    int score;
    
    // קריאת כל שורה בפורמט: שם,ציון
    while (inFile >> name >> score) {
        highScores.push_back(ScoreEntry(name, score));
    }
    
    inFile.close();
    SortScores();
}

void ScoreManager::SaveToFile() {
    std::ofstream outFile(filename);
    
    if (!outFile.is_open()) {
        std::cerr << "Error: Could not open file for writing: " << filename << std::endl;
        return;
    }
    
    // שמירת עד 10 ציונים בלבד
    int count = std::min(10, (int)highScores.size());
    for (int i = 0; i < count; i++) {
        outFile << highScores[i].name << " " << highScores[i].score << "\n";
    }
    
    outFile.close();
}

void ScoreManager::SortScores() {
    // מיון מהגבוה לנמוך
    std::sort(highScores.begin(), highScores.end(), 
        [](const ScoreEntry& a, const ScoreEntry& b) {
            return a.score > b.score;
        });
}

bool ScoreManager::IsHighScore(int score) {
    // אם יש פחות מ-10 ציונים, כל ציון חדש נכנס
    if (highScores.size() < 10) {
        return true;
    }
    
    // אחרת, בודקים אם הציון גבוה מהציון העשירי
    return score > highScores[9].score;
}

void ScoreManager::AddScore(std::string name, int score) {
    // הוספת הציון החדש
    highScores.push_back(ScoreEntry(name, score));
    
    // מיון מחדש
    SortScores();
    
    // שמירה על 10 ציונים בלבד
    if (highScores.size() > 10) {
        highScores.resize(10);
    }
    
    // שמירה לקובץ
    SaveToFile();
}

std::vector<ScoreEntry> ScoreManager::GetTop10() {
    return highScores;
}

int ScoreManager::GetLowestHighScore() {
    if (highScores.empty()) {
        return 0;
    }
    
    if (highScores.size() < 10) {
        return 0; // אם יש פחות מ-10, כל ציון נכנס
    }
    
    return highScores[9].score;
}
