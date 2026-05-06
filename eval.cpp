#include "eval.h"

float Eval::considerMaterial(){
    int diff = State::materialCount[white] - State::materialCount[black];
    return (float)diff;
}

float Eval::considerOccupancy(){
    float white_control = __builtin_popcount(moveGen::allAttackedSquares[white]);
    float black_control = __builtin_popcount(moveGen::allAttackedSquares[black]);
    float diff = (white_control - black_control) * 0.08; //assuming each extra square gives an advantage of 0.2
    return diff;
}

float Eval::considerKingSafety(){
    int white_king_s = __builtin_ctzll(BitBoard::BITBOARD[W_KING]);
    int black_king_s = __builtin_ctzll(BitBoard::BITBOARD[B_KING]);
    //evalute based on if the neighborhood is attacked by opponent or safe
    uint64_t surrounding_white = moveGen::kingAttacks[white_king_s];
    uint64_t surrounding_black = moveGen::kingAttacks[black_king_s];
    //find intersection with the oppnent's attack and find the  population
    int danger_white_sq = __builtin_popcount(surrounding_white & moveGen::allAttackedSquares[black]);
    int danger_black_sq = __builtin_popcount(surrounding_black & moveGen::allAttackedSquares[white]);
    float diff = (danger_white_sq - danger_black_sq) * 0.08;
    return -diff; //if white's king is in more danger than black has advantage
}   

float Eval::considerDevelopment(){
    //simple implementation: just evaluate based on if the pieces have moved from their home squares or not
    static uint64_t home_setup_white = 0xFFFFULL;
    static uint64_t home_setup_black = 0xFFFF000000000000ULL;
    int n_unmoved_white = __builtin_popcount(home_setup_white & State::pieceNotMoved);
    int n_unmoved_black = __builtin_popcount(home_setup_black & State::pieceNotMoved);
    float diff = (n_unmoved_white - n_unmoved_black) * 0.08;
    return -diff; //if whtie has more unmoved pieces then black has advantage
}

float Eval::considerCenterControl(){
    static uint64_t center_mask = 0x3838383800000000; // from c3 to f6
    int n_white_control = __builtin_popcount(center_mask & BitBoard::OCCUPIED[white]);
    int n_black_control = __builtin_popcount(center_mask & BitBoard::OCCUPIED[black]);
    float diff = (n_white_control - n_black_control) * 0.2;
    return diff;
}

float Eval::getEval(){
    evaluation = considerMaterial() + considerOccupancy() + considerKingSafety() + considerDevelopment() + considerCenterControl();
    return evaluation;
}