#pragma once
#include "raylib.h"
#include "moveGen.h"
#include <vector>
class Input{
    private:
    inline static int currPiece;
    public:
    static void handleInput();
    static int findSquare(Vector2 mousePos);
    static void dragPiece(int piece, Vector2 mousePos);

    static bool isLegal(const Move&);
    static void makeMove(const Move&);
    static void undoMove(const Move&);
};