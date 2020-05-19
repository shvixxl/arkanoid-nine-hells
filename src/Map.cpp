#include "../include/Map.hpp"
#include "../include/TextureManager.hpp"

Map::Map(const char* backgroundTexture, const char* brickTexture, const char* crackTexture, int h, int w)
{
    this->backgroundTexture = TextureManager::LoadTexture(backgroundTexture);
    this->brickTexture = TextureManager::LoadTexture(brickTexture);
    this->brickTexture = TextureManager::LoadTexture(crackTexture);
    
    background = new Background();

    mapHeight = h;
    mapWidth = w;
    map = new int*[h];
    for (int i = 0; i < h; i++)
    {
        map[i] = new int[w];
    }
    
    Generate();
    Next();
}


// MAP GENERATION
void Map::Generate()
{
    int currentWidth;

    for (int y = 0; y < mapHeight; y++)
    {
        currentWidth = rand() % (mapWidth / 2) + 1;

        for (int x = (mapWidth / 2) - currentWidth; x < (mapWidth / 2) + currentWidth; x++)
        {
            map[y][x] = 1;
        }
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
            if (map[y][x])
            {
                bricks.push_back(Brick(x * 32, 768 - ((step - y) * 16)));
            }
        }
    }
}

void Map::Update()
{

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
    brickRect.x = (rand() % 8) * 32;
    brickRect.y = (rand() % 8) * 16;
    brickRect.h = 16;
    brickRect.w = 32;

    crackRect.x = 0;
    crackRect.y = 0;
    crackRect.h = brickRect.h;
    crackRect.w = brickRect.w;

    windowRect.x = x * 2;
    windowRect.y = y * 2;
    windowRect.h = brickRect.h * 2;
    windowRect.w = brickRect.h * 2;
}

void Brick::Render(SDL_Texture* brickTexture, SDL_Texture* crackTexture)
{
    SDL_RenderCopy(Game::renderer, brickTexture, &brickRect, &windowRect);
    SDL_RenderCopy(Game::renderer, crackTexture, &crackRect, &windowRect);
}




Background::Background()
{
    textureRect.x = 0;
    textureRect.y = 0;
    textureRect.h = 640;
    textureRect.w = 480;
    
    windowRect.x = 0;
    windowRect.y = 0;
    windowRect.h = 768;
    windowRect.w = 512;
}

void Background::Render(SDL_Texture* texture)
{
    SDL_RenderCopy(Game::renderer, texture, &textureRect, &windowRect);
}


