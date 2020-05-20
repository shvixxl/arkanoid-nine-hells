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

        SDL_Rect getRect() { return windowRect; }

    private:
        // To determine direction (MOVE_LEFT, MOVE_RIGHT)
        int move = 0;
        
        // Current speed
        float speed = 0;

        // Value by which speed changes
        const float speedBoost = 0.5;

        SDL_Texture* texture = nullptr;

        SDL_Rect textureRect;
        SDL_Rect windowRect;
};

