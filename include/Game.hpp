#define GAME

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_events.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>

class Game
{
    public:
        void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);

        void handleEvents();
        void update();
        void render();
        void clean();

        bool running() { return isRunning; }

    private:
        bool isRunning = false;
};

