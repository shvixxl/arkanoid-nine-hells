#include "../include/Player.hpp"
#include "../include/TextureManager.hpp"

GameObject::GameObject(const char* filename, int x, int y)
{
    this->texture = TextureManager::LoadTexture(filename);

    this->x = x;
    this->y = y;
}

void GameObject::Update()
{
    x++;
    y++;

    srcRect.h = 32;
    srcRect.w = 42;
    srcRect.x = 0;
    srcRect.y = 0;

    destRect.x = x;
    destRect.y = y;
    destRect.h = srcRect.h * 2;
    destRect.w = srcRect.w * 2;
}

void GameObject::Render()
{
    SDL_RenderCopy(Game::renderer, texture, &srcRect, &destRect);
}


