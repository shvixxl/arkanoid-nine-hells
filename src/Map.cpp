#include "../include/Map.hpp"
#include "../include/Window.hpp"
#include <SDL2/SDL_render.h>

Map::Map(const char* backgroundTexture, const char* brickTexture, const char* crackTexture, int h, int w)
{
    this->backgroundTexture = Window::LoadTexture(backgroundTexture);
    this->brickTexture = Window::LoadTexture(brickTexture);
    this->crackTexture = Window::LoadTexture(crackTexture);
    
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
    SDL_DestroyTexture(crackTexture);
    SDL_DestroyTexture(crackTexture);
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

void Map::Update()
{
    background->Update();
}

void Map::Render()
{
    background->Render(backgroundTexture);

    for (auto it = bricks.begin(); it != bricks.end(); ++it)
    {
        it->Render(brickTexture, crackTexture);
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
    windowRect.h = brickRect.h * 2;
    windowRect.w = brickRect.w * 2;
}

Brick::~Brick()
{

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
    
    windowRect.x = 0;
    windowRect.y = 0;
    windowRect.h = 768;
    windowRect.w = 512;
}

void Background::Render(SDL_Texture* texture)
{
    Window::Render(texture, &textureRect, &windowRect);
}

void Background::Update()
{
    textureRect.x = textureRect.w * ((SDL_GetTicks() / frameDelay) % 5);
}

