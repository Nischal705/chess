#include "bitboard.h"
#include "constants.h"
namespace BitBoard{
    uint64_t BITBOARD[12] = {0xFF00ULL, 0b01000010ULL, 0b00100100ULL, 0b10000001ULL,
                            0b00010000ULL, 0b00001000ULL, 0xFF00ULL << 40, 0b01000010ULL << 56,
                            0b00100100ULL << 56, 0b10000001ULL << 56, 0b00010000ULL << 56,
                            0b00001000ULL << 56};
    uint64_t OCCUPIED[3] = {0xFFFFULL, 0xFFFF000000000000ULL, 0xFFFF00000000FFFFULL};
    
    void addPiece(int piece, int sq){
       /* if(State::getPiece[sq] != EMPTY){
            removePiece(State::getPiece[sq], sq);
        }*/
        //if(piece == EMPTY)
           // return;
        BITBOARD[piece] |= (1ULL << sq);
        int color = (piece < B_PAWN)? white : black;
        OCCUPIED[color] |= (1ULL << sq);
        OCCUPIED[either] = OCCUPIED[black] | OCCUPIED[white];
        State::getPiece[sq] = piece;
    }

    void removePiece(int piece, int sq){
       // if(piece == EMPTY)
         //   return;
        uint64_t mask = ~(1ULL << sq);
        BITBOARD[piece] &= mask;
        int color = (piece < B_PAWN)? white : black;
        OCCUPIED[color] &= mask;
        //OCCUPIED[either] = OCCUPIED[black] | OCCUPIED[white];
        OCCUPIED[either] &= mask;
        State::getPiece[sq] = EMPTY;
    }
}
