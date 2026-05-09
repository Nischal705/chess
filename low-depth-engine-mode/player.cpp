#include "player.h"
#include "state.h"
#include "moveGen.h"
#include "specialMove.h"
#include "input.h"
#include "alphabeta.h"
#include <thread>
#include <chrono>
void Human::playMove(){
    Input::handleInput();
    
}

void Engine::playMove(){
    std::cout << "Engine thinking..." << std::endl;
    AlphaBeta::engineThink(black);

    if(State::isCheckmate || State::isStalemate){
        handleGameEnd();
        State::switchTurn();
        return;
    }


    Move move = AlphaBeta::getBestMove();
    //get eval too 
    Input::makeMove(move);
    State::updateStateVariables(move);
    State::updateMaterialCount(move);
    State::switchTurn();
    moveGen::init_allMoves(white);
    std::cout << "completed thinking" << std::endl;
    std:: cout << "eval = " << AlphaBeta::getTrueEval() << std::endl;
    return;
}

void Engine::handleGameEnd(){
    std::string message = (State::isCheckmate)? "You Won" : "It's a Draw";
    std::cout << message << std::endl;
    //std::this_thread::sleep_for(std::chrono::seconds(10));
    
}