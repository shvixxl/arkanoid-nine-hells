#ifndef GAME
#define GAME
#include "Game.hpp"
#endif

class GameObject
{
    public:
        GameObject(const char* texture, int x, int y);
        ~GameObject();
        
        void Update();
        void Render();

    private:
        int x;
        int y;

        SDL_Texture* texture = NULL;
        SDL_Rect srcRect, destRect;

};

