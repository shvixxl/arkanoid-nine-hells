#include "../include/MapManager.hpp"
#include "../include/Window.hpp"
#include <SDL2/SDL_timer.h>


int MapManager::mapHeight;
int MapManager::mapWidth;

int** MapManager::map = nullptr;

int MapManager::step;
int MapManager::current;

int MapManager::transitionStep;
int MapManager::currentTransitionStep;
SDL_TimerID MapManager::transitionTimer;

SDL_Texture* MapManager::backgroundTexture = nullptr;
SDL_Texture* MapManager::brickTexture = nullptr;
SDL_Texture* MapManager::crackTexture = nullptr;
SDL_Texture* MapManager::powerTexture = nullptr;

Background* MapManager::background = nullptr;
std::vector<Brick> MapManager::bricks;
std::vector<Power> MapManager::powers;


void MapManager::Init(Levels type, int h, int w)
{
    switch (type)
    {
        case avernus:
            backgroundTexture = Window::LoadTexture("assets/avernus.png");
            brickTexture = Window::LoadTexture("assets/avernus_bricks.png");
            crackTexture = Window::LoadTexture("assets/crack.png");
            powerTexture = Window::LoadTexture("assets/soul.png");
            break;
        default:
            return;
            break;
    }

    background = new Background();

    mapHeight = h;
    mapWidth = w;
    map = new int*[mapHeight];
    for (int i = 0; i < mapHeight; i++)
    {
        map[i] = new int[mapWidth];
    }

    step = 12;
    current = 0;

    transitionStep = 16;
    currentTransitionStep = 0;
    transitionTimer = SDL_AddTimer(50, MapManager::TransitionStep, nullptr);

    Generate(2);
    Next();
}

void MapManager::Clean()
{
    bricks.clear();

    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(brickTexture);
    SDL_DestroyTexture(crackTexture);
    SDL_DestroyTexture(powerTexture);
}

void MapManager::Generate(int spire)
{
    // First X on current Y and the last X
    int firstX, lastX;

    // Chance to make map wider on current Y
    int growthStep = mapHeight / mapWidth;
    int growth = 0;

    // If map should have spire (spire != 0)
    if (spire)
    {
        firstX = (mapWidth - ((spire + 1) / 2)) / 2;
        lastX  = (mapWidth + ((spire - 1) / 2)) / 2;
    }

    for (int y = 0; y < mapHeight; y++)
    {
        for (int x = firstX; x <= lastX; x++)
        {
            map[y][x] = 1;
        }

        growth++;

        if (growth >= growthStep)
        {
            growth = 0;

            if (firstX > 0)
            {
                firstX--;
            }
            if (lastX < mapWidth - 1)
                lastX++;
            }
    }
}

// Load next <step> rows of bricks on the screen
void MapManager::Next()
{
    bricks.clear();
    int previous = current;
    current += step;

    for (int y = previous; y < current && y < mapHeight; y++)
    {
        for (int x = 0; x < mapWidth; x++)
        {
            if (map[y][x] == 1)
            {
                bricks.push_back(Brick(x, y - previous));
            }
        }
    }
    
    currentTransitionStep += transitionStep;
}

void MapManager::Update()
{
    MapManager::UpdateBackground();
    MapManager::UpdatePowers();
    MapManager::UpdateBricks();

    if (bricks.empty())
    {
        MapManager::Next();
    }
}

void MapManager::UpdateBackground()
{
    background->Update();
}

void MapManager::UpdateBricks()
{
    for (size_t i = 0; i < bricks.size(); ++i)
    {
        bricks.at(i).Update();

        SDL_Rect brickRect = bricks.at(i).getRect();

        std::vector<size_t> collided_spheres = EntityManager::SpheresCheckCollision(&brickRect);

        for (size_t k = 0; k < collided_spheres.size(); ++k)
        {
            EntityManager::SpheresRebound(collided_spheres.at(k), &brickRect, 0, 0);

            if (bricks.at(i).Hit())
            {
                bricks.erase(bricks.begin() + i);

                // Add power after the destruction of the brick
                powers.push_back(Power(&brickRect));

                break;
            }
        }
    }
}

