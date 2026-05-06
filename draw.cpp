#include "draw.h"
#include "images.h"
#include "bitboard.h"
#include "moveGen.h"
#include "specialMove.h"
#include "constants.h"
using namespace BitBoard;

void Draw::drawBoard(){
    DrawRectangle(boardX, boardY, boardSize, boardSize, WHITE);
    for(int r = 0; r < 8; r++){
        for(int c = 0; c < 8; c++){
            if((r + c) & 1){
                DrawRectangle(boardX + c*cellSize, boardY + r*cellSize, cellSize, cellSize, BLUE);
            }
        }
    }
}



void Draw::drawPieces(){
    for(int sq = 0; sq <= 63; sq++){
        uint64_t mask = (1ULL << sq);
        int c = sq%8, r = 7 - sq/8;

        if(sq == State::prev_sq)
            DrawRectangle(boardX + c*cellSize, boardY + r*cellSize, cellSize, cellSize, Fade(YELLOW,0.8f));
        if(sq == State::curr_sq)
            DrawRectangle(boardX + c*cellSize, boardY + r*cellSize, cellSize, cellSize, Fade(YELLOW,0.8f));
        //if(moveGen::isKingCheck(State::getPiece[sq] >= 6) && sq == State::KingSq)
        if(State::isKingCheckBool && sq == State::KingSq)    
            DrawRectangle(boardX + c*cellSize, boardY + r*cellSize, cellSize, cellSize, Fade(RED,0.8f));
        
        // if(BitBoard::OCCUPIED[white] & mask)    
        //          DrawRectangle(boardX + c*cellSize, boardY + r*cellSize, cellSize, cellSize, MAGENTA);
        //      if(BitBoard::OCCUPIED[black] & mask)
        //          DrawRectangle(boardX + c*cellSize, boardY + r*cellSize, cellSize, cellSize, GREEN);
        

        if(OCCUPIED[either] & mask){
            // DrawRectangle(boardX + c*cellSize, boardY + r*cellSize, cellSize/2, cellSize/2, BLACK);
            

            if(mask & BITBOARD[W_PAWN])
                DrawTexture(Images::pieceTextures[W_PAWN], boardX + c*cellSize + padding, boardY + r*cellSize + padding, WHITE);
            else if(mask & BITBOARD[B_PAWN])
                DrawTexture(Images::pieceTextures[B_PAWN], boardX + c*cellSize + padding, boardY + r*cellSize + padding, WHITE);
            else if(mask & BITBOARD[W_KNIGHT])
                DrawTexture(Images::pieceTextures[W_KNIGHT], boardX + c*cellSize + padding, boardY + r*cellSize + padding, WHITE);
            else if(mask & BITBOARD[B_KNIGHT])
                DrawTexture(Images::pieceTextures[B_KNIGHT], boardX + c*cellSize + padding, boardY + r*cellSize + padding, WHITE);
            else if(mask & BITBOARD[W_BISHOP])
                DrawTexture(Images::pieceTextures[W_BISHOP], boardX + c*cellSize + padding, boardY + r*cellSize + padding, WHITE);
            else if(mask & BITBOARD[B_BISHOP])
                DrawTexture(Images::pieceTextures[B_BISHOP], boardX + c*cellSize + padding, boardY + r*cellSize + padding, WHITE);
            else if(mask & BITBOARD[W_ROOK])
                DrawTexture(Images::pieceTextures[W_ROOK], boardX + c*cellSize + padding, boardY + r*cellSize + padding, WHITE);
            else if(mask & BITBOARD[B_ROOK])
                DrawTexture(Images::pieceTextures[B_ROOK], boardX + c*cellSize + padding, boardY + r*cellSize + padding, WHITE);
            else if(mask & BITBOARD[W_QUEEN])
                DrawTexture(Images::pieceTextures[W_QUEEN], boardX + c*cellSize + padding, boardY + r*cellSize + padding, WHITE);
            else if(mask & BITBOARD[B_QUEEN])
                DrawTexture(Images::pieceTextures[B_QUEEN], boardX + c*cellSize + padding, boardY + r*cellSize + padding, WHITE);
            else if(mask & BITBOARD[W_KING])
                DrawTexture(Images::pieceTextures[W_KING], boardX + c*cellSize + padding, boardY + r*cellSize + padding, WHITE);
            else if(mask & BITBOARD[B_KING])
                DrawTexture(Images::pieceTextures[B_KING], boardX + c*cellSize + padding, boardY + r*cellSize + padding, WHITE);
            }
    }
}

void Draw::drawEvalBar(){
    float eval = Eval::evaluation;
    
}