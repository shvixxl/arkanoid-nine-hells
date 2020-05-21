#include "../include/Game.hpp"
#include "../include/Window.hpp"
#include "../include/Ball.hpp"
#include "../include/Power.hpp"
#include "../include/Map.hpp"
#include "../include/Player.hpp"

// Game objects
Map* map = nullptr;
Player* player = nullptr;
Ball* ball = nullptr;

// Static variables
bool Game::isStarted = false;

void Game::init(const char* title, int x, int y, int width, int height, bool fullscreen)
{
    bool success = true;

    // Initialize random
    srand(time(NULL));

    // "Create window in fullscreen or not" flag
    int windowFlags = fullscreen ? SDL_WINDOW_FULLSCREEN : SDL_WINDOW_SHOWN;
    
    // "Which image support initialize" flag
    int imageFlags = IMG_INIT_PNG;

    // Initialize SDL
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        printf("Failed to initialize SDL subsystems! SDL_Error: %s\n", SDL_GetError());
        success = false;
    }

    // Initialize SDL_Image
    if (!(IMG_Init(imageFlags) & imageFlags))
    {
        printf("Failed to initialize PNG support! IMG_Error: %s\n", IMG_GetError());
        success = false;
    }

    // Initializing window and renderer
    Window::Init(title, x, y, width, height, windowFlags);

    // Run the game
    if (!success && !Window::isExist())
    {
        isRunning = false;
    }
    else
    {
        isRunning = true;

        map = new Map("assets/Avernus.png", "assets/brick.png", "assets/crack.png", "assets/bonus.png",25, 8);
        player = new Player("assets/skyship.png");
    }
}

void Game::start()
{
    SDL_Rect tempRect = player->getRect();

    int x = tempRect.x + tempRect.w / 2 - 16;
    int y = tempRect.y + tempRect.h;

    ball = new Ball("assets/ball.png", x, y, player->getSpeed());

    isStarted = true;
}

void Game::stop()
{
    delete ball;
    ball = nullptr;

    isStarted = false;
}

void Game::handleEvents()
{
    // Event handler
    SDL_Event event;

    // Get event
    SDL_PollEvent(&event);

    // Handle event
    switch (event.type)
    {
        case SDL_QUIT:
            isRunning = false;
            break;

        default:
            player->HandleEvents(&event);
            break;
    }
}

void Game::update()
{
    player->Update();

    if (Game::started())
    {
        ball->Update(player->getSpeed(), player->getRect());
    }

    map->Update(ball);
}

void Game::render()
{
    Window::RednerClear();

    map->Render();

    player->Render();

    if (Game::started())
    {
        ball->Render();
    }

    Window::RenderPresent();
}

void Game::clean()
{
    delete player;
    player = nullptr;

    delete map;
    map = nullptr;

    Window::Clean();

    IMG_Quit();

    SDL_Quit();
}
