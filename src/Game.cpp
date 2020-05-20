#include "../include/Game.hpp"
#include "../include/Window.hpp"
#include "../include/Map.hpp"
#include "../include/Player.hpp"
#include "../include/Ball.hpp"

Map* map = nullptr;
Player* player = nullptr;
Ball* ball = nullptr;

SDL_Window* Window::window = nullptr;
SDL_Renderer* Window::renderer = nullptr;

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

        map = new Map("assets/Avernus.png", "assets/brick.png", "assets/crack.png", 25, 8);
        player = new Player("assets/skyship.png");
        ball = new Ball("assets/ball.png");
    }
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

    ball->Update();

    map->Update();
}

void Game::render()
{
    Window::RednerClear();

    map->Render();

    player->Render();

    ball->Render();

    Window::RenderPresent();
}

void Game::clean()
{
    delete ball;
    ball = nullptr;
    
    delete player;
    player = nullptr;

    delete map;
    map = nullptr;

    Window::Clean();

    IMG_Quit();

    SDL_Quit();
}
