#include "../include/Game.hpp"
#include "../include/Window.hpp"
#include "../include/EntityManager.hpp"
#include "../include/MapManager.hpp"
#include "../include/SpellManager.hpp"
#include "../include/GameManager.hpp"

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

    // Inititalize SDL_ttf
    if (TTF_Init() < 0)
    {
        printf("Failed to initialize TTF support! TTF_Error: %s\n", TTF_GetError());
        success = false;
    }

    // Initializing window and renderer
    if (!Window::Init(title, x, y, width, height, windowFlags))
    {
        success = false;
    }

    // Initialize GameManager
    if (!GameManager::Init())
    {
        success = false;
    }

    // Run the game
    if (!success)
    {
        isRunning = false;
    }
    else
    {
        isRunning = true;
        GameManager::LoadScene(main_menu);
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
            if (GameManager::HandleEvents(&event) == -1)
            {
                isRunning = false;
            }
            break;
    }
}

void Game::update()
{
    Window::Update();

    GameManager::Update();
}

void Game::render()
{
    Window::RednerClear();

    GameManager::Render();

    Window::RenderPresent();
}

void Game::clean()
{
    GameManager::Clean();

    Window::Clean();

    TTF_Quit();

    IMG_Quit();

    SDL_Quit();
}
