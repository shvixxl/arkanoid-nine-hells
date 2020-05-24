#include "../include/EntityManager.hpp"
#include "../include/Window.hpp"

std::vector<Sphere> EntityManager::spheres;
std::vector<Ship> EntityManager::ships;

void EntityManager::Clean()
{
    ships.clear();
    spheres.clear();
}


// Functions for ships

void EntityManager::addShip(Ships type)
{
    ships.push_back(Ship(type));
}


void EntityManager::UpdateShips()
{
    for (size_t i = 0; i < ships.size(); ++i)
    {
        ships.at(i).Update();

        SDL_Rect shipRect = ships.at(i).getRect();

        std::vector<size_t> collided_spheres = EntityManager::SpheresCheckCollision(&shipRect);
        for (size_t k = 0; k < collided_spheres.size(); ++k)
        {
            EntityManager::SpheresRebound(k, &shipRect, ships.at(i).getSpeed(), 0);
        }
    }
}


void EntityManager::RenderShips()
{
    for (size_t i = 0; i < ships.size(); ++i)
    {
        ships.at(i).Render();
    }
}

void EntityManager::ShipsHandleEvents(SDL_Event* event)
{
    if (!ships.empty())
    {
        if (event->type == SDL_KEYDOWN && event->key.repeat == 0)
        {
            switch (event->key.keysym.sym)
            {
                case SDLK_LEFT:
                    ships.at(0).Move(+MOVE_LEFT);
                    break;
                case SDLK_RIGHT:
                    ships.at(0).Move(+MOVE_RIGHT);
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
                    ships.at(0).Move(-MOVE_LEFT);
                    break;
                case SDLK_RIGHT:
                    ships.at(0).Move(-MOVE_RIGHT);
                    break;
                case SDLK_SPACE:
                    EntityManager::addSphere(annihilation, ships.at(0).getRect().x / 2, ships.at(0).getRect().y + ships.at(0).getRect().w, ships.at(0).getSpeed());
                    break;
                default:
                    break;
            }
        }
    }
}






// Functions for spheres

void EntityManager::addSphere(Spheres type, int x, int y, float paddle_speed)
{
    spheres.push_back(Sphere(type, x, y, paddle_speed));
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
    for (size_t i = 0; i < spheres.size(); ++i)
    {
        spheres.at(i).Render();
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

