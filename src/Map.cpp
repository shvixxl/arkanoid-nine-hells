#include "../include/Map.hpp"
#include "../include/Window.hpp"
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>

Map::Map(const char* backgroundTexture, const char* brickTexture, const char* crackTexture, const char* powerTexture, int h, int w)
{
    this->backgroundTexture = Window::LoadTexture(backgroundTexture);
    this->brickTexture = Window::LoadTexture(brickTexture);
    this->crackTexture = Window::LoadTexture(crackTexture);
    this->powerTexture = Window::LoadTexture(powerTexture);
    
    background = new Background();

    mapHeight = h;
    mapWidth = w;
    map = new int*[mapHeight];
    for (int i = 0; i < mapHeight; i++)
    {
        map[i] = new int[mapWidth];
    }

    Generate();
    Next();
}

Map::~Map()
{
    bricks.clear();

    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(brickTexture);
    SDL_DestroyTexture(crackTexture);
    SDL_DestroyTexture(powerTexture);
}

// MAP GENERATION
void Map::Generate()
{
    int currentWidth, spreading = 0;
    currentWidth = rand() % (mapWidth / 2) + 1;

    for (int y = 0; y < mapHeight; y++)
    {

        for (int x = (mapWidth / 2) - currentWidth; x < (mapWidth / 2) + currentWidth; x++)
        {
            map[y][x] = 1;
        }
        
        if (spreading > 0)
        {
            if (currentWidth == 1)
            {
                currentWidth = rand() % 2 + currentWidth;
            }
            else
            {
                currentWidth = rand() % 3 + currentWidth - 1;
            }

            spreading = -1;
        }

        spreading++;
    }
}

// Load next <step> rows of bricks on the screen
void Map::Next()
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
                bricks.push_back(Brick(x * 64, 768 - ((current - y) * 32)));
            }
        }
    }
}

void Map::Update(Ball* ball)
{
    background->Update();

    if (Game::started())
    {
        SDL_Rect ballRect = ball->getRect();

        for (auto it = powers.begin(); it != powers.end(); ++it)
        {
            SDL_Rect powerRect = it->getRect();

            if (SDL_HasIntersection(&powerRect, &ballRect))
            {
                it = powers.erase(it);
                printf("Hmm");
            }
        }

        for (auto it = bricks.begin(); it != bricks.end(); ++it)
        {
            SDL_Rect brickRect = it->getRect();

            if (SDL_HasIntersection(&brickRect, &ballRect))
            {
                ball->Rebound(&brickRect, 0, 0);
                if (it->Hit())
                {
                    it = bricks.erase(it);
                    powers.push_back(Power(&brickRect));
                }
            }
        }
    }
}

void Map::Render()
{
    background->Render(backgroundTexture);

    for (auto it = bricks.begin(); it != bricks.end(); ++it)
    {
        it->Render(brickTexture, crackTexture);
    }

    for (auto it = powers.begin(); it != powers.end(); ++it)
    {
        it->Render(powerTexture);
    }
}






Brick::Brick(int x, int y)
{
    int type = rand() % 64;
    brickRect.h = 16;
    brickRect.w = 32;
    brickRect.x = (type / 8) * brickRect.w;
    brickRect.y = (type % 8) * brickRect.h;

    crackRect.x = 0;
    crackRect.y = 0;
    crackRect.h = brickRect.h;
    crackRect.w = brickRect.w;

    windowRect.x = x;
    windowRect.y = y;
    windowRect.h = brickRect.h * Window::getMultiplierH();
    windowRect.w = brickRect.w * Window::getMultiplierW();

    health = 7;
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
    textureRect.h = 384;
    textureRect.w = 256;

    frames = 5;
    frameDelay = 100;
    
    windowRect.x = 0;
    windowRect.y = 0;
    windowRect.h = textureRect.h * Window::getMultiplierH();
    windowRect.w = textureRect.w * Window::getMultiplierW();
}

void Background::Render(SDL_Texture* texture)
{
    Window::Render(texture, &textureRect, &windowRect);
}

void Background::Update()
{
    textureRect.x = textureRect.w * ((SDL_GetTicks() / frameDelay) % frames);
}

