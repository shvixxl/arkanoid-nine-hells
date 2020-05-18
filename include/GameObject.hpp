#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

class GameObject
{
    public:
        GameObject(const char* texture, SDL_Renderer* renderer, int x, int y);
        ~GameObject();
        
        void Update();
        void Render();

    private:
        int x;
        int y;

        SDL_Texture* texture;
        SDL_Rect srcRect, destRect;
        SDL_Renderer* renderer;

};

