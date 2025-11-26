#include "Colors.h"
#include <vector>

const Color darkGrey = {26, 31, 40, 255};
const Color green = {47, 230, 23, 255};
const Color red = {232, 18, 18, 255};
const Color orenge = {226, 116, 17, 255};
const Color yellow = {237, 234, 4, 255};
const Color purple = {166, 0, 247, 255};
const Color cyan = {21, 204, 209, 255};
const Color blue = {13, 64, 216, 255};
const Color lightBlue = {59, 85, 162, 255};
const Color darkBlue = {44, 44, 127, 255};
// דוגמה לצבעי הרקע הרצויים
const Color darkGridBackground = {10, 10, 50, 255}; // רקע כחול כהה (החלק המלא)
const Color gridLinesColor = {30, 30, 90, 255}; // קווי רשת כחולים מעט בהירים יותר

std::vector<Color> GetCellColors(){
    return {darkGrey, green, red, orenge, yellow, purple, cyan, blue};
}

// פונקציית עזר להכהיית צבע (Shadow)
Color GetShadowColor(Color baseColor) {
    return (Color){
        (unsigned char)(baseColor.r * 0.7f),
        (unsigned char)(baseColor.g * 0.7f),
        (unsigned char)(baseColor.b * 0.7f),
        255
    };
}

// פונקציית עזר להבהרת צבע (Highlight)
Color GetHighlightColor(Color baseColor) {
    return (Color){
        (unsigned char)std::min(255, (int)(baseColor.r * 1.3f)),
        (unsigned char)std::min(255, (int)(baseColor.g * 1.3f)),
        (unsigned char)std::min(255, (int)(baseColor.b * 1.3f)),
        255
    };
}