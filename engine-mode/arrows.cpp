#include "arrows.h"
#include "raylib.h"
#include "input.h"
#include <algorithm>
std::vector<Arrow> ArrowStorage::arrows;
std::unordered_set<int> ArrowStorage::seenArrows;

Arrow::Arrow(int from, int to): from(from), to(to){};


void ArrowStorage::handleArrows(){
    Vector2 mousePos = GetMousePosition();
    if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)){
        currArrow.from = Input::findSquare(mousePos);
        creationStage = true;
    }
    if(creationStage){
        currArrow.to = Input::findSquare(mousePos);
        if(IsMouseButtonReleased(MOUSE_BUTTON_RIGHT)){
            int key = getKey(currArrow.from, currArrow.to);
            if(!seenArrows.count(key)){
                arrows.push_back(currArrow);
                seenArrows.insert(key);
            }
            else{
                arrows.erase(std::find(arrows.begin(), arrows.end(), currArrow));
                seenArrows.erase(key);
            }
            currArrow.from = -1;
            currArrow.to = -1;
            creationStage = false;
        }
    }
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        currArrow.from = -1;
        currArrow.to = -1;
        arrows.clear();
        seenArrows.clear();
    }
}

int ArrowStorage::getKey(int from, int to){
    return from*100 + to;
}
