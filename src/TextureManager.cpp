#include "../include/TextureManager.hpp"
#include <SDL2/SDL_image.h>

SDL_Texture* TextureManager::LoadTexture(const char* filename, SDL_Renderer* renderer)
{
    SDL_Surface* image;

    image = IMG_Load(filename);
    if (!image) 
    {
        printf("Image %s could not be loaded! IMG_Error: %s", filename, IMG_GetError());
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);

    return texture;
}

