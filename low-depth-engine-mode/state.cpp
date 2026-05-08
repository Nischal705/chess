#include "state.h"

namespace State{
    void updateStateVariables(const Move& moveMade){
        State::isKingCheckBool = (moveGen::isKingCheck(moveMade.piece < 6))? true : false;
        int OPP_KING = (moveMade.piece < 6)? B_KING : W_KING;
        State::KingSq = __builtin_ctzll(BitBoard::BITBOARD[OPP_KING]);
        State::pieceNotMoved &= ~(1ULL << moveMade.from);
        State::prev_sq = moveMade.from;
        State::curr_sq = moveMade.to;
        State::oppMove = moveMade;
        State::moveCompleted = true;
        //update material count
    }
    //king sq can be obtained through bitbaords which are sure to be correctly updated
    //king check bool can also be obtained from bitbaords
    //prev sq and curr sq are not necessary during alpha beta search, they there only for drawing 
    //move completed bool is is just totally unnecessary
    //opp move is necessary for enpassants  
    //pieceNotMoved is necessary for castling and also for evaluation 
    //so while recursing, we need to correctly update only opp move and pieceNotMoved;

    // void recordCurrStateVariables(){
    //  //this not gonna work cause we have to call this inside the sub-functions too bro    
    // }

    // void resetStateVariables(const Move& moveMade) 

    

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
    void restoreMaterialCount(const Move& move){
        int color = move.piece/6;
        int opp_color = (color == white)? black : white; 
        if(move.piece == W_PAWN || move.piece == B_PAWN){
            if(move.capturedPiece == EMPTY && (move.from % 8 != move.to % 8)){
                materialCount[opp_color]++;
                return;
            }
        }
        //for all other captures, capturePiece tells the captured piece
        if(move.capturedPiece != EMPTY){
            materialCount[opp_color] += value[move.capturedPiece];
        }
    }
}