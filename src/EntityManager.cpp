#include "../include/EntityManager.hpp"
#include "../include/Window.hpp"
#include <cstddef>

std::vector<Sphere> EntityManager::spheres;
Ship* EntityManager::ship = nullptr;

void EntityManager::Clean()
{
    delete ship;
    ship = nullptr;

    spheres.clear();
}


// Functions for ships

void EntityManager::addShip(Ships type)
{
    ship = new Ship(type);
}


void EntityManager::UpdateShip()
{
    ship->Update();

    SDL_Rect shipRect = ship->getRect();

    std::vector<size_t> collided_spheres = EntityManager::SpheresCheckCollision(&shipRect);
    for (size_t k = 0; k < collided_spheres.size(); ++k)
    {
        EntityManager::SpheresRebound(collided_spheres.at(k), &shipRect, ship->getSpeed(), 0);
    }
}


void EntityManager::RenderShip()
{
        ship->Render();
}

void EntityManager::ShipHandleEvents(SDL_Event* event)
{
    if (ship)
    {
        if (event->type == SDL_KEYDOWN && event->key.repeat == 0)
        {
            switch (event->key.keysym.sym)
            {
                case SDLK_LEFT:
                    ship->Move(+MOVE_LEFT);
                    break;
                case SDLK_RIGHT:
                    ship->Move(+MOVE_RIGHT);
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
                    ship->Move(-MOVE_LEFT);
                    break;
                case SDLK_RIGHT:
                    ship->Move(-MOVE_RIGHT);
                    break;
                case SDLK_SPACE:
                    EntityManager::addSphere(driftglobe, ship->getRect().x + ship->getRect().w / 2, ship->getRect().y + ship->getRect().w, ship->getSpeed(), 0);
                    break;
                default:
                    break;
            }
        }
    }
}






// Functions for spheres

void EntityManager::addSphere(Spheres type, int x, int y, float speedX, float speedY)
{
    spheres.push_back(Sphere(type, x, y, speedX, speedY));
}


void EntityManager::UpdateSpheres()
{
    for (size_t i = 0; i < spheres.size(); ++i)
    {
        spheres.at(i).Update();
    }
}


void EntityManager::RenderSpheres()
{
    for (auto it = spheres.begin(); it != spheres.end(); ++it)
    {
        it->Render();
    }
}


std::vector<size_t> EntityManager::SpheresCheckCollision(SDL_Rect* objectRect)
{
    std::vector<size_t> collided_spheres;

    for (size_t i = 0; i < spheres.size(); ++i)
    {
        if (spheres.at(i).CheckCollision(objectRect))
        {
            collided_spheres.push_back(i);
        }
    }
    return collided_spheres;
}


void EntityManager::SpheresRebound(size_t id, SDL_Rect* objectRect, float objectSpeedX, float objectSpeedY)
{
    spheres.at(id).Rebound(objectRect, objectSpeedX, objectSpeedY);
}

