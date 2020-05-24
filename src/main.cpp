#include "../include/Game.hpp"

Game *game = NULL;

int main()
{
    game = new Game();
    game->init("ArkanoidNineHells", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 832, 832, false);

    while (game->running())
    {
        game->handleEvents();
        game->update();
        game->render();
    }

    game->clean();

    return 0;
}
