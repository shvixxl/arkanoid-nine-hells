#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

class Game
{
    public:
        Game();
        ~Game();

        void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);

        void handleEvents();
        void update();
        void render();
        void clean();

        bool running() { return isRunning; }

    private:
        bool isRunning = false;

        int count = 0;

        SDL_Window* window = NULL;
        SDL_Renderer* renderer = NULL;


};
