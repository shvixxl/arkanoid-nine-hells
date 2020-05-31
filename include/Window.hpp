#ifndef GAME
#define GAME
#include "Game.hpp"
#include <SDL2/SDL_ttf.h>
#endif

class Window
{
    public:
        static bool Init(const char* title, int x, int y, int w, int h, int windowFlags);
        static void Clean();

        static SDL_Texture* LoadTexture(const char* filename);
        static SDL_Texture* LoadText(const char* filename, const char* text, int size, SDL_Color color);

        static void Update();

        static void Render(SDL_Texture* texture, SDL_Rect* textureRect, SDL_Rect* windowRect);
        static void RednerClear();
        static void RenderPresent();

        static void CheckCollision();

        static int getWidth();
        static int getHeight();

        static int getRealWidth();
        static int getRealHeight();

        static float getTopPaddingH();

    private:
        static SDL_Window* window;
        static SDL_Renderer* renderer;
};

