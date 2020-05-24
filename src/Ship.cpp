#include "../include/Ship.hpp"
#include "../include/Sphere.hpp"
#include "../include/EntityManager.hpp"
#include "../include/Window.hpp"

Ship::Ship(Ships type)
{
    const char* filename;
    switch (type)
    {
        case skyship:
            filename = "assets/skyship.png";
            break;

        default:
            filename = "assets/skyship.png";
            break;
    }
    
    texture = Window::LoadTexture(filename);

    move = 0;
    speed = 0;

    speedBoost = 0.5;
    speedSlow = 0.05;

    textureRect.h = 32;
    textureRect.w = 42;
    textureRect.x = 0;
    textureRect.y = 0;

    windowRect.h = textureRect.h * Window::getMultiplierH();
    windowRect.w = textureRect.w * Window::getMultiplierW();
    windowRect.x = Window::getWidth() / 2 - windowRect.w / 2;
    windowRect.y = 5;

    x = windowRect.x;
}

Ship::~Ship()
{
    SDL_DestroyTexture(texture);
}

void Ship::Update()
{
    // Speed change
    if (move == MOVE_LEFT)
    {
        speed -= speedBoost;
    }
    else if (move == MOVE_RIGHT)
    {
        speed += speedBoost;
    }
    else if (move == MOVE_LEFT + MOVE_RIGHT)
    {
        Slow();
    }

    x += speed;

    // Change position
    if (x <= 1)
    {
        x = 1;
        speed = 0;
    }
    else if (x + windowRect.w >= Window::getWidth())
    {
        x = Window::getWidth() - windowRect.w;
        speed = 0;
    }

    windowRect.x = x;
}

void Ship::Render()
{
    Window::Render(texture, &textureRect, &windowRect);
}

void Ship::Move(int MOVE)
{
    move += MOVE;
}

void Ship::Slow()
{
    if (speed < 0)
    {
        speed += speedSlow;
    }
    else if (speed > 0)
    {
        speed -= speedSlow;
    }
}

void Ship::Power()
{

}

