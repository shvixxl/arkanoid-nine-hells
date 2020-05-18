#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

class TextureManager
{
    public:
        static SDL_Texture* LoadTexture(const char* filename, SDL_Renderer* renderer);
};

