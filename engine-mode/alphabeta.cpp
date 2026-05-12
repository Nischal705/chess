#include "alphabeta.h"
#include "constants.h"
#include "state.h"
#include "input.h"
#include "eval.h"
#include <vector>
#include <iostream>

float AlphaBeta::alphaBeta(float alpha, float beta, int depth, int color){
    //std::cout << "depth=" << depth << " color=" << color << std::endl;
    if(depth == max_depth){
        return Eval::getEval();
    }
    bool legalMoveFound = false;
    //saving the necessary curr state variables locally 
    Move oppMove = State::oppMove;
    uint64_t pieceNotMoved = State::pieceNotMoved;
    
    //bool firstMove = true;

    // Move bestMove;
    //float bestEval = (color == white)? -100 : 100;
    std::vector<Move> possibleMoves = moveGen::GET_ALL_MOVES(color);
    
    
    


    if(color == white){ //we are iterating through the white move as a response to a black move on upper level 
                        //beta is the best evaluation for black that is guaranteed through other branches
                        //and alpha is the evaluation for white that we are gonna make for the current black move in question
                        //if we get a white move whose evaluation i.e. alpha is greater than beta (guaranteed best eval for black through other upper black move choices)
                        //then we totally abort considering that current upper black move i.e. we break from here and return instead of going deeper
        float maxEval = -inf;
        for(auto& move : possibleMoves){
            Input::makeMove(move);
            State::updateMaterialCount(move);
            State::oppMove = move;
            State::pieceNotMoved &= ~(1ULL << move.from);
            if(!moveGen::isKingCheck(color)){
                legalMoveFound = true;
                float eval = alphaBeta(alpha, beta, depth + 1, !color);
                maxEval = std::max(eval, maxEval);
                alpha = std::max(eval, alpha);
                if(alpha >= beta){
                    State::oppMove = oppMove;
                    Input::undoMove(move);
                    State::restoreMaterialCount(move);
                    
                    State::pieceNotMoved = pieceNotMoved;
                    break;
                }
                   
            }
           
            State::oppMove = oppMove;
            Input::undoMove(move);
            State::restoreMaterialCount(move);
            State::pieceNotMoved = pieceNotMoved;
        }
        if(!legalMoveFound){
                if(moveGen::isKingCheck(color))
                    return (color == white)? -inf + depth: inf - depth;
                else    
                    return 0;
            }
        return maxEval;
    }
    else{
        float minEval = inf;
        for(auto& move : possibleMoves){
            Input::makeMove(move);
            //State::updateStateVariables(move);
            State::updateMaterialCount(move);
            State::oppMove = move;
            State::pieceNotMoved &= ~(1ULL << move.from);
            if(!moveGen::isKingCheck(color)){
                legalMoveFound = true;
                float eval = alphaBeta(alpha, beta, depth + 1, !color);
                minEval = std::min(eval, minEval);
                beta =  std::min(eval, beta);
                if(alpha >= beta){
                    State::oppMove = oppMove;
                    Input::undoMove(move);
                    State::restoreMaterialCount(move);
                    State::pieceNotMoved = pieceNotMoved;
                    break;
                }
            }
           
                
                    
            State::oppMove = oppMove;
            // if(specialMove::isEnpassant(move))
            //         std::cout << "black enpassant move rejected" << std::endl;
            Input::undoMove(move);
            State::restoreMaterialCount(move);
            State::pieceNotMoved = pieceNotMoved;
        }

    if(!legalMoveFound){
                if(moveGen::isKingCheck(color))
                    return (color == white)? -inf + depth : inf - depth;
                else    
                    return 0;
            }
    return minEval;
    }

}

void AlphaBeta::engineThink(int color){
    Move bestMove;
    float bestEval = (color == white)? -inf : inf;
    float alpha = -inf;
    float beta = +inf;
    bool legalMoveFound = false;
    Move oppMove = State::oppMove;
    uint64_t pieceNotMoved = State::pieceNotMoved;
    std::vector<Move> possibleMoves = moveGen::GET_ALL_MOVES(color);
    if(color == black){
        for(auto& move : possibleMoves){
            Input::makeMove(move);
            State::updateMaterialCount(move);
            State::oppMove = move; 
            State::pieceNotMoved &= ~(1ULL << move.from);
            if(!moveGen::isKingCheck(color)){
                legalMoveFound = true;
                float eval = alphaBeta(alpha, beta, 1, !color);
                if(eval < bestEval){
                    bestMove = move;
                    bestEval = eval;
                }
                beta = std::min(eval, beta);
            }
            State::oppMove = oppMove;
            Input::undoMove(move);
            State::restoreMaterialCount(move);
            State::pieceNotMoved = pieceNotMoved;
        }
    }
    else{
        for(auto& move : possibleMoves){
            Input::makeMove(move);
            State::updateMaterialCount(move);
            State::oppMove = move; 
            State::pieceNotMoved &= ~(1ULL << move.from);
            if(!moveGen::isKingCheck(color)){
                legalMoveFound = true;
                float eval = alphaBeta(alpha, beta, 1, !color);
                if(eval > bestEval){
                    bestMove = move;
                    bestEval = eval;
                }
                alpha = std::max(eval, alpha);
            }
            State::oppMove = oppMove;
            Input::undoMove(move);
            State::restoreMaterialCount(move);
            State::pieceNotMoved = pieceNotMoved;
        }
    }
    if(!legalMoveFound){
        if(moveGen::isKingCheck(color)){
            State::isCheckmate = true;
            trueEval = (color == white)? -inf : inf;
        }
        else{    
            State::isStalemate = true;
            trueEval = 0;
        }
        return;
    }
    trueEval = bestEval;
    finalMove = bestMove;
}

  /*  for(auto& move : possibleMoves){
        
        // std::cout << "captured: " << move.capturedPiece << "\n";
        // std::cout << "from: " << move.from << std::endl << "to: " << move.to << std::endl;
        Input::makeMove(move);
        if(!moveGen::isKingCheck(color)){
            //std::cout << "piece: " << move.piece << "\n";
            float nextEval = minimax(depth + 1, !color);
            if(firstMove){
                bestEval = nextEval;
                bestMove = move;
                firstMove = false;
                Input::undoMove(move);
                continue;
            }
            if(color == white){
                if(nextEval > bestEval){
                bestEval = nextEval;
                bestMove = move;
                }
            }
            else{
                if(nextEval < bestEval){
                    bestEval = nextEval;
                    bestMove = move;
                }
            }
        
        }
         Input::undoMove(move);
    }

    if(depth == 0){
        finalMove = bestMove;
        trueEval = bestEval;
    }
    //std::cout << "depth = " << depth << std::endl;
    return bestEval;
}
*/

Move AlphaBeta::getBestMove(){
    return finalMove;
}

float AlphaBeta::getTrueEval(){
    return trueEval;
}