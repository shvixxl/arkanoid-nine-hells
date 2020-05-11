#include "../include/Game.hpp"

Game::Game()
{

}

Game::~Game()
{

}

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{
    // "Create window in fullscreen or not" flag
    int flags = fullscreen ? SDL_WINDOW_FULLSCREEN : SDL_WINDOW_SHOWN;

    // Initialize SDL
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        printf("Failed to initialize SDL subsystems! SDL_Error: %s\n", SDL_GetError());
    }
    else
    {
        printf("SDL subsystems initialized!...\n");

        // Creating window
        window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
        if (window == NULL)
        {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        }
        else
        {
            printf("Window created!...\n");

            // Creating renderer
            renderer = SDL_CreateRenderer(window, -1, 0);
            if (renderer == NULL)
            {
                printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
            }
            else
            {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

                printf("Renderer created!...\n");

                printf("Game is running!...\n");

                isRunning = true;
            }
        }

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

}

void Game::render()
{
    SDL_RenderClear(renderer);
    // TODO render
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

    printf("Game cleaned!\n");
}
