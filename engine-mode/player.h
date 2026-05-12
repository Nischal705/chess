#pragma once

class Player{
    public:
        int color;
        virtual void playMove() = 0;
        static void switchTurn();
        
};

class Human : public Player{
    public:
        Human(int color){ this->color = color;};
        void playMove() override;
};

class Engine : public Player{
    public:
        Engine(int color){ this->color = color;};
        void playMove() override;
        void handleGameEnd();
};