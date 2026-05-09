#pragma once
#include "raylib.h"
#include "constants.h"
#include "eval.h"
class Draw{
    private:
        inline static float evalBarWidth = cellSize/3;
        inline static float evalBarPadding = cellSize/5;
        inline static bool isSwitchOn = true;
        inline static Rectangle buttonRectangle = {cellSize/4, cellSize/3, cellSize*0.7, cellSize/3};
    public:
        static void drawBoard();
        static void drawPieces();
        static void drawEvalBar();
        static void handleEvalBarVisibility();
        static void drawSwitch();
};