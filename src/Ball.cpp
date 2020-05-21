#include "../include/Ball.hpp"
#include "../include/Window.hpp"
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <math.h>

Ball::Ball(const char* filename, int x, int y, float paddleSpeed)
{
    texture = Window::LoadTexture(filename);

    textureRect.h = 16;
    textureRect.w = 16;
    textureRect.x = 0;
    textureRect.y = 0;

    frames = 5;
    frameDelay = 100;

    windowRect.h = textureRect.h * Window::getMultiplierH();
    windowRect.w = textureRect.w * Window::getMultiplierW();
    windowRect.x = x;
    windowRect.y = y;

    this->x = x;
    this->y = y;

    speedY = 10 - abs(paddleSpeed);
    speedX = (10 - speedY) * (paddleSpeed < 0 ? -1 : 1);
}

Ball::~Ball()
{
    SDL_DestroyTexture(texture);
}

void Ball::Update(float paddleSpeed, SDL_Rect paddleRect)
{
    // Movement
    x += speedX;
    y += speedY;

    // Paddle collison
    //
    // If the ball collided with the paddle
    if (SDL_HasIntersection(&paddleRect, &windowRect))
    {
        Rebound(&paddleRect, paddleSpeed, 0);
    }

    // Borders collision (smooth ball repulsion)

    // Left border
    if (x < windowRect.w)
    {
        speedX += SPEED_REDUCE;
    }
    // Right border
    else if (x + windowRect.w >= Window::getWidth())
    {
        speedX -= SPEED_REDUCE;
    }
    // Bottom border
    else if (y >= Window::getHeight() - windowRect.h)
    {
        speedY *= -1;
        y = Window::getHeight() - windowRect.h;
    }
    // Top border
    else if (y + windowRect.h < 1)
    {
        Game::stop();
        return;
    }

    // Magic speed limit
    if (speedX > MAX_SPEED || speedX < -MAX_SPEED)
    {
        speedX -= SPEED_REDUCE * (speedX < 0 ? -1 : 1);
    }
    if (speedY > MAX_SPEED || speedY < -MAX_SPEED)
    {
        speedX -= SPEED_REDUCE * (speedY < 0 ? -1 : 1);
    }

    windowRect.x = x;
    windowRect.y = y;

    // Animation
    textureRect.x = ((SDL_GetTicks() / frameDelay) % frames) * textureRect.w;
}

void Ball::Render()
{
    Window::Render(texture, &textureRect, &windowRect);
}

void Ball::Rebound(SDL_Rect* objectRect, float objectSpeedX, float objectSpeedY)
{
    // Determine side from which the ball will rebound and change
    // direction of ball movement

    int dx = objectRect->x - windowRect.x;
    int dy = objectRect->y - windowRect.y;
    
    int dx_ = (windowRect.x + windowRect.w) - (objectRect->x + objectRect->w);
    int dy_ = (windowRect.y + windowRect.h) - (objectRect->y + objectRect->h);

    // North
    if (dy > 0 && dy > dx && dy > dx_)
    {
        speedY *= speedY > 0 ? -1 : 1;
    }
    // South
    else if (dy_ > 0 && dy_ > dx && dy_ > dx_)
    {
        speedY *= speedY < 0 ? -1 : 1;
    }
    // West
    else if (dx > 0 && dx > dy && dx > dy_)
    {
        speedX *= speedX > 0 ? -1 : 1;
    }
    // East
    else if (dx_ > 0 && dx_ > dy && dx_ > dy_)
    {
        speedX *= speedX < 0 ? -1 : 1;
    }

    // Change speed (This is Physics, Baby!)
    speedY += objectSpeedY;
    speedX += objectSpeedX;
}
