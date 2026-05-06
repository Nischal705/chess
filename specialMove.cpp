#include "specialMove.h"
#include "constants.h"

namespace specialMove{
    std::vector<Move> specialMoves;
    std::vector<Move> getEnpassants(int color, const Move& oppMove){
        //gives the enpassant moves playable by "color"
        int opp_color = (color == white)? black: white;
        specialMoves.clear();
        if(color == white){
            if(oppMove.piece == B_PAWN && (oppMove.from - oppMove.to == 16)){
                if(State::getPiece[oppMove.to + 1] == W_PAWN && oppMove.to + 1 <= 39){
                    int from = oppMove.to + 1;
                    int to = oppMove.to + 8;
                    specialMoves.push_back({from, to, W_PAWN, B_PAWN});
                }
                if(State::getPiece[oppMove.to - 1] == W_PAWN && oppMove.to - 1 >= 32){
                    int from = oppMove.to - 1;
                    int to = oppMove.to + 8;
                    specialMoves.push_back({from, to, W_PAWN, B_PAWN});
                }
            }
        }
        else{ //allowing black to play enpassant too
            if(oppMove.piece == W_PAWN && (oppMove.from - oppMove.to == -16)){
                if(State::getPiece[oppMove.to + 1] == B_PAWN && oppMove.to + 1 <= 31){
                    int from = oppMove.to + 1;
                    int to = oppMove.to - 8;
                    specialMoves.push_back({from, to, B_PAWN, W_PAWN});
                }
                if(State::getPiece[oppMove.to - 1] == B_PAWN && oppMove.to - 1 >= 24){
                    int from = oppMove.to - 1;
                    int to = oppMove.to - 8;
                    specialMoves.push_back({from, to, B_PAWN, W_PAWN});
                }
            }
        }
        return specialMoves;
    }

    void makeEnpassant(const Move& move){
        int color = (move.piece < 6)? white : black;
        int opp_color = (color == white)? black : white; 
        int opp_pawn_sq = (move.piece < 6)? move.to - 8 : move.to + 8;
        int PAWN = (move.piece < 6)? W_PAWN : B_PAWN;
        int OPP_PAWN = (PAWN == white)? B_PAWN : W_PAWN;
        BitBoard::removePiece(move.piece, move.from);
        BitBoard::removePiece(OPP_PAWN, opp_pawn_sq);
        BitBoard::addPiece(move.piece, move.to); 
        BitBoard::BITBOARD[OPP_PAWN] &= ~(1ULL << opp_pawn_sq);
        BitBoard::OCCUPIED[color] &= ~(1ULL << move.from);
        BitBoard::OCCUPIED[opp_color] &= ~(1ULL << opp_pawn_sq);
        BitBoard::OCCUPIED[opp_color] &= ~(1ULL << move.to);
        BitBoard::OCCUPIED[color] &= ~(1ULL << opp_pawn_sq);
        BitBoard::OCCUPIED[either] = BitBoard::OCCUPIED[white] | BitBoard::OCCUPIED[black];
    } 

    void undoEnpassant(const Move& move){
        int opp_pawn_sq = (move.piece < 6)? move.to - 8 : move.to + 8;
        BitBoard::removePiece(move.piece, move.to);
        BitBoard::removePiece(move.capturedPiece, opp_pawn_sq);
        BitBoard::addPiece(move.piece, move.from);
        BitBoard::addPiece(move.capturedPiece, opp_pawn_sq);
    }

    bool isEnpassant(const Move& move){
        if(move.piece == W_PAWN && (move.to - move.from == 7 || move.to - move.from == 9))
            if(State::getPiece[move.to] == EMPTY)
                return true;
        if(move.piece == B_PAWN && (move.from - move.to == 7 || move.from - move.to == 9))
            if(State::getPiece[move.to] == EMPTY)
                return true;
        return false;
    }

    std::vector<Move> getCastles(int color){
        int castle = canCastle(color);
        if(castle == NO) 
            return {};
        int opp_color = (color == white)? black : white;
        int KING = (color == white)? W_KING : B_KING;
        int ROOK = (color == white)? W_ROOK : B_ROOK;
        int K_HOME = (color == white)? 4 : 60;
        int S_ROOK_HOME = (color == white)? 7 : 63;
        int L_ROOK_HOME = (color == white)? 0 : 56;
        std::vector<Move> castleMoves;
        if(castle == SHORT || castle == BOTH){
            castleMoves.push_back({K_HOME, K_HOME + 2, KING, EMPTY});
        }
        if(castle == LONG || castle == BOTH){
            castleMoves.push_back({K_HOME, K_HOME - 2, KING, EMPTY});
        }
        return castleMoves;
    }

    int canCastle(int color){
        int opp_color = (color == white)? black : white;
        int KING = (color == white)? W_KING : B_KING;
        int ROOK = (color == white)? W_ROOK : B_ROOK;
        int K_HOME = (color == white)? 4 : 60;
        int S_ROOK_HOME = (color == white)? 7 : 63;
        int L_ROOK_HOME = (color == white)? 0 : 56;
        //if king or rook has already moved then not possible
        if(!(State::pieceNotMoved & (1ULL << K_HOME)))
            return NO;  // if king moved then no castle possible 
        //find the squares which are unoccupied and also not attacked by opponent
        uint64_t safe_empty_sq = ~BitBoard::OCCUPIED[either] & ~moveGen::allAttackedSquares[opp_color];
        uint64_t w_s_mask = (color == white)? 0b01100000ULL : 0x6000000000000000;
        uint64_t w_l_mask = (color == white)? 0b1110ULL : 0x0E00000000000000;
        bool s = false, l = false;
        if(State::getPiece[K_HOME] == KING && State::getPiece[S_ROOK_HOME] == ROOK){
            if(((safe_empty_sq & w_s_mask) == w_s_mask) && (State::pieceNotMoved & (1ULL << S_ROOK_HOME)))
                    s = true;
        }
        if(State::getPiece[K_HOME] == KING && State::getPiece[L_ROOK_HOME] == ROOK ){
            if((safe_empty_sq & w_l_mask) == w_l_mask && (State::pieceNotMoved & (1ULL << L_ROOK_HOME)))
                    l = true;
        }
        if(s && l) return BOTH;
        if(s) return SHORT;
        if(l) return LONG;
        return NO;
    }

    bool isCastle(const Move& move){
        int can_castle = canCastle(move.piece >= 6);
        if(can_castle == NO)
            return false;
        //can_castle == true means the castle rights are on so we check if the move played is castle or not
        if(move.piece == W_KING || move.piece == B_KING){
            if(abs(move.from - move.to) == 2)
                return true;
        }
        return false;
    }

    void makeCastle(const Move& move){ //assuming this move is sure to be a valid castle move
        int ROOK = (move.piece < 6)? W_ROOK : B_ROOK;
        int KING = move.piece;
        //erase the king
        BitBoard::BITBOARD[move.piece] &= ~(1ULL << move.from);
        BitBoard::OCCUPIED[move.piece >= 6] &= ~(1ULL << move.from);
        if(move.from - move.to < 0){ //short castle
            //erase the rook
            BitBoard::BITBOARD[ROOK] &= ~(1ULL << (move.from + 3));
            BitBoard::OCCUPIED[move.piece >= 6] &= ~(1ULL << (move.from + 3));
            //place the king
            BitBoard::BITBOARD[move.piece] |= (1ULL << move.to);
            BitBoard::OCCUPIED[move.piece >= 6] |= (1ULL << move.to);
            //place the rook
            BitBoard::BITBOARD[ROOK] |= (1ULL << (move.from + 1));
            BitBoard::OCCUPIED[move.piece >= 6] |= (1ULL << (move.from + 1));
            BitBoard::OCCUPIED[either] = BitBoard::OCCUPIED[0] | BitBoard::OCCUPIED[1];
            //update getPiece array
            State::getPiece[move.from] = EMPTY;
            State::getPiece[move.from + 3] = EMPTY;
            State::getPiece[move.to] = KING;
            State::getPiece[move.from + 1] = ROOK;
            return;
        }
        if(move.from - move.to > 0){ //long castle
            //erase the rook
            BitBoard::BITBOARD[ROOK] &= ~(1ULL << (move.from - 4));
            BitBoard::OCCUPIED[move.piece >= 6] &= ~(1ULL << (move.from - 4));
            //place the king
            BitBoard::BITBOARD[move.piece] |= (1ULL << move.to);
            BitBoard::OCCUPIED[move.piece >= 6] |= (1ULL << move.to);
            //place the rook
            BitBoard::BITBOARD[ROOK] |= (1ULL << (move.from - 1));
            BitBoard::OCCUPIED[move.piece >= 6] |= (1ULL << (move.from - 1));
            BitBoard::OCCUPIED[either] = BitBoard::OCCUPIED[0] | BitBoard::OCCUPIED[1];
             //update getPiece array
            State::getPiece[move.from] = EMPTY;
            State::getPiece[move.from - 4] = EMPTY;
            State::getPiece[move.to] = KING;
            State::getPiece[move.from - 1] = ROOK;
            return;
        }
    }

    void undoCastle(const Move& move){
        int ROOK = (move.piece < 6)? W_ROOK : B_ROOK;
        int KING = move.piece;
        int color = (move.piece < 6)? white : black;
        int rook_i, rook_f, king_i, king_f;
        king_i = move.from;
        king_f = move.to;
        if(move.to > move.from){//short castle
            rook_i = move.from + 3;
            rook_f = move.from + 1;  
        }
        else{
            rook_i = move.from - 4;
            rook_f = move.from  - 1; 
        }
        //remove king from king_f
        BitBoard::BITBOARD[KING] &= ~(1ULL << king_f);
        BitBoard::OCCUPIED[color] &= ~(1ULL << king_f);
        State::getPiece[king_f] = EMPTY;
        //remove rook from rook_f
        BitBoard::BITBOARD[ROOK] &= ~(1ULL << rook_f);
        BitBoard::OCCUPIED[color] &= ~(1ULL << rook_f);
        State::getPiece[rook_f] = EMPTY; 
        //place king in king_i
        BitBoard::BITBOARD[KING] |= (1ULL << king_i);
        BitBoard::OCCUPIED[color] |= (1ULL << king_i);
        State::getPiece[king_i] = KING;
        //place rook in rook_i       
        BitBoard::BITBOARD[ROOK] |= (1ULL << rook_i);
        BitBoard::OCCUPIED[color] |= (1ULL << rook_i);
        State::getPiece[rook_i] = ROOK;
        //update occupied either
        BitBoard::OCCUPIED[either] = BitBoard::OCCUPIED[0] | BitBoard::OCCUPIED[1];
    }    
}
