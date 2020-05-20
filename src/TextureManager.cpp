#include "../include/TextureManager.hpp"

SDL_Texture* TextureManager::LoadTexture(const char* filename)
{
    SDL_Surface* image = NULL;

    image = IMG_Load(filename);
    if (!image)
    {
        printf("Image %s could not be loaded! IMG_Error: %s", filename, IMG_GetError());
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(Game::renderer, image);
    SDL_FreeSurface(image);

    return texture;
}

void RenderTexture(SDL_Texture* texture, SDL_Rect* textureRect, SDL_Rect* windowRect)
{
    SDL_RenderCopy(Game::renderer, texture, textureRect, windowRect);
}
