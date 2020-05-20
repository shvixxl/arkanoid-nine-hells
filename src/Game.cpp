#include "../include/Game.hpp"
#include "../include/TextureManager.hpp"
#include "../include/Player.hpp"
#include "../include/Map.hpp"

GameObject* player;
Map* map;
SDL_Renderer* Game::renderer = NULL;

Game::Game()
{

}

Game::~Game()
{

}

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
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

    // Creating window
    window = SDL_CreateWindow(title, xpos, ypos, width, height, windowFlags);
    if (window == NULL)
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        success = false;
    }

    // Creating renderer
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (renderer == NULL)
    {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
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

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        player = new GameObject("assets/skyship.png", 0, 0);
        map = new Map("assets/Avernus.png", "assets/brick.png", "assets/crack.png", 25, 8);
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
            break;
    }
}

void Game::update()
{
    player->Update();

    map->Update();
}

void Game::render()
{
    SDL_RenderClear(renderer);

    map->Render();

    player->Render();

    SDL_RenderPresent(renderer);
}

void Game::clean()
{
    SDL_DestroyRenderer(renderer);
    renderer = NULL;

    SDL_DestroyWindow(window);
    window = NULL;

    IMG_Quit();

    SDL_Quit();
}
