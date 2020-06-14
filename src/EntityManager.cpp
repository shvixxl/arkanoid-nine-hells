#include "../include/EntityManager.hpp"
#include "../include/Window.hpp"
#include "../include/SpellManager.hpp"
#include "../include/MapManager.hpp"
#include <SDL2/SDL_hints.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_shape.h>

std::string EntityManager::ship_type;
std::string EntityManager::sphere_type;

std::vector<Sphere> EntityManager::spheres;
Ship* EntityManager::ship = nullptr;



void EntityManager::Init(const char* ship, const char* sphere)
{
    ship_type = ship;
    sphere_type = sphere;
}

void EntityManager::Clean()
{
    delete ship;
    ship = nullptr;

    for (auto i = spheres.begin(); i != spheres.end(); i++)
    {
        i->Clean();
    }
    spheres.clear();
}


// Functions for ship management

void EntityManager::addShip()
{
    ship = new Ship(ship_type.c_str(), -1, -1);
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

void EntityManager::throwSphere()
{
    SDL_Rect shipRect = ship->getRect();

    int x = shipRect.x + shipRect.w / 2;
    int y = shipRect.y + shipRect.h + 10;

    EntityManager::addSphere(sphere_type.c_str(), x, y, ship->getSpeed(), 0);
}

void EntityManager::addSphere(const char* type, int x, int y, float speedX, float speedY)
{
    spheres.push_back(Sphere(type, x, y, speedX, speedY));
}


void EntityManager::UpdateSpheres()
{
    for (size_t i = 0; i < spheres.size(); ++i)
    {
        if (spheres.at(i).Update() == -1)
        {
            spheres.erase(spheres.begin() + i);
            i -= 1;
        }
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

bool EntityManager::isSpheres()
{
    return !spheres.empty();
}





// Ship class

Ship::Ship(const char* type, int x, int y)
{
    this->type = type;

    std::ifstream file("data/ships.json");
    if (!file.is_open())
    {
        printf("Some data files is missing!");
    }
    file >> data;
    file.close();

    data = data[type];

    texture = Window::LoadTexture(data["texture"].asString().c_str());

    move = 0;
    speed = 0;

    speedBoost = data["speed_boost"].asFloat();
    speedSlow = data["speed_slow"].asFloat();

    textureRect.h = 32;
    textureRect.w = 42;
    textureRect.x = 0;
    textureRect.y = 0;
    
    windowRect.h = textureRect.h;
    windowRect.w = textureRect.w;
    if (x == -1)
    {
        windowRect.x = Window::getWidth() / 2 - windowRect.w / 2;
    }
    else 
    {
        windowRect.x = x;
    }
    if (y == -1)
    {
        windowRect.y = Window::getTopPaddingH();
    }
    else
    {
        windowRect.y = y;
    }

    paddleRect.h = 32;
    paddleRect.w = data["paddle_width"].asInt();
    paddleRect.x = windowRect.x + windowRect.w / 2 - paddleRect.w / 2;
    paddleRect.y = windowRect.y;

    this->x = paddleRect.x;
    
    animation_timer = new Timer(50);
    lastX = this->x;
    frames = 1;
    frame = rand() % 1;

    isDisplaced = false;
    isHasted = false;
}

Ship::~Ship()
{
    SDL_DestroyTexture(texture);
}

void Ship::Update()
{
    Displace();
    Haste();

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
    if (x < 1)
    {
        x = 1;
        speed = 0;
    }
    else if (x + paddleRect.w > Window::getWidth())
    {
        x = Window::getWidth() - paddleRect.w;
        speed = 0;
    }

    // Haste effect (save position)
    if (animation_timer->Ready())
    {
        lastX = x;
    }

    paddleRect.x = x;
    windowRect.x = paddleRect.x - ((windowRect.w - paddleRect.w) / 2);
}

void Ship::Render()
{

    if (isDisplaced)
    {

        SDL_Rect displaceRect = windowRect;

        Uint8 tempAlpha;
        SDL_GetTextureAlphaMod(texture, &tempAlpha);

        if (animation_timer->Ready())
        {
            tempAlpha = rand() % 32 + 96;
        }

        for (int i = 1; i <= 2; i++)
        {
            SDL_SetTextureAlphaMod(texture, tempAlpha / i);

            displaceRect.x -= (windowRect.w / SpellManager::getDisplacement()) / 2 * i;
            Window::Render(texture, &textureRect, &displaceRect);
            displaceRect.x = windowRect.x;

            displaceRect.x += (windowRect.w / SpellManager::getDisplacement()) / 2 * i;
            Window::Render(texture, &textureRect, &displaceRect);
            displaceRect.x = windowRect.x;
        }

        SDL_SetTextureAlphaMod(texture, tempAlpha);
    }

    if (isHasted)
    {
        SDL_Rect hasteRect = windowRect;

        Uint8 tempAlpha;
        SDL_GetTextureAlphaMod(texture, &tempAlpha);

        for (int i = 1; i < abs(lastX - x); i++)
        {
            SDL_SetTextureAlphaMod(texture, 48);
            hasteRect.x -= 2 *  (lastX > x ? -1 : 1);

            Window::Render(texture, &textureRect, &hasteRect);
        }

        SDL_SetTextureAlphaMod(texture, tempAlpha);
    }

    // Animation
    if (animation_timer->Ready())
    {
        frame += 1;
        if (frame >= frames)
        {
            frame = 0;
        }
        textureRect.x = (frame) * textureRect.w;

        animation_timer->Restart();
    }

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

void Ship::Displace()
{
    if (SpellManager::SpellDisplacement() && !isDisplaced)
    {
        paddleRect.w *= SpellManager::getDisplacement();
        paddleRect.x -= (paddleRect.w - paddleRect.w / SpellManager::getDisplacement()) / 2;
        x = paddleRect.x;

        isDisplaced = true;

        SDL_SetTextureAlphaMod(texture, 128);
    }
    else if (!SpellManager::SpellDisplacement() && isDisplaced)
    {
        paddleRect.w /= SpellManager::getDisplacement();
        paddleRect.x += (paddleRect.w * SpellManager::getDisplacement() - paddleRect.w) / 2;
        x = paddleRect.x;

        SDL_SetTextureAlphaMod(texture, 255);
        
        isDisplaced = false;
    }
}

void Ship::Haste()
{
    if (SpellManager::SpellHaste() && !isHasted)
    {
        speedBoost *= SpellManager::getHaste();
        speedSlow *= SpellManager::getHaste();

        isHasted = true;
    }
    else if (!SpellManager::SpellHaste() && isHasted)
    {
        speedBoost /= SpellManager::getHaste();
        speedSlow /= SpellManager::getHaste();
        
        isHasted = false;
    }
}






// Sphere class

Sphere::Sphere(const char* type, int x, int y, float speedX, float speedY)
{
    this->type = type;

    std::ifstream file("data/spheres.json");
    if (!file.is_open())
    {
        printf("Some data files is missing!");
    }
    file >> data;
    file.close();

    data = data[type];

    texture = Window::LoadTexture(data["texture"].asString().c_str());

    textureRect.h = 16;
    textureRect.w = 16;
    textureRect.x = 0;
    textureRect.y = 0;

    animation_timer = new Timer(100);
    frames = 4;
    frame = rand() % 4;

    find_path_delay = new Timer(50);
    find_path_dots = nullptr;

    windowRect.h = textureRect.h;
    windowRect.w = textureRect.w;
    windowRect.x = x - windowRect.w / 2;
    windowRect.y = y - windowRect.h / 2;

    this->x = windowRect.x;
    this->y = windowRect.y;

    movement_type = data["movement_type"].asInt();
    max_speed = data["max_speed"].asFloat();

    if (movement_type == 2)
    {
        this->speedY = data["start_speed"].asFloat() + speedY;
    }
    else
    {
        this->speedY = max_speed;
    }
    this->speedX = speedX;
}

Sphere::~Sphere()
{

}

void Sphere::Clean()
{
    SDL_DestroyTexture(texture);
}

int Sphere::Update()
{
    // ----MOVEMENT----
    // Types:
    //
    // 1 - standard
    // 2 - gravity
    
    // ---Speed changes---
    if (movement_type == 1)
    {

    }
    else if (movement_type == 2)
    {
        speedY += data["speed_boost"].asFloat() * (speedY >= 0 ? 1 : -1);
    }


    // ---Borders collision---
    // If above top border then delete ball (return -1 to EntityManager)
    if (y + windowRect.h < 1)
    {
        return -1;
    }
    // Bottom
    if (y + windowRect.h > Window::getHeight() && speedY > 0)
    {
        if (movement_type == 2)
        {
            speedY = data["start_speed"].asFloat() * -1;
        }
        else
        {
            speedY *= -1;
        }

        y = Window::getHeight() - windowRect.h;
    }
    // Right
    if (x < 0)
    {
        speedX *= -1;
        x = 0;
    }
    // Left
    if (x + windowRect.w > Window::getWidth())
    {
        speedX *= -1;
        x = Window::getWidth() - windowRect.w;
    }

    // ---Speed Limit---
    if (speedX > max_speed)
    {
        speedX = max_speed;
    }
    else if (speedX < -max_speed)
    {
        speedX = -max_speed;
    }

    // ---Change position---
    x += speedX;
    y += speedY;
    windowRect.x = x;
    windowRect.y = y;

    return 0;
}

void Sphere::Render()
{
    if (SpellManager::SpellFindPath())
    {
        if (find_path_dots == nullptr)
        {
            FindPath();
        }

        if (find_path_delay->Ready())
        {
            FindPath();
            find_path_delay->Restart();
        }

        for (int dot = 0; dot < SpellManager::getFindPath(); dot++)
        {
            SDL_SetRenderDrawColor(Window::getRenderer(), 255, 255, 255, 255);
            SDL_RenderDrawPoint(Window::getRenderer(), find_path_dots[dot]->x + find_path_dots[dot]->w / 2, find_path_dots[dot]->y + find_path_dots[dot]->h / 2);
        }
    }
    else
    {
        find_path_dots = nullptr;
    }

    // Animation
    if (animation_timer->Ready())
    {
        frame += 1;
        if (frame >= frames)
        {
            frame = 0;
        }
        textureRect.x = (frame) * textureRect.w;

        animation_timer->Restart();
    }

    Window::Render(texture, &textureRect, &windowRect);
}

bool Sphere::CheckCollision(SDL_Rect* objectRect)
{
    return SDL_HasIntersection(&windowRect, objectRect);
}

void Sphere::Rebound(SDL_Rect* objectRect, float objectSpeedX, float objectSpeedY)
{
    int deltaX = 0;
    int deltaY = 0;

    // deltaX
    if (windowRect.x < objectRect->x)
    {
        deltaX = objectRect->x - windowRect.x;
    }
    else if (windowRect.x + windowRect.w > objectRect->x + objectRect->w)
    {
        deltaX = (objectRect->x + objectRect->w) - (windowRect.x + windowRect.w);
    }

    // deltaY
    if (windowRect.y < objectRect->y)
    {
        deltaY = objectRect->y - windowRect.y;
    }
    else if(windowRect.y + windowRect.h > objectRect->y + objectRect->h)
    {
        deltaY = (objectRect->y + objectRect->h) - (windowRect.y + windowRect.h);
    }

    // Rebound
    if (abs(deltaX) >= abs(deltaY))
    {
        // West
        if (deltaX > 0 && speedX > 0)
        {
            speedX *= -1;
        }
        // East
        else if (deltaX < 0 && speedX < 0)
        {
            speedX *= -1;
        }
    }
    else
    {
        // North
        if (deltaY > 0 && speedY > 0)
        {
            if (movement_type == 2)
            {
                speedY = data["start_speed"].asFloat() * -1;
            }
            else
            {
                speedY *= -1;
            }
        }
        // South
        else if (deltaY < 0 && speedY < 0)
        {
            if (movement_type == 2)
            {
                speedY = data["start_speed"].asFloat();
            }
            else
            {
                speedY *= -1;
            }
        }
    }

    // Add object speed to sphere speed
    speedX += objectSpeedX;
    speedY += objectSpeedY;
}

void Sphere::FindPath()
{
    find_path_dots = new SDL_Rect*[SpellManager::getFindPath()];

    float temp_x = x;
    float temp_y = y;
    float temp_speedX = speedX;
    float temp_speedY = speedY;

    std::vector<SDL_Rect> bricksRect = MapManager::GetBricksRect();

    for (int dot = 0; dot < SpellManager::getFindPath(); dot++)
    {
        for (int step = 0; step < 20; step++)
        {
            for (size_t i = 0; i < bricksRect.size(); i++)
            {
                if (CheckCollision(&bricksRect.at(i)))
                {
                    Rebound(&bricksRect.at(i), 0, 0);
                }
            }
            Update();
        }

        find_path_dots[dot] = new SDL_Rect(windowRect);
    }

    x = windowRect.x = temp_x;
    y = windowRect.y = temp_y;
    speedX = temp_speedX;
    speedY = temp_speedY;
}

