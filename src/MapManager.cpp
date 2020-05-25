#include "../include/MapManager.hpp"
#include "../include/Window.hpp"


int MapManager::mapHeight;
int MapManager::mapWidth;

int** MapManager::map = nullptr;

int MapManager::step;
int MapManager::current;

SDL_Texture* MapManager::backgroundTexture = nullptr;
SDL_Texture* MapManager::brickTexture = nullptr;
SDL_Texture* MapManager::crackTexture = nullptr;
SDL_Texture* MapManager::powerTexture = nullptr;

Background* MapManager::background = nullptr;
std::vector<Brick> MapManager::bricks;
std::vector<Power> MapManager::powers;


void MapManager::Init(Maps type, int h, int w)
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

    step = 16;
    current = 0;

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
                bricks.push_back(Brick(x, y, step));
            }
        }
    }
}

void MapManager::Update()
{
    MapManager::UpdateBackground();
    MapManager::UpdateBricks();
    MapManager::UpdatePowers();
}

void MapManager::UpdateBackground()
{
    background->Update();
}

void MapManager::UpdateBricks()
{
    for (size_t i = 0; i < bricks.size(); ++i)
    {
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

        std::vector<size_t> collided_spheres = EntityManager::SpheresCheckCollision(&powerRect);

        if (!collided_spheres.empty())
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






Brick::Brick(int x, int y, int h)
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

    windowRect.x = brickRect.w * x * Window::getMultiplierW();
    windowRect.y = Window::getHeight() - (brickRect.h * (h - y) * Window::getMultiplierH());
    windowRect.h = brickRect.h * Window::getMultiplierH();
    windowRect.w = brickRect.w * Window::getMultiplierW();

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

void Brick::Render(SDL_Texture* brickTexture, SDL_Texture* crackTexture)
{
    Window::Render(brickTexture, &brickRect, &windowRect);
    Window::Render(crackTexture, &crackRect, &windowRect);
}






Background::Background()
{
    textureRect.x = 0;
    textureRect.y = 0;
    textureRect.h = 192;
    textureRect.w = 192;

    frames = 1;
    frameDelay = 100;

    windowRect.x = 0;
    windowRect.y = 0;
    windowRect.h = textureRect.h * Window::getMultiplierH();
    windowRect.w = textureRect.w * Window::getMultiplierW();
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
    textureRect.x = textureRect.w * ((SDL_GetTicks() / frameDelay) % frames);
}






Power::Power(SDL_Rect* objectRect)
{
    textureRect.x = 0;
    textureRect.y = 0;
    textureRect.w = 12;
    textureRect.h = 8;

    frames = 4;
    frameDelay = 100;

    windowRect.w = textureRect.w * Window::getMultiplierW();
    windowRect.h = textureRect.h * Window::getMultiplierH();
    windowRect.x = objectRect->x + objectRect->w / 2 - windowRect.w / 2;
    windowRect.y = objectRect->y + objectRect->h / 2 - windowRect.h / 2;
}

Power::~Power()
{

}

void Power::Update()
{

}

void Power::Render(SDL_Texture* texture)
{
    // Animation
    textureRect.x = ((SDL_GetTicks() / frameDelay) % frames) * textureRect.w;

    Window::Render(texture, &textureRect, &windowRect);
}

