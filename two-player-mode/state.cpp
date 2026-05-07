#include "state.h"

namespace State{
     void updateStateVariables(const Move& moveMade){
        State::isKingCheckBool = (moveGen::isKingCheck(moveMade.piece < 6))? true : false;
        int OPP_KING = (moveMade.piece < 6)? B_KING : W_KING;
        State::KingSq = __builtin_ctzll(BitBoard::BITBOARD[OPP_KING]);
        State::pieceNotMoved &= ~(1ULL << moveMade.to);
        State::prev_sq = moveMade.from;
        State::curr_sq = moveMade.to;
        State::oppMove = moveMade;
        State::moveCompleted = true;
        //update material count

    }
    

    void updateMaterialCount(const Move& move){
        //check for enpassant
        int color = move.piece/6;
        int opp_color = (color == white)? black : white; 
        if(move.piece == W_PAWN || move.piece == B_PAWN){
            if(move.capturedPiece == EMPTY && (move.from % 8 != move.to % 8)){
                materialCount[opp_color]--;
                return;
            }
        }
        //for all other captures, capturePiece tells the captured piece
        if(move.capturedPiece != EMPTY){
            materialCount[opp_color] -= value[move.capturedPiece];
        }
    }
}