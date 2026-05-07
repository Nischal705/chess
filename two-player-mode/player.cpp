#include "player.h"
#include "state.h"
#include "moveGen.h"
#include "specialMove.h"
#include "input.h"
void Human::playMove(){
    Input::handleInput();
}

void Engine::playMove(){
    Input::handleInput();
}