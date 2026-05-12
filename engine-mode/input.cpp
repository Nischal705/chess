#include "input.h"
#include "constants.h"
#include "state.h"
#include "bitboard.h"
#include "images.h"
#include "specialMove.h"
#include "eval.h"
#include "draw.h"
#include <iostream>
void Input::handleInput(){
    Vector2 mousePos = GetMousePosition();
    static int fromSq = -1;
    static int currPiece = EMPTY;

   if(State::promotionStage){
            
            int newPiece = getChosenPiece(State::pendingMove);
            if(newPiece != -1){
                Move move = State::pendingMove;
                move.piece = newPiece; 
                move.isPromotion = true;
                if(move.piece/6 == white) BitBoard::addPiece(W_PAWN, move.from);
                else BitBoard::addPiece(B_PAWN, move.from);
                Input::makeMove(move);
                State::updateStateVariables(move);
                State::updateMaterialCount(move);
                State::switchTurn();

                State::promotionStage = false;
                currPiece = EMPTY;
                fromSq = -1;
            }
        return;
    }



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
        if(!State::promotionStage){
            int toSq = findSquare(mousePos);
            if(toSq == -1){
                BitBoard::addPiece(currPiece, fromSq);
                currPiece = EMPTY;
                return;
            }

            
            Move moveTried = {fromSq, toSq, currPiece, State::getPiece[toSq]};
            

            /////////first check if the pawn move is legal, if yes then only show the window
            if(specialMove::isPromotion(moveTried)){
                if(isLegal(moveTried, moveGen::allMoves)){
                    State::promotionStage = true;
                    State::pendingMove = moveTried;
                    return;
                }
            }

            //make the normal moves 
            BitBoard::addPiece(moveTried.piece, fromSq); //put the piece back on move.from to let makeMove do the whole thing
        
            if(isLegal(moveTried, moveGen::allMoves)){
                Input::makeMove(moveTried); 
                State::updateStateVariables(moveTried);
                State::updateMaterialCount(moveTried);
                State::switchTurn();            
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
                }
                
            }
        
        moveGen::print_board();

        currPiece = EMPTY;
        fromSq = -1;



    }
        
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
           
            makeMove(move);
        
            if(moveGen::isKingCheck(color)){
                undoMove(move);
                return false;
            }
            else{
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
        return;
    }
    if(specialMove::isCastle(move)){
        specialMove::makeCastle(move);
        return;
    }
    if(move.isPromotion){
        specialMove::makePromotion(move);
        return;
    }
    BitBoard::removePiece(move.piece, move.from);
    BitBoard::removePiece(move.capturedPiece, move.to);
    BitBoard::addPiece(move.piece, move.to);
}
void Input::undoMove(const Move& move){
    if(specialMove::isEnpassant(move)){ 
        specialMove::undoEnpassant(move);
        return;
    }
    if(specialMove::isCastle(move)){
        specialMove::undoCastle(move);
        return;
    }
    if(move.isPromotion){
        specialMove::undoPromotion(move);
        return;
    }
    BitBoard::removePiece(move.piece, move.to);
    BitBoard::addPiece(move.capturedPiece, move.to);
    BitBoard::addPiece(move.piece, move.from);
}

int Input::getChosenPiece(const Move& move){
    int color = move.piece/6;
    int c = move.to%8;
    Vector2 mousePos = GetMousePosition();
    Rectangle knightRect, bishopRect, rookRect, queenRect;
    if(color == white){
        queenRect = {boardX + c*cellSize, boardY + 0*cellSize, cellSize, cellSize};
        rookRect = {boardX + c*cellSize, boardY + 1*cellSize, cellSize, cellSize};
        bishopRect = {boardX + c*cellSize, boardY + 2*cellSize, cellSize, cellSize};
        knightRect = {boardX + c*cellSize, boardY + 3*cellSize, cellSize, cellSize};
    }
    else{
        queenRect = {boardX + c*cellSize, boardY + 7*cellSize, cellSize, cellSize};
        rookRect = {boardX + c*cellSize, boardY + 6*cellSize, cellSize, cellSize};
        bishopRect = {boardX + c*cellSize, boardY + 5*cellSize, cellSize, cellSize};
        knightRect = {boardX + c*cellSize, boardY + 4*cellSize, cellSize, cellSize};
    }

    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        if(CheckCollisionPointRec(mousePos, queenRect))
            return (color == white)? W_QUEEN : B_QUEEN;
        if(CheckCollisionPointRec(mousePos, rookRect))
            return (color == white)? W_ROOK : B_ROOK;
        if(CheckCollisionPointRec(mousePos, bishopRect))
            return (color == white)? W_BISHOP : B_BISHOP;
        if(CheckCollisionPointRec(mousePos, knightRect))
            return (color == white)? W_KNIGHT : B_KNIGHT;
    }
    return -1;

}
