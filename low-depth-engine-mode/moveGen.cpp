#include "moveGen.h"
#include <iostream>
#include <vector>
#include <cstring>
#include "constants.h"
#include "bitboard.h"
#include "specialMove.h"
#include <iomanip>

namespace moveGen{
    uint64_t knightAttacks[64];
    uint64_t kingAttacks[64];
    uint64_t allAttackedSquares[2] = {0x0ULL, 0x0ULL};
    uint64_t allDefendedSquares[2] = {0x0ULL, 0x0ULL};
    std::vector<Move>allMoves(40);
    std::vector<Move> getAllMoves(int color){
        //memset(allAttackedSquares, 0x0ULL, sizeof(allAttackedSquares));
        allAttackedSquares[color] = 0ULL;
        //allAttackedSquares[1] = 0ULL;
        memset(allDefendedSquares, 0x0ULL, sizeof(allDefendedSquares));
        std::vector<Move> allMoves = {};























        
        for(int sq = 0; sq <= 63; sq++){ //the sq in which piece is standing
            int piece = State::getPiece[sq];
            if(piece == EMPTY)
                continue;
            if(piece/6 != color)
                continue;
            //int color = (piece < B_PAWN)? white : black;
            int opp_color = (color == white)? black : white;
            int OPP_KING = (color == white)? B_KING : W_KING;

            //if enemy's color then don't calculate
            //if(State::currPlayer->color != color)
              //  continue;

            if(piece == W_KNIGHT || piece == B_KNIGHT){
                uint64_t legalTo = knightAttacks[sq] & ~BitBoard::OCCUPIED[color];

                allAttackedSquares[color] |= legalTo;

                while(legalTo){
                    int s = __builtin_ctzll(legalTo); //trailing zeroes = index of last one
                    allMoves.push_back({sq, s, piece, State::getPiece[s]});
                    legalTo &= (legalTo - 1); //flips the last one bit to zero
                }
            }

            if(piece == W_KING || piece == B_KING){
                uint64_t legalTo = kingAttacks[sq] & ~BitBoard::OCCUPIED[color];
                allAttackedSquares[color] |= legalTo;
                while(legalTo){
                    int s = __builtin_ctzll(legalTo);
                    allMoves.push_back({sq, s, piece, State::getPiece[s]});
                    legalTo &= (legalTo - 1);
                }
            }

            // if(piece == W_PAWN || piece == B_PAWN){ //why am i including this in the loop man
            //     //move this and the king and the knight out of the loop later
            //     uint64_t pawn_board = BitBoard::BITBOARD[piece];
            //     int initial_rank = (color == white)? 1 : 6;
            //     while(pawn_board){
            //         int s = __builtin_ctzll(pawn_board);
            //         int up_s = (color == white)? s+8 : s-8;;
            //         int up_up_s = (color == white)? s+16 : s-16;
            //         int r_capture = (color == white)? s+9 : s-7 ;
            //         int l_capture = (color == white)? s+7 : s-9;
            //         if(s%8 < 7)
            //             allAttackedSquares[color] |= (1ULL << r_capture);
            //         if(s%8 > 0)
            //             allAttackedSquares[color] |= (1ULL << l_capture);
            //         if((1ULL << up_s) & ~BitBoard::OCCUPIED[either]){ //single push
            //             allMoves.push_back({s, up_s, piece, EMPTY});
            //             if(((1ULL << up_up_s) & ~BitBoard::OCCUPIED[either]) && s/8 == initial_rank) //double push
            //                 allMoves.push_back({s, up_up_s, piece, EMPTY});
            //         }
            //         if(s%8 < 7){
            //         if((1ULL << r_capture) & BitBoard::OCCUPIED[opp_color])
            //             allMoves.push_back({s, r_capture, piece, State::getPiece[r_capture]}); //right capture
            //         }
            //         if(s%8 > 0){
            //             if((1ULL << l_capture) & BitBoard::OCCUPIED[opp_color])
            //             allMoves.push_back({s, l_capture, piece, State::getPiece[l_capture]}); //left capture
            //         }
            //         pawn_board &= (pawn_board - 1);
            //     }
            // }

            if(piece == W_BISHOP || piece == B_BISHOP || piece == W_ROOK || piece == B_ROOK || piece == W_QUEEN || piece == B_QUEEN){
                static std::vector<std::vector<int>> direction(12);
                direction[W_BISHOP] = {7, 9, -7, -9}; direction[B_BISHOP] = {7, 9, -7, -9};
                direction[W_ROOK] = {8, 1, -8, -1}; direction[B_ROOK] = {8, 1, -8, -1};
                direction[W_QUEEN] = {7,8,9,1,-7,-8,-9,-1}; direction[B_QUEEN] = {7,8,9,1,-7,-8,-9,-1};
                for(auto& dir : direction[piece]){
                    int prev_s = sq, curr_s = sq + dir;
                    while(true){
                        if(curr_s < 0 || curr_s > 63)
                            break;
                        if(abs(curr_s/8 - prev_s/8) > 1 || abs(curr_s%8 - prev_s%8) > 1) //if curr_s magically appears on the other side
                            break;
                        if((1ULL << curr_s) & ~BitBoard::OCCUPIED[either]){ //empty square
                            allMoves.push_back({sq, curr_s, piece, EMPTY});
                            allAttackedSquares[color] |= (1ULL << curr_s);
                        }
                        if((1ULL << curr_s) & BitBoard::OCCUPIED[opp_color]){ //enemy piece
                            allMoves.push_back({sq, curr_s, piece, State::getPiece[curr_s]});
                            allAttackedSquares[color] |= (1ULL << curr_s);
                            break;
                        }   
                        if((1ULL << curr_s) & BitBoard::OCCUPIED[color]){ //own piece
                            allDefendedSquares[color] |= (1ULL << curr_s);
                            break;
                        }
                        prev_s = curr_s;
                        curr_s += dir;
                    }
                }
            }
        }






        // if(piece == W_PAWN || piece == B_PAWN){ //why am i including this in the loop man
                //move this and the king and the knight out of the loop later
                int piece = (color == white)? W_PAWN : B_PAWN;
                int opp_color = (color == white)? black : white;
                int OPP_KING = (color == white)? B_KING : W_KING;
                uint64_t pawn_board = BitBoard::BITBOARD[piece];
                int initial_rank = (color == white)? 1 : 6;
                while(pawn_board){
                    int s = __builtin_ctzll(pawn_board);
                    int up_s = (color == white)? s+8 : s-8;;
                    int up_up_s = (color == white)? s+16 : s-16;
                    int r_capture = (color == white)? s+9 : s-7 ;
                    int l_capture = (color == white)? s+7 : s-9;
                    if(s%8 < 7)
                        allAttackedSquares[color] |= (1ULL << r_capture);
                    if(s%8 > 0)
                        allAttackedSquares[color] |= (1ULL << l_capture);
                    if((1ULL << up_s) & ~BitBoard::OCCUPIED[either]){ //single push
                        allMoves.push_back({s, up_s, piece, EMPTY});
                        if(((1ULL << up_up_s) & ~BitBoard::OCCUPIED[either]) && s/8 == initial_rank) //double push
                            allMoves.push_back({s, up_up_s, piece, EMPTY});
                    }
                    if(s%8 < 7){
                    if((1ULL << r_capture) & BitBoard::OCCUPIED[opp_color])
                        allMoves.push_back({s, r_capture, piece, State::getPiece[r_capture]}); //right capture
                    }
                    if(s%8 > 0){
                        if((1ULL << l_capture) & BitBoard::OCCUPIED[opp_color])
                        allMoves.push_back({s, l_capture, piece, State::getPiece[l_capture]}); //left capture
                    }
                    pawn_board &= (pawn_board - 1);
                }
            //}

















        return allMoves;
    }


    bool isKingCheck(int color){ //color of king to be checked if in check
        int opp_color = ((color == white)? black : white);
        int KING = ((color == white)? W_KING : B_KING);
        int OPP_KING = (color == white)? B_KING : W_KING;
        int OPP_ROOK = OPP_KING - 1;
        int OPP_BISHOP = OPP_ROOK - 1;
        int OPP_KNIGHT = OPP_BISHOP - 1;
        int OPP_PAWN = OPP_KNIGHT - 1;
        int OPP_QUEEN = OPP_KING + 1;
        uint64_t OPP_R_Q = BitBoard::BITBOARD[OPP_ROOK] | BitBoard::BITBOARD[OPP_QUEEN];
        uint64_t OPP_B_Q = BitBoard::BITBOARD[OPP_BISHOP] | BitBoard::BITBOARD[OPP_QUEEN];
        //this requires calling init_allMoves and calculating everything for the opponent so let's change it
        // if(BitBoard::BITBOARD[KING] & allAttackedSquares[opp_color])
        //     return true;
        // return false;

        //plan: 
        //pulling a reverse check
        //implementation: we first of all remove the piece and place in its to sq
        //without calling init_allMoves 
        //then we call this function
        //so make changes to isLegalMove() after this

        int king_s = __builtin_ctzll(BitBoard::BITBOARD[KING]); //index of our king
         uint64_t opp_king_s = __builtin_ctzll(BitBoard::BITBOARD[OPP_KING]); //index of opp king
        int king_mask = (1ULL << king_s);
        static std::vector<int> h_direction{8, 1, -8, -1};
        static std::vector<int> d_direction{7, 9, -7, -9};
        
        //check knight or king check and pawn too 
        /*uint64_t opp_knight_bitboard = BitBoard::BITBOARD[OPP_KNIGHT]; // can be multiple
        while(opp_knight_bitboard){
            int knight_i = __builtin_ctzll(opp_knight_bitboard);
            if(king_mask & knightAttacks[knight_i])
                return true;
            opp_knight_bitboard &= (opp_knight_bitboard - 1);
        }*/
        //check knight checks by asking if a knight sitting in the king's sq can see opp knight
        if(knightAttacks[king_s] & BitBoard::BITBOARD[OPP_KNIGHT])
            return true;
        //check if attacked by opp king
        if(kingAttacks[opp_king_s] & king_mask)
            return true;
        //check for pawn
        int p1 = (color == white)? king_s + 9 : king_s -7; //right side corner
        int p2 = (color == white)? king_s + 7 : king_s - 9; //left side corner
        if(king_s % 8 < 7)
            if((State::getPiece[p1] == OPP_PAWN))
                return true;
        if(king_s % 8 > 0)
            if((State::getPiece[p2] == OPP_PAWN))
                return true;
        //now go for the other pieces
        int curr_s, prev_s;
        for(int& dir : h_direction){
            prev_s = king_s;
            while(true){
                curr_s = prev_s + dir;
                if(curr_s > 63 || curr_s < 0)
                    break;
                if(prev_s % 8 == 0 && (dir == -1))
                    break;
                if(prev_s % 8 == 7 && (dir == 1))
                    break;
                if((1ULL << curr_s) & BitBoard::OCCUPIED[color])
                    break;    
                if((1ULL << curr_s) & OPP_R_Q)
                    return true;
                if((1ULL << curr_s) & BitBoard::OCCUPIED[opp_color])
                    break;
                
                prev_s = curr_s;
            }
        } 
        for(int& dir : d_direction){
            prev_s = king_s;
            while(true){
                curr_s = prev_s + dir;
                if(curr_s > 63 || curr_s < 0)
                    break;
                if(prev_s % 8 == 0 && (dir == 7 || dir == -9))
                    break;
                if(prev_s % 8 == 7 && (dir == -7 || dir == 9))
                    break;
                if((1ULL << curr_s) & BitBoard::OCCUPIED[color])
                    break;
                if((1ULL << curr_s) & OPP_B_Q){
                    return true;
                }
                if((1ULL << curr_s) & BitBoard::OCCUPIED[opp_color])
                    break;
                prev_s = curr_s;
            }        
        }
        return false;
    }

    
    void init_KnightAttacks(){
        for(int k_sq = 0; k_sq <= 63; k_sq++){
            for(int sq = 0; sq <= 63; sq++){
                int k_r = k_sq/8, k_c = k_sq%8;
                int r = sq/8, c = sq%8;
                if((abs(k_r - r) == 2 && abs(k_c - c) == 1)||(abs(k_r - r) == 1 && abs(k_c - c) == 2)){
                    knightAttacks[k_sq] |= (1ULL << sq);
                }
            }
        }
    }

    void init_KingAttacks(){
        for(int k_sq = 0; k_sq <= 63; k_sq++){
            for(int s = 0; s <= 63; s++){
                int k_r = k_sq/8, k_c = k_sq%8;
                int r = s/8, c = s%8;
                int x_comp = abs(k_c - c), y_comp = abs(k_r - r);
                if(std::max(x_comp, y_comp) == 1 && std::min(x_comp, y_comp) >= 0)
                    kingAttacks[k_sq] |= (1ULL << s);
            }
        }
    }
    void init(){
        init_KnightAttacks();
        init_KingAttacks();
        init_allMoves(white);
    }
    void init_allMoves(int color){ //color whose moves are to be listed
        moveGen::allMoves.clear();
        moveGen::allMoves = moveGen::getAllMoves(color); //add parameter color here later
        std::vector<Move> specialMoves = specialMove::getEnpassants(color, State::oppMove);
        allMoves.insert(allMoves.end(), specialMoves.begin(), specialMoves.end());
        std::vector<Move> castleMoves = specialMove::getCastles(color);
        allMoves.insert(allMoves.end(), castleMoves.begin(), castleMoves.end());
    }

    std::vector<Move> GET_ALL_MOVES(int color){
        std::vector<Move>ALL_MOVES = getAllMoves(color);
        // std::vector<Move> specialMoves = specialMove::getEnpassants(color, State::oppMove);
        // ALL_MOVES.insert(ALL_MOVES.end(), specialMoves.begin(), specialMoves.end());
        // std::vector<Move> castleMoves = specialMove::getCastles(color);
        // ALL_MOVES.insert(ALL_MOVES.end(), castleMoves.begin(), castleMoves.end());
        return ALL_MOVES;   
    }
    void print_board(){
        for(int i = 7; i >= 0; i--){
            for(int s = 8*i; s <  8*(i+1); s++){
                std::cout << State::getPiece[s] << "\t";
            }
         std::cout << std::endl;
        }
    }
}

