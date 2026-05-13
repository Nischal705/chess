#pragma once
#include <vector> 
#include <unordered_set>
class Arrow{
    public:
        int from;
        int to;
        Arrow(int from, int to);
        bool operator==(const Arrow& other) const{
            return from == other.from && to == other.to;
        }
};
class ArrowStorage{
    private:
        inline static bool creationStage = false;
        static std::unordered_set<int> seenArrows;
        static int getKey(int from, int to);
    public:
        inline static Arrow currArrow{-1, -1};
        static std::vector<Arrow> arrows;
        static void handleArrows();
        
};

