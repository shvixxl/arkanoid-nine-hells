#ifndef GAME
#define GAME
#include "Game.hpp"
#endif

class TextureManager
{
    public:
        static SDL_Texture* LoadTexture(const char* filename);
        static void RenderTexture(SDL_Texture* texture, SDL_Rect* textureRect, SDL_Rect* windowRect);
};

