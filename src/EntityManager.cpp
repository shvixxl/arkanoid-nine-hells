#include "../include/EntityManager.hpp"
#include "../include/Window.hpp"

std::vector<Sphere> EntityManager::spheres;
Ship* EntityManager::ship = nullptr;

void EntityManager::Clean()
{
    delete ship;
    ship = nullptr;

    spheres.clear();
}


// Functions for ship management

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
                default:
                    break;
            }
        }
    }
}

bool EntityManager::ShipCheckCollision(SDL_Rect* objectRect)
{
    SDL_Rect shipRect = ship->getRect();

    return SDL_HasIntersection(objectRect, &shipRect);
}






// Functions for spheres management

void EntityManager::throwSphere(Spheres type)
{
    SDL_Rect shipRect = ship->getRect();

    int x = shipRect.x + shipRect.w / 2;
    int y = shipRect.y + shipRect.h;

    EntityManager::addSphere(type, x, y, ship->getSpeed(), 0);
}

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






// Ship class

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

    speedBoost = 0.1;
    speedSlow = 0.025;

    textureRect.h = 32;
    textureRect.w = 42;
    textureRect.x = 0;
    textureRect.y = 0;

    windowRect.h = textureRect.h;
    windowRect.w = textureRect.w;
    windowRect.x = Window::getWidth() / 2 - windowRect.w / 2;
    windowRect.y = Window::getTopPaddingH();

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
    
    Slow();

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
    if (speed < -speedSlow)
    {
        speed += speedSlow;
    }
    else if (speed > speedSlow)
    {
        speed -= speedSlow;
    }
    else
    {
        speed = 0;
    }
}

void Ship::Power()
{

}






// Sphere class

Sphere::Sphere(Spheres type, int x, int y, float speedX, float speedY)
{
    const char* filename;
    switch (type)
    {
        case driftglobe:
            filename = "assets/driftglobe.png";
            break;

        default:
            filename = "assets/SphereA.png";
            break;
    }

    texture = Window::LoadTexture(filename);

    textureRect.h = 16;
    textureRect.w = 16;
    textureRect.x = 0;
    textureRect.y = 0;

    frames = 4;
    frameDelay = 100;

    windowRect.h = textureRect.h;
    windowRect.w = textureRect.w;
    windowRect.x = x - windowRect.w / 2;
    windowRect.y = y - windowRect.h / 2;

    this->x = windowRect.x;
    this->y = windowRect.y;
    this->r = windowRect.w / 2;

    maxSpeed = 2;
    reduceSpeed = 0.1;

    this->speedY = maxSpeed + speedY;
    this->speedX = speedX;
}

Sphere::~Sphere()
{
    //SDL_DestroyTexture(texture);
}

void Sphere::Update()
{
    // Movement
    x += speedX;
    y += speedY;

    // Borders collision

    // Top border
    if (y + windowRect.h < 1)
    {

    }
    // Bottom border
    if (y + windowRect.h >= Window::getHeight())
    {
        speedY *= -1;
        y = Window::getHeight() - windowRect.h;
    }
    // Left border
    if (x <= windowRect.w)
    {
        speedX += reduceSpeed * (windowRect.w - x);
    }
    // Right border
    // This is opposite border to the left so here is a little confusing formula:
    //
    // [Window::getWidth - windowRect.w - x] equal to the x on the other side
    // 
    // About [+ 1] in speedX formula... Idk why but this necessary to get
    // the same result as on the other side
    //
    if (Window::getWidth() - windowRect.w - x <= windowRect.w)
    {
        speedX -= reduceSpeed * ((windowRect.w) - ((Window::getWidth() - x - windowRect.w) + 1));
    }



    // Magic speed limit
    if (speedX > maxSpeed || speedX < -maxSpeed)
    {
        speedX = maxSpeed * (speedX < 0 ? -1 : 1);
    }
    if (speedY > maxSpeed || speedY < -maxSpeed)
    {
        speedY = maxSpeed * (speedY < 0 ? -1 : 1);
    }

    windowRect.x = x;
    windowRect.y = y;
}

void Sphere::Render()
{
    // Animation
    textureRect.x = ((SDL_GetTicks() / frameDelay) % frames) * textureRect.w;

    Window::Render(texture, &textureRect, &windowRect);
}

bool Sphere::CheckCollision(SDL_Rect* objectRect)
{
    float centerX = x + r;
    float centerY = y + r;

    float closestX;
    float closestY;

    // Find the closest X on object to the center of Sphere
    if (centerX < objectRect->x)
    {
        closestX = objectRect->x;
    }
    else if (centerX > objectRect->x + objectRect->w)
    {
        closestX = objectRect->x + objectRect->w;
    }
    else
    {
        closestX = centerX;
    }

    // Find the closest Y on object to the center of Sphere
    if (centerY < objectRect->y)
    {
        closestY = objectRect->y;
    }
    else if (centerY > objectRect->y + objectRect->h)
    {
        closestY = objectRect->y + objectRect->h;
    }
    else
    {
        closestY = centerY;
    }

    // If distance between the closest point on object to
    // the center of Sphere is less than radius then there is collision!
    if (pow(centerX - closestX, 2) + pow(centerY - closestY, 2) < pow(r, 2))
    {
        return true;
    }
    
    return false;
}

void Sphere::Rebound(SDL_Rect* objectRect, float objectSpeedX, float objectSpeedY)
{

    float centerX = x + r;
    float centerY = y + r;

    float closestX;
    float closestY;

    // Find the closest X on object to the center of Sphere
    if (centerX < objectRect->x)
    {
        closestX = objectRect->x;
    }
    else if (centerX > objectRect->x + objectRect->w)
    {
        closestX = objectRect->x + objectRect->w;
    }
    else
    {
        closestX = centerX;
    }

    // Find the closest Y on object to the center of Sphere
    if (centerY < objectRect->y)
    {
        closestY = objectRect->y;
    }
    else if (centerY > objectRect->y + objectRect->h)
    {
        closestY = objectRect->y + objectRect->h;
    }
    else
    {
        closestY = centerY;
    }

    float deltaX = abs(closestX - centerX);
    float deltaY = abs(closestY - centerY);

    // Here rebound is happen
    //
    // From inside of the object?? It is possible with high speed
    // of the ball and should be fixed... (TODO)
    if (deltaX == 0 && deltaY == 0)
    {

    }
    // From North or South side. Just change Y direction to the opposite
    // if it isn't done already
    else if (deltaX == 0)
    {
        if ((closestY > centerY && speedY > 0) || (closestY < centerY && speedY < 0))
        {
            speedY *= -1;
        }
    }
    // From West or East side. Just change X direction to the opposite
    // if it isn't done already
    else if (deltaY == 0)
    {
        if ((closestX > centerX && speedX > 0) || (closestX < centerX && speedX < 0))
        {
            speedX *= -1;
        }
    }
    // Corners.. Change (TODO) 
    else
    {
        if ((closestY > centerY && speedY > 0) || (closestY < centerY && speedY < 0))
        {
            speedY *= -1;
        }
        else if ((closestX > centerX && speedX > 0) || (closestX < centerX && speedX < 0))
        {
            speedX *= -1;
        }
    }

    // Transfer speed from object to ball (TODO when "corner rebound" will complete)
    speedX += objectSpeedX;
    speedY += objectSpeedY;
}

