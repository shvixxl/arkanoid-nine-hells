#include "../include/Player.hpp"
#include "../include/Window.hpp"

Player::Player(const char* filename)
{
    texture = Window::LoadTexture(filename);
    
    textureRect.h = 32;
    textureRect.w = 42;
    textureRect.x = 0;
    textureRect.y = 0;

    windowRect.h = textureRect.h * 2;
    windowRect.w = textureRect.w * 2;
    windowRect.x = 512 / 2 - windowRect.w / 2;
    windowRect.y = 5;
}

Player::~Player()
{
    SDL_DestroyTexture(texture);
}

void Player::Update()
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
        if (speed < 0)
        {
            speed += speedBoost;
        }
        else if (speed > 0)
        {
            speed -= speedBoost;
        }
    }

    // Move player
    windowRect.x += speed;

}

void Player::Render()
{
    Window::Render(texture, &textureRect, &windowRect);
}

void Player::HandleEvents(SDL_Event* event)
{
    if (event->type == SDL_KEYDOWN && event->key.repeat == 0)
    {
        switch (event->key.keysym.sym)
        {
            case SDLK_LEFT:
                move += MOVE_LEFT;
                break;
            case SDLK_RIGHT:
                move += MOVE_RIGHT;
                break;
            default:
                break;
        }
    }

    if (event->type == SDL_KEYUP && event->key.repeat == 0)
    {
        switch (event->key.keysym.sym)
        {
            case SDLK_LEFT:
                move -= MOVE_LEFT;
                break;
            case SDLK_RIGHT:
                move -= MOVE_RIGHT;
                break;
            default:
                break;
        }
    }

}

