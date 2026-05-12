#include "eval.h"
#include "alphabeta.h"
#include "raylib.h"
#include <cmath>

float Eval::considerMaterial(){
    int diff = State::materialCount[white] - State::materialCount[black];
    return (float)diff;
}

float Eval::considerOccupancy(){
    float white_control = __builtin_popcountll(moveGen::allAttackedSquares[white]);
    float black_control = __builtin_popcountll(moveGen::allAttackedSquares[black]);
    float diff = (white_control - black_control) * 0.07; //assuming each extra square gives an advantage of 0.2
    return diff;
}

float Eval::considerKingSafety(){
    int white_king_s = __builtin_ctzll(BitBoard::BITBOARD[W_KING]);
    int black_king_s = __builtin_ctzll(BitBoard::BITBOARD[B_KING]);

    //testing if it can castle or not
    // if(black_king_s == 57)
    //      return -10;
    // //testing if it can play enpassant 
    // if(State::getPiece[17] == B_PAWN || State::getPiece[17] == W_PAWN);
    //     return -10;
    // if(State::getPiece[32] == B_PAWN)
    //     return -3;
    // if(State::getPiece[24] == B_PAWN)
    //     return -5;
    
    //evalute based on if the neighborhood is attacked by opponent or safe
    uint64_t surrounding_white = moveGen::kingAttacks[white_king_s];
    uint64_t surrounding_black = moveGen::kingAttacks[black_king_s];
    //find intersection with the oppnent's attack and find the  population
    int danger_white_sq = __builtin_popcountll(surrounding_white & moveGen::allAttackedSquares[black]);
    int danger_black_sq = __builtin_popcountll(surrounding_black & moveGen::allAttackedSquares[white]);
    float diff = (danger_white_sq - danger_black_sq) * 0.03;
    return -diff; //if white's king is in more danger than black has advantage
}   

float Eval::considerDevelopment(){
    //simple implementation: just evaluate based on if the pieces have moved from their home squares or not
    static uint64_t home_setup_white = 0b11100111ULL;
    static uint64_t home_setup_black = 0xE700000000000000ULL; //not including the queen and king for no reason
    int n_unmoved_white = __builtin_popcountll(home_setup_white & State::pieceNotMoved);
    int n_unmoved_black = __builtin_popcountll(home_setup_black & State::pieceNotMoved);
    float diff = (n_unmoved_white - n_unmoved_black) * 0.13;
    return -diff; //if whtie has more unmoved pieces then black has advantage
}

float Eval::considerCenterControl(){
    static uint64_t center_mask = 0x3838383800000000; // from c3 to f6
    int n_white_control = __builtin_popcountll(center_mask & BitBoard::OCCUPIED[white]);
    int n_black_control = __builtin_popcountll(center_mask & BitBoard::OCCUPIED[black]);
    float diff = (n_white_control - n_black_control) * 0.09;
    return diff;
}

float Eval::getEval(){
    return considerMaterial() + considerOccupancy() + considerKingSafety()  + considerDevelopment()  + considerCenterControl();
}

float Eval::getEvalHeight(){
    float eval = AlphaBeta::getTrueEval();
    static float H = boardSize/2;
    float evalHeight;
    if(eval >= 0) evalHeight = H * (1 - std::exp(-0.3 * eval));
    else evalHeight = -H * (1 - std::exp(0.3 * eval));
    return evalHeight; //negative height for -ve eval
}

