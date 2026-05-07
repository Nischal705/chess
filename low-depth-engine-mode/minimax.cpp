#include "minimax.h"
#include "constants.h"
#include "state.h"
#include "input.h"
#include "eval.h"
#include <vector>

float Minimax::minimax(int depth, int color){
    //std::cout << "depth=" << depth << " color=" << color << std::endl;
    if(depth == max_depth){
        return Eval::getEval();
    }
    
    bool firstMove = true;

    Move bestMove;
    float bestEval = (color == white)? -100 : 100;
    std::vector<Move> possibleMoves = moveGen::GET_ALL_MOVES(color);

    for(auto& move : possibleMoves){
        
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

Move Minimax::getBestMove(){
    return finalMove;
}

float Minimax::getTrueEval(){
    return trueEval;
}