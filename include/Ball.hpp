#ifndef GAME
#define GAME
#include "Game.hpp"
#endif

#define MAX_SPEED 20
#define SPEED_REDUCE 2

class Ball
{
    public:
        Ball(const char* filename, int x, int y, float paddleSpeed);
        ~Ball();
        
        void Update(float paddleSpeed, SDL_Rect paddleRect);
        void Render();
        
        void Rebound(SDL_Rect* objectRect, float objectSpeedX, float objectSpeedY);

        SDL_Rect getRect() { return windowRect; }

    private:
        int r;

        float x;
        float y;

        float speedY;
        float speedX;

        int frames;
        int frameDelay;

        SDL_Texture* texture = nullptr;

        SDL_Rect textureRect;
        SDL_Rect windowRect;

};

