#include "../include/Ball.hpp"
#include "../include/Window.hpp"

Ball::Ball(const char* filename)
{
    texture = Window::LoadTexture(filename);

    textureRect.h = 16;
    textureRect.w = 16;
    textureRect.x = 0;
    textureRect.y = 0;

    windowRect.h = textureRect.h * 2;
    windowRect.w = textureRect.w * 2;
    windowRect.x = 512 / 2 - windowRect.w / 2;
    windowRect.y = 47;
}

Ball::~Ball()
{
    SDL_DestroyTexture(texture);
}

void Ball::Update()
{

}

void Ball::Render()
{
    Window::Render(texture, &textureRect, &windowRect);
}

