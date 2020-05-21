#ifndef GAME
#define GAME
#include "Game.hpp"
#endif

#define POWERS 6

class Power
{
    public:
        Power(SDL_Rect* objectRect);
        ~Power();

        void Update();
        void Render(SDL_Texture* texture);

        int GeneratePower();

        SDL_Rect getRect() { return windowRect; }

    private:
        int frames;
        int frameDelay;

        SDL_Rect textureRect;
        SDL_Rect windowRect;
};

