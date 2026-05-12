#pragma once
#include "constants.h"
#include "bitboard.h"
#include "state.h"
#include "moveGen.h"
#include <cstdint>
class Eval{
    private:
        inline static bool evalOn = true;
    public:
        static float considerMaterial();
        static float considerOccupancy();
        static float considerKingSafety();
        static float considerDevelopment();
        static float considerCenterControl();
        static float getEval();
        static float getEvalHeight();
        
};
