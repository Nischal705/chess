#pragma once
#include "moveGen.h"

class AlphaBeta{
    private:
        inline static int max_depth = 6; //keep it even 
        inline static float inf = 1000;
        inline static Move finalMove;
        inline static int engineColor = 1; //don't keep this here later
        inline static float trueEval;
    public:
        static float alphaBeta(float alpha, float beta, int depth, int color);
        static void engineThink(int color);
        static Move getBestMove();
        static float getTrueEval();
};