#include "../include/Window.hpp"
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_ttf.h>

SDL_Window* Window::window = nullptr;
SDL_Renderer* Window::renderer = nullptr;

bool Window::Init(const char* title, int x, int y, int w, int h, int windowFlags)
{
    window = SDL_CreateWindow(title, x, y, w, h, windowFlags);
    if (window == NULL)
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL)
    {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
        
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    return true;
}

void Window::Clean()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

SDL_Texture* Window::LoadTexture(const char* filename)
{
    SDL_Surface* image = nullptr;

    image = IMG_Load(filename);
    if (!image)
    {
        printf("Image %s could not be loaded! IMG_Error: %s\n", filename, IMG_GetError());
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);

    return texture;
}

SDL_Texture* Window::LoadText(const char* filename, const char* text, int size, SDL_Color color)
{
    TTF_Font* font = TTF_OpenFont(filename, size);

    SDL_Surface* surface = nullptr;

    surface  = TTF_RenderText_Solid(font, text, color);
    if (!surface)
    {
        printf("Text %s could not be loaded! TTF_Error: %s\n", text, TTF_GetError());
    }
    
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    TTF_CloseFont(font);

    return texture;
}

void Window::Update()
{
    float scaleX = (float) Window::getRealWidth() / Window::getWidth();
    float scaleY = (float) Window::getRealHeight() / Window::getHeight();

    SDL_RenderSetScale(renderer, scaleX, scaleY);
}

void Window::Render(SDL_Texture* texture, SDL_Rect* textureRect, SDL_Rect* windowRect)
{
    if (SDL_RenderCopy(renderer, texture, textureRect, windowRect) < 0)
    {
        printf("Texture could not be rendered! SDL_Error: %s\n", SDL_GetError());
    }
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
    return 192;
}

int Window::getHeight()
{
    return 256;
}

int Window::getRealWidth()
{
    int w;

    SDL_GetWindowSize(window, &w, NULL);

    return w;
}

int Window::getRealHeight()
{
    int h;

    SDL_GetWindowSize(window, NULL, &h);

    return h;
}

float Window::getTopPaddingH()
{
    return 40;
}
