#include "../include/Game.hpp"
#include "../include/TextureManager.hpp"
#include "../include/GameObject.hpp"

GameObject* player;

Game::Game()
{

}

Game::~Game()
{

}

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{   
    bool success = true;

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
        
        player = new GameObject("assets/skyship.png", renderer, 0, 0);
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
}

void Game::render()
{
    SDL_RenderClear(renderer);

    player->Render();

    SDL_RenderPresent(renderer);
}

void Game::clean()
{
    // Destroy window
    SDL_DestroyWindow(window);
    window = NULL;

    // Destroy renderer
    SDL_DestroyRenderer(renderer);
    renderer = NULL;

    // Quit SDL subsystems
    SDL_Quit();
}
