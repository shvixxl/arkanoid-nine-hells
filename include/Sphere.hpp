#ifndef GAME
#define GAME
#include "Game.hpp"
#endif

enum Spheres
{
    annihilation,
    driftglobe,
    SPHERES_COUNT = driftglobe + 1
};

class Sphere
{
    public:
        Sphere(Spheres type, int x, int y, float speedX, float speedY);
        ~Sphere();

        void Update();
        void Render();

        bool CheckCollision(SDL_Rect* objectRect);
        void Rebound(SDL_Rect* objectRect, float objectSpeedX, float objectSpeedY);

        SDL_Rect getRect() { return windowRect; }

    private:
        float r;

        float x;
        float y;

        float speedY;
        float speedX;

        float maxSpeed;
        float reduceSpeed;

        int frames;
        int frameDelay;

        SDL_Texture* texture;

        SDL_Rect textureRect;
        SDL_Rect windowRect;

};

