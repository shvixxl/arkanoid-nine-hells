#ifndef GAME
#define GAME
#include "Game.hpp"
#endif

class Timer
{
    public:
        Timer(Uint32 delay);
        ~Timer();

        void Set(Uint32 delay);
        void Restart();

        bool Ready();

    private:
        Uint32 start;
        Uint32 delay;
};

