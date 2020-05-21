#include "../include/Power.hpp"
#include "../include/Window.hpp"

Power::Power(SDL_Rect* objectRect)
{
    textureRect.x = 0;
    textureRect.y = 0;
    textureRect.w = 16;
    textureRect.h = 16;

    frames = 5;
    frameDelay = 100;

    windowRect.w = textureRect.w * Window::getMultiplierW();
    windowRect.h = textureRect.h * Window::getMultiplierH();
    windowRect.x = objectRect->x + objectRect->w / 2 - windowRect.w / 2;
    windowRect.y = objectRect->y + objectRect->h / 2 - windowRect.h / 2;
}

Power::~Power()
{
    
}

void Power::Update()
{
    textureRect.x = ((SDL_GetTicks() % frameDelay) / frames) * textureRect.w;
}

void Power::Render(SDL_Texture* texture)
{
    Window::Render(texture, &textureRect, &windowRect);
}

int Power::GeneratePower()
{
    return rand() % POWERS + 1;
}

