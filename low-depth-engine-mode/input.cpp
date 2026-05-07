#include "input.h"
#include "constants.h"
#include "state.h"
#include "bitboard.h"
#include "images.h"
#include "specialMove.h"
#include "eval.h"
#include <iostream>
void Input::handleInput(){
    Vector2 mousePos = GetMousePosition();
    static int fromSq = -1;
    static int currPiece = EMPTY;
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){


        fromSq = findSquare(mousePos);
        if(fromSq == -1)
            return;
        currPiece = State::getPiece[fromSq]; //gives piece index but we got enums
        if(currPiece != EMPTY)
            BitBoard::removePiece(currPiece, fromSq);
    }

    if(currPiece == EMPTY){
        fromSq = -1;
        return;
    }

    if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
            dragPiece(currPiece, mousePos);
    }
    if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT)){
        int toSq = findSquare(mousePos);
        if(toSq == -1){
            BitBoard::addPiece(currPiece, fromSq);
            currPiece = EMPTY;
            return;
        }

        
        Move moveTried = {fromSq, toSq, currPiece, State::getPiece[toSq]};
        
        BitBoard::addPiece(moveTried.piece, fromSq); //put the piece back on move.from to let makeMove do the whole thing
        if(isLegal(moveTried, moveGen::allMoves)){
            Input::makeMove(moveTried); 

            
            State::updateStateVariables(moveTried);
            State::updateMaterialCount(moveTried);

            //std::cout << "white = " << State::materialCount[white] << " black = " << State::materialCount[black] << std::endl;
            
            State::switchTurn();

            //std::cout<< std::endl << "this is now " << State::currPlayer->color << "'s turn" << std::endl;
            //now find the moves for oppononet
            //moveGen::init_allMoves(State::currPlayer->color);
            //move completed
            
            
        }
        else { 
            if (moveTried.from != moveTried.to) {

                BitBoard::removePiece(moveTried.piece, moveTried.to);

                if (moveTried.capturedPiece != 12){
                    BitBoard::addPiece(moveTried.capturedPiece, moveTried.to);
                }
                else{
                    BitBoard::OCCUPIED[white] &= ~(1ULL << moveTried.to);
                    BitBoard::OCCUPIED[black] &= ~(1ULL << moveTried.to);
                    BitBoard::OCCUPIED[either] &= ~(1ULL << moveTried.to);
                }
                BitBoard::addPiece(moveTried.piece, moveTried.from);
                //moveGen::init_allMoves(State::currPlayer->color);
            }
            
        }
        
            
        //std::cout<< "curr player color = " << State::currPlayer->color << std::endl;
        moveGen::print_board();

        currPiece = EMPTY;
        fromSq = -1;
    }
    
}

int Input::findSquare(Vector2 mousePos){
    if(mousePos.x > boardX && mousePos.x < boardX + boardSize){
        if(mousePos.y > boardY && mousePos.y < boardY + boardSize){
            int c = (mousePos.x - boardX)/cellSize;
            int r = 7 - (int)(mousePos.y - boardY)/cellSize;
            return r*8 + c;
        }
    }
    return -1;
}

void Input::dragPiece(int piece, Vector2 mousePos){
    Texture2D pieceTexture = Images::pieceTextures[piece];
    float textureX = mousePos.x - pieceTexture.width / 2;
    float textureY = mousePos.y - pieceTexture.height / 2; 
    DrawTexture(pieceTexture, textureX, textureY, WHITE);
}

bool Input::isLegal(const Move& move, std::vector<Move>& allMoves){
    for(int i = 0; i < allMoves.size(); i++){
        if(move == allMoves[i]){
            int color = move.piece/6;
            /*Input::makeMove(move);
            
            if(moveGen::isKingCheck(color)){
                undoMove(move);
                return false;
            }
            else{
                undoMove(move);   
                return true;
            }
            */
            makeMove(move);
            // BitBoard::removePiece(move.piece, move.from);
            // BitBoard::removePiece(move.capturedPiece, move.to);
            // BitBoard::addPiece(move.piece, move.to);
            if(moveGen::isKingCheck(color)){
                // BitBoard::removePiece(move.piece, move.to);
                // BitBoard::addPiece(move.piece, move.from);
                // BitBoard::addPiece(move.capturedPiece, move.to);
                undoMove(move);
                return false;
            }
            else{
                // BitBoard::removePiece(move.piece, move.to);
                // BitBoard::addPiece(move.piece, move.from);
                // BitBoard::addPiece(move.capturedPiece, move.to);
                undoMove(move);
                return true;
            }
            
        }
    }
    return false;
}

void Input::makeMove(const Move& move){
    //check for enpassant move
    if(specialMove::isEnpassant(move)){
        specialMove::makeEnpassant(move);
        // moveGen::init_allMoves(move.piece >= 6);
        // moveGen::init_allMoves(move.piece < 6); //pass to opponent i.e. fill allMoves by opponents move
        return;
    }
    if(specialMove::isCastle(move)){
        specialMove::makeCastle(move);
        // moveGen::init_allMoves(move.piece >= 6);
        // moveGen::init_allMoves(move.piece < 6);
        return;
    }
    BitBoard::removePiece(move.piece, move.from);
    BitBoard::removePiece(move.capturedPiece, move.to);
    BitBoard::addPiece(move.piece, move.to);
    // moveGen::init_allMoves(move.piece >= 6);
    // moveGen::init_allMoves(move.piece < 6);
}
void Input::undoMove(const Move& move){
    if(specialMove::isEnpassant(move)){
        specialMove::undoEnpassant(move);
        // moveGen::init_allMoves(move.piece >= 6);
        // moveGen::init_allMoves(move.piece < 6);
        return;
    }
    if(specialMove::isCastle(move)){
        specialMove::undoCastle(move);
        // moveGen::init_allMoves(move.piece >= 6);
        // moveGen::init_allMoves(move.piece < 6);
        return;
    }
    BitBoard::removePiece(move.piece, move.to);
    BitBoard::addPiece(move.capturedPiece, move.to);
    BitBoard::addPiece(move.piece, move.from);
    // moveGen::init_allMoves(move.piece >= 6);
    // moveGen::init_allMoves(move.piece < 6);
}