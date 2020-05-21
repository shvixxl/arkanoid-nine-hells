#ifndef GAME
#define GAME
#include "Game.hpp"
#endif

#define MOVE_LEFT 1
#define MOVE_RIGHT 2

class Player
{
    public:
        Player(const char* filename);
        ~Player();

        void Update();
        void Render();

        void HandleEvents(SDL_Event* event);

        void Slow();

        void Power();

        SDL_Rect getRect() { return windowRect; }
        float getSpeed() { return speed; }

    private:
        // To determine direction (MOVE_LEFT, MOVE_RIGHT)
        int move;

        float x;

        // Current speed
        float speed;

        // Value by which speed changes
        float speedBoost;
        float speedSlow;

        // Is ball was throwm
        SDL_Texture* texture = nullptr;

        SDL_Rect textureRect;
        SDL_Rect windowRect;
};

