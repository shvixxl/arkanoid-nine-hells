#ifndef GAME
#define GAME
#include "Game.hpp"
#include "Ball.hpp"
#endif


class Brick
{
    public:
        Brick(int x, int y);
        ~Brick();

        bool Hit();
        void Render(SDL_Texture* brickTexture, SDL_Texture* crackTexture);
        
        SDL_Rect getRect() { return windowRect; }
    private:
        int health;

        SDL_Rect brickRect;
        SDL_Rect crackRect;

        SDL_Rect windowRect;
};


class Background
{
    public:
        Background();
        ~Background();

        void Render(SDL_Texture* texture);
        void Update();

    private:
        int frameDelay = 100;

        SDL_Rect textureRect;
        SDL_Rect windowRect;
};


class Map
{
    public:
        Map(const char* background, const char* brickTexture, const char* crackTexture, int h, int w);
        ~Map();

        void Generate();
        void Next();
        void Update(Ball* ball);
        void Render();

    private:
        int mapHeight;
        int mapWidth;
        int** map;

        int step = 5;
        int current = 0;
        
        SDL_Texture* brickTexture;
        SDL_Texture* crackTexture;
        SDL_Texture* backgroundTexture;

        std::vector<Brick> bricks;
        Background* background = NULL;
};

