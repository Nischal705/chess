#pragma once
#include <cstdint>
#include <iostream>
#include <vector>
struct Move{
        int from;
        int to;
        int piece;
        int capturedPiece;

        bool operator==(const Move& other) const{
            return (this->from == other.from)&&(this->to == other.to);
        }
};
namespace moveGen{
    extern uint64_t knightAttacks[64];
    extern uint64_t kingAttacks[64];
    extern uint64_t allAttackedSquares[2];
    extern uint64_t allDefendedSquares[2];
    extern std::vector<Move> allMoves;

    void init();
    void init_KnightAttacks();
    void init_KingAttacks();
    void init_allMoves(int color);
    void print_board();
    bool isKingCheck(int color);
    std::vector<Move> getAllMoves(int color);
}

