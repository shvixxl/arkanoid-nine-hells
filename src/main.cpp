#include "../include/Game.hpp"
#include <SDL2/SDL_timer.h>

Game *game = NULL;

int main()
{
    game = new Game();
    game->init("ArkanoidNineHells", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 512, 768, false);

    while (game->running())
    {
        game->handleEvents();
        game->update();
        game->render();
    }

    game->clean();

    return 0;
}
