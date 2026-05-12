#pragma once
#include <cstdint>
#include "state.h"
#include "moveGen.h"
namespace BitBoard{
    extern uint64_t BITBOARD[12] ;
    extern uint64_t OCCUPIED[3];
        
    extern void addPiece(int piece, int sq);
    extern void removePiece(int piece, int sq);
}
/*
namespace State{
    extern int getPiece[64];
    extern bool isKingCheckBool;
    extern int KingSq, prev_sq, curr_sq;
    extern Move oppMove;
    extern uint64_t pieceNotMoved;
}*/