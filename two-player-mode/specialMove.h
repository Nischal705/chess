#pragma once
#include "bitboard.h"
#include "moveGen.h"
#include "state.h"
#include <vector>
namespace specialMove{
    extern std::vector<Move> specialMoves;
    std::vector<Move> getEnpassants(int color, const Move& oppMove);
    bool isEnpassant(const Move& move);
    std::vector<Move> getCastles(int color);
    int canCastle(int color);
    bool isCastle(const Move& move);
    void makeCastle(const Move& move);
    void undoCastle(const Move& move);
    void makeEnpassant(const Move& move);
    void undoEnpassant(const Move& move);
}