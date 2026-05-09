#pragma once
#include "constants.h"
#include "player.h"
#include "moveGen.h"
#include "bitboard.h"
#include <cstdint>

namespace State{
    inline int getPiece[64] = {W_ROOK, W_KNIGHT, W_BISHOP,  W_QUEEN, W_KING, W_BISHOP, W_KNIGHT, W_ROOK,
                       W_PAWN, W_PAWN, W_PAWN, W_PAWN, W_PAWN, W_PAWN, W_PAWN, W_PAWN,
                       EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
                         EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
                         EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
                        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
                        B_PAWN, B_PAWN, B_PAWN, B_PAWN, B_PAWN, B_PAWN, B_PAWN, B_PAWN,
                        B_ROOK, B_KNIGHT, B_BISHOP,  B_QUEEN, B_KING, B_BISHOP, B_KNIGHT, B_ROOK
                    };                 
    inline bool isKingCheckBool = false;
    inline int KingSq = -1;
    inline int prev_sq = -1, curr_sq = -1;
    inline Move oppMove;
    inline uint64_t pieceNotMoved = 0xFFFF00000000FFFF;
    inline bool moveCompleted = false;
    
    inline bool isCheckmate = false; 
    inline bool isStalemate = false;
    inline Human human(0);
    inline Engine engine(1);
    inline Player* currPlayer = &human;
    inline void switchTurn(){
        if(currPlayer->color == human.color)
            currPlayer = &engine;
        else    
            currPlayer = &human;
    }
    void updateStateVariables(const Move& moveMade);

    inline int materialCount[2] = {39, 39};
    void updateMaterialCount(const Move& move);
    void restoreMaterialCount(const Move& move);
}