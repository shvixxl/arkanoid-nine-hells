#include "../include/Game.hpp"
#include "../include/Window.hpp"
#include "../include/Sphere.hpp"
#include "../include/PowerManager.hpp"
#include "../include/MapManager.hpp"
#include "../include/Ship.hpp"
#include "../include/EntityManager.hpp"

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
        MapManager::Init(avernus, 80, 12);
        PowerManager::Init();
        EntityManager::addShip(skyship);
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
            EntityManager::ShipHandleEvents(&event);
            break;
    }
}

void Game::update()
{
    EntityManager::UpdateSpheres();
    
    EntityManager::UpdateShip();

    MapManager::UpdateBackground();

    MapManager::UpdatePowers();
    
    MapManager::UpdateBricks();

    PowerManager::Update();
}

void Game::render()
{
    Window::RednerClear();

    MapManager::Render();
    
    EntityManager::RenderShip();

    EntityManager::RenderSpheres();

    PowerManager::Render();

    Window::RenderPresent();
}

void Game::clean()
{
    EntityManager::Clean();

    MapManager::Clean();

    Window::Clean();

    IMG_Quit();

    SDL_Quit();
}
