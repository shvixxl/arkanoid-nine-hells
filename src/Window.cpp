#include "../include/Window.hpp"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>

void Window::Init(const char* title, int x, int y, int w, int h, int windowFlags)
{
    window = SDL_CreateWindow(title, x, y, w, h, windowFlags);
    if (window == NULL)
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL)
    {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
    }
    else
    {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    }

}

void Window::Clean()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

SDL_Texture* Window::LoadTexture(const char* filename)
{
    SDL_Surface* image = NULL;

    image = IMG_Load(filename);
    if (!image)
    {
        printf("Image %s could not be loaded! IMG_Error: %s", filename, IMG_GetError());
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);

    return texture;
}

void Window::Render(SDL_Texture* texture, SDL_Rect* textureRect, SDL_Rect* windowRect)
{
    SDL_RenderCopy(renderer, texture, textureRect, windowRect);
}

void Window::RednerClear()
{
    SDL_RenderClear(renderer);
}

void Window::RenderPresent()
{
    SDL_RenderPresent(renderer);
}

int Window::getWidth()
{
    int w;

    SDL_GetWindowSize(window, &w, NULL);

    return w;
}

int Window::getHeight()
{
    int h;

    SDL_GetWindowSize(window, NULL, &h);

    return h;
}