void MapManager::UpdatePowers()
{
    for (size_t i = 0; i < powers.size(); ++i)
    {
        powers.at(i).Update();

        SDL_Rect powerRect = powers.at(i).getRect();

        if (EntityManager::ShipCheckCollision(&powerRect))
        {
            SpellManager::GenerateSpell();
            powers.erase(powers.begin() + i);
        }
        else if (powerRect.y + powerRect.h < 0)
        {
            powers.erase(powers.begin() + i);
        }
    }
}

void MapManager::Render()
{
    background->Render(backgroundTexture);

    for (size_t i = 0; i < bricks.size(); ++i)
    {
        bricks.at(i).Render(brickTexture, crackTexture);
    }

    for (size_t i = 0; i < powers.size(); ++i)
    {
        powers.at(i).Render(powerTexture);
    }
}

Uint32 MapManager::TransitionStep(Uint32 interval, void*)
{
    if (transitionStep > 0)
    {
        background->TransitionStep();

        for (size_t i = 0; i < bricks.size(); ++i)
        {
            bricks.at(i).TranstionStep();
        }

        for (size_t i = 0; i < powers.size(); ++i)
        {
            powers.at(i).TranstionStep();
        }

        transitionStep -= 1;
    }

    return interval;
}






Brick::Brick(int x, int y)
{
    int type = rand() % 9;

    brickRect.h = 8;
    brickRect.w = 16;
    brickRect.x = (type / 3) * brickRect.w;
    brickRect.y = (type % 3) * brickRect.h;

    crackRect.x = 0;
    crackRect.y = 0;
    crackRect.h = brickRect.h;
    crackRect.w = brickRect.w;

    windowRect.x = brickRect.w * x;
    windowRect.y = Window::getHeight() + (brickRect.h * y);
    windowRect.h = brickRect.h;
    windowRect.w = brickRect.w;

    health = 5;
}

Brick::~Brick()
{

}

bool Brick::Hit()
{
    int damage = rand() % 4 + 1;

    health -= damage;
    crackRect.x += crackRect.w * damage;

    // Return true if this brick is broken (health <= 0)
    return !(health > 0);
}

void Brick::Update()
{

}

void Brick::TranstionStep()
{
    windowRect.y -= 6;
}

void Brick::Render(SDL_Texture* brickTexture, SDL_Texture* crackTexture)
{
    Window::Render(brickTexture, &brickRect, &windowRect);
    Window::Render(crackTexture, &crackRect, &windowRect);
}






Background::Background()
{
    textureRect.x = 0;
    textureRect.y = 0;
    textureRect.h = 256;
    textureRect.w = 192;

    frames = 1;
    frameDelay = 100;

    windowRect.x = 0;
    windowRect.y = 0;
    windowRect.h = Window::getHeight();
    windowRect.w = Window::getWidth();
}

Background::~Background()
{

}

void Background::Render(SDL_Texture* texture)
{
    Window::Render(texture, &textureRect, &windowRect);
}

void Background::Update()
{

}

void Background::TransitionStep()
{
    textureRect.y += 1;
}







Power::Power(SDL_Rect* objectRect)
{
    textureRect.x = 0;
    textureRect.y = 0;
    textureRect.w = 12;
    textureRect.h = 8;

    speed = 2;

    frames = 4;
    frameDelay = 100;

    windowRect.w = textureRect.w;
    windowRect.h = textureRect.h;
    windowRect.x = objectRect->x + objectRect->w / 2 - windowRect.w / 2;
    windowRect.y = objectRect->y + objectRect->h / 2 - windowRect.h / 2;
}

Power::~Power()
{

}

void Power::Update()
{
    windowRect.y -= speed;
}

void Power::Render(SDL_Texture* texture)
{
    // Animation
    textureRect.x = ((SDL_GetTicks() / frameDelay) % frames) * textureRect.w;

    Window::Render(texture, &textureRect, &windowRect);
}

void Power::TranstionStep()
{
    windowRect.y -= 6;
}

