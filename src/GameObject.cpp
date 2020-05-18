#include "../include/GameObject.hpp"
#include "../include/TextureManager.hpp"

GameObject::GameObject(const char* filename, SDL_Renderer* renderer, int x, int y)
{
    this->renderer = renderer;
    this->texture = TextureManager::LoadTexture(filename, renderer);

    this->x = x;
    this->y = y;
}

void GameObject::Update()
{
    x++;
    y++;

    srcRect.h = 32;
    srcRect.w = 210;
    srcRect.x = 0;
    srcRect.y = 0;

    destRect.x = x;
    destRect.y = y;
    destRect.h = srcRect.h * 2;
    destRect.w = srcRect.w * 2;
}

void GameObject::Render()
{
    SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
}


