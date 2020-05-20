#ifndef GAME
#define GAME
#include "Game.hpp"
#endif

class Ball
{
    public:
        Ball(const char* filename);
        ~Ball();
        
        void Update();
        void Render();

        SDL_Rect getRect() { return windowRect; }

    private:
        SDL_Texture* texture = nullptr;

        SDL_Rect textureRect;
        SDL_Rect windowRect;

};

