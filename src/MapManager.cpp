#include "../include/MapManager.hpp"
#include "../include/Window.hpp"
#include "../include/GameManager.hpp"

std::string MapManager::mapType;

int MapManager::mapHeight;
int MapManager::mapWidth;

int** MapManager::map = nullptr;

int MapManager::step;
int MapManager::current;

int MapManager::transitionStep;
int MapManager::currentTransitionStep;

SDL_Texture* MapManager::backgroundTexture = nullptr;
SDL_Texture* MapManager::brickTexture = nullptr;
SDL_Texture* MapManager::crackTexture = nullptr;
SDL_Texture* MapManager::powerTexture = nullptr;

Background* MapManager::background = nullptr;
std::vector<Brick> MapManager::bricks;
std::vector<Power> MapManager::powers;

Json::Value MapManager::data;

Uint32 MapManager::startTime;


void MapManager::Init(const char* type)
{
    mapType = type;

    std::ifstream file("data/levels.json");
    if (!file.is_open())
    {
        printf("Some data files is missing!");
    }
    file >> data;
    file.close();

    data = data[type];

    backgroundTexture = Window::LoadTexture(data["background"].asString().c_str());
    brickTexture = Window::LoadTexture(data["bricks"].asString().c_str());

    crackTexture = Window::LoadTexture("assets/crack.png");
    powerTexture = Window::LoadTexture("assets/soul.png");

    background = new Background();

    mapHeight = data["height"].asInt();
    mapWidth = data["width"].asInt();

    map = new int*[mapHeight];
    for (int i = 0; i < mapHeight; i++)
    {
        map[i] = new int[mapWidth];
    }

    step = data["step"].asInt();
    current = 0;

    transitionStep = step * 4;
    currentTransitionStep = 0;

    Generate(2);
    Next();

    startTime = SDL_GetTicks();
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
    int growthStep = mapHeight / mapWidth * 2;
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


        if (growth >= growthStep)
        {
            growth = 0;

            if (firstX > 0)
            {
                firstX--;
            }
            if (lastX < mapWidth - 1)
            {
                lastX++;
            }
        }

        growth++;
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
    MapManager::TransitionStep();

    MapManager::UpdateBackground();
    MapManager::UpdateSouls();
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

        if (bricks.at(i).getHealth() <= 0)
        {
            SDL_Rect brickRect = bricks.at(i).getRect();

            float score = 100 * data["score_multiplier"].asFloat();

            if ((float) (SDL_GetTicks() - startTime) / 60000 >= 10)
            {
                score *= 0.1;
            }
            else
            {
                score *= 1.0 - ((float) (SDL_GetTicks() - startTime) / 60000) * 0.1;
            }

            GameManager::addScore(score);

            bricks.erase(bricks.begin() + i);
            i -= 1;

            // Add power after the destruction of the brick
            powers.push_back(Power(&brickRect));
        }
    }
}

void MapManager::HitBricks(size_t brick)
{
    bricks.at(brick).Hit();
}

void MapManager::UpdateSouls()
{
    for (size_t i = 0; i < powers.size(); ++i)
    {
        powers.at(i).Update();

        SDL_Rect soulRect = powers.at(i).getRect();
        
        if (soulRect.y + soulRect.h < 0)
        {
            powers.erase(powers.begin() + i);
            i -= 1;
        }
    }
}

void MapManager::TakeSouls(size_t soul)
{
        SpellManager::GenerateSpell();
        powers.erase(powers.begin() + soul);
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

void MapManager::TransitionStep()
{
    if (currentTransitionStep > 0)
    {
        if (currentTransitionStep % 4 == 0)
        {
            background->TransitionStep();
        }

        for (size_t i = 0; i < bricks.size(); ++i)
        {
            bricks.at(i).TranstionStep();
        }

        for (size_t i = 0; i < powers.size(); ++i)
        {
            powers.at(i).TranstionStep();
        }

        currentTransitionStep -= 1;
    }
}

std::vector<SDL_Rect> MapManager::GetBricksRect()
{
    std::vector<SDL_Rect> bricksRect;

    for (size_t i = 0; i < bricks.size(); ++i)
    {
        bricksRect.push_back(bricks.at(i).getRect());
    }

    return bricksRect;
}

std::vector<SDL_Rect> MapManager::GetSoulsRect()
{
    std::vector<SDL_Rect> soulsRect;

    for (size_t i = 0; i < powers.size(); ++i)
    {
        soulsRect.push_back(powers.at(i).getRect());
    }

    return soulsRect;
}

bool MapManager::isBricks()
{
    return !bricks.empty() && current < mapHeight;
}

bool MapManager::isSouls()
{
    return !powers.empty();
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
    windowRect.y -= 2;
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

    animation_timer = new Timer(100);
    frames = 4;
    frame = rand() % frames;

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
    windowRect.y -= 2;
}

void Power::Render(SDL_Texture* texture)
{
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

void Power::TranstionStep()
{
    windowRect.y -= 6;
}

