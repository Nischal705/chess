#include "images.h"
#include "raylib.h"
#include "constants.h"

void Images::loadTexture(){
    Image pieceImages[12];
    pieceImages[W_PAWN] = LoadImage("pieces/white_pawn.png");
    pieceImages[W_KNIGHT] = LoadImage("pieces/white_horse.png");
    pieceImages[W_BISHOP] = LoadImage("pieces/white_bishop.png");
    pieceImages[W_ROOK] = LoadImage("pieces/white_rook.png");
    pieceImages[W_KING] = LoadImage("pieces/white_king.png");
    pieceImages[W_QUEEN] = LoadImage("pieces/white_queen.png");
    pieceImages[B_PAWN] = LoadImage("pieces/black_pawn.png");
    pieceImages[B_KNIGHT] = LoadImage("pieces/black_horse.png");
    pieceImages[B_BISHOP] = LoadImage("pieces/black_bishop.png");
    pieceImages[B_ROOK] = LoadImage("pieces/black_rook.png");
    pieceImages[B_KING] = LoadImage("pieces/black_king.png");
    pieceImages[B_QUEEN] = LoadImage("pieces/black_queen.png");

    for(int i = 0; i < 12; i++){
        ImageResize(&pieceImages[i], cellSize - 2*padding, cellSize - 2*padding);
        pieceTextures[i] = LoadTextureFromImage(pieceImages[i]);
    }
}