#ifndef GAME
#define GAME
#include "Game.hpp"
#include "Ship.hpp"
#include "Sphere.hpp"
#include "PowerManager.hpp"
#include "EntityManager.hpp"
#endif

enum Maps
{
    avernus,
    MAPS_COUNT = avernus+1
};

class Brick
{
    public:
        Brick(int x, int y, int h);
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
        int frames;
        int frameDelay;
        
        SDL_Rect textureRect;
        SDL_Rect windowRect;
};

class Power
{
    public:
        Power(SDL_Rect* objectRect);
        ~Power();

        void Render(SDL_Texture* texture);
        void Update();

        SDL_Rect getRect() { return windowRect; }

    private:
        int frames;
        int frameDelay;
        
        SDL_Rect textureRect;
        SDL_Rect windowRect;
};

class MapManager
{
    public:
        static void Init(Maps type, int h, int w);

        static void Clean();

        static void Generate(int spire);
        static void Next();

        static void Update();
        static void UpdateBackground();
        static void UpdateBricks();
        static void UpdatePowers();

        static void Render();

    private:
        static int mapHeight;
        static int mapWidth;
        static int** map;

        static int step;
        static int current;
        
        static SDL_Texture* brickTexture;
        static SDL_Texture* crackTexture;
        static SDL_Texture* backgroundTexture;
        static SDL_Texture* powerTexture;

        static std::vector<Brick> bricks;
        static std::vector<Power> powers;
        static Background* background;
};
