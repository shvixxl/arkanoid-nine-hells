#ifndef GAME
#define GAME
#include "Game.hpp"
#endif

class Window
{
    public:
        static void Init(const char* title, int x, int y, int w, int h, int windowFlags);
        static void Clean();

        static SDL_Texture* LoadTexture(const char* filename);
        
        static void Render(SDL_Texture* texture, SDL_Rect* textureRect, SDL_Rect* windowRect);
        static void RednerClear();
        static void RenderPresent();

        static void CheckCollision();

        static int getWidth();
        static int getHeight();

        static bool isExist() { return window && renderer; }

        static SDL_Window* window;
        static SDL_Renderer* renderer;
};

