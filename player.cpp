#include "player.h"
#include "state.h"
#include "moveGen.h"
#include "specialMove.h"
#include "input.h"
#include "minimax.h"
void Human::playMove(){
    Input::handleInput();
}

void Engine::playMove(){
     std::cout << "Engine thinking..." << std::endl;
    Minimax::minimax(0, 1);
    Move move = Minimax::getBestMove();

    Input::makeMove(move);
    State::updateStateVariables(move);
    State::updateMaterialCount(move);
    State::switchTurn();
    moveGen::init_allMoves(white);
    std::cout << "completed thinking" << std::endl;
    std:: cout << "eval = " << Minimax::getTrueEval() << std::endl;
    return;
}