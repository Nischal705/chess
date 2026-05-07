#pragma once
#include "moveGen.h"

class Minimax{
    private:
        inline static int max_depth = 4; //keep it even 
        inline static Move finalMove;
        inline static int engineColor = 1;
        inline static float trueEval;
    public:
        static float minimax(int depth, int color);
        static Move getBestMove();
        static float getTrueEval();
};