#include <iostream>
#include "raylib.h"
#include "constants.h"
#include "draw.h"
#include "player.h"
#include "state.h"
#include "bitboard.h"
#include "images.h"
#include "input.h"
#include "moveGen.h"
#include "alphabeta.h"
//g++ main.cpp draw.cpp images.cpp bitboard.cpp input.cpp moveGen.cpp specialMove.cpp player.cpp state.cpp eval.cpp alphabeta.cpp -lraylib -lopengl32 -lgdi32 -lwinmm -o chess.exe
int main(){
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Chess");
    SetTargetFPS(FPS);
    Images::loadTexture();
    moveGen::init();
    while(!WindowShouldClose()){
        ClearBackground(BLACK);
        BeginDrawing();
        Draw::drawBoard();
        
        State::currPlayer->playMove();
        
        Draw::drawPieces();
        EndDrawing(); 
    
    }

    CloseWindow();
    return 0;
}