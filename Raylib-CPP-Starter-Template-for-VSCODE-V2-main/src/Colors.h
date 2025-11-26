#pragma once
#include "raylib.h"
#include <vector>

Color GetShadowColor(Color baseColor);
Color GetHighlightColor(Color baseColor);

extern const Color darkGrey;
extern const Color green;
extern const Color red;
extern const Color orange;
extern const Color yellow;
extern const Color purple;
extern const Color cyan;
extern const Color blue;
extern const Color lightBlue;
extern const Color darkBlue;
extern const Color darkGridBackground;
extern const Color gridLinesColor;

std::vector<Color> GetCellColors();

