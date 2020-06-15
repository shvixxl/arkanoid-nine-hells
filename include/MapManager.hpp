#ifndef GAME
#define GAME
#include "Game.hpp"
#include "Timer.hpp"
#include "EntityManager.hpp"
#include "SpellManager.hpp"
#endif

class Brick
{
    public:
        Brick(int x, int y);
        ~Brick();

        bool Hit();

        void Update();
        void Render(SDL_Texture* brickTexture, SDL_Texture* crackTexture);

        void TranstionStep();

        SDL_Rect getRect() { return windowRect; }
        int getHealth() { return health; };

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

        void TransitionStep();

        SDL_Rect getRect() { return windowRect; }

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

        void TranstionStep();

        SDL_Rect getRect() { return windowRect; }

    private:
        int speed;

        Timer* animation_timer;
        int frames;
        int frame;

        SDL_Rect textureRect;
        SDL_Rect windowRect;
};

class MapManager
{
    public:
        static void Init(const char* type);

        static void Clean();

        static void Generate(int spire);
        static void Next();

        static void Update();
        static void UpdateBackground();
        static void UpdateSouls();
        static void UpdateBricks();
        
        static void HitBricks(size_t brick);
        static void TakeSouls(size_t soul);

        static void Render();

        static bool isTransition() { return currentTransitionStep > 0; }
        static void TransitionStep();

        static std::vector<SDL_Rect> GetBricksRect();
        static std::vector<SDL_Rect> GetSoulsRect();

        static bool isBricks();
        static bool isSouls();

    private:
        static std::string mapType;

        static int mapHeight;
        static int mapWidth;
        static int** map;

        static int step;
        static int current;

        static int transitionStep;
        static int currentTransitionStep;

        static SDL_Texture* brickTexture;
        static SDL_Texture* crackTexture;
        static SDL_Texture* backgroundTexture;
        static SDL_Texture* powerTexture;

        static std::vector<Brick> bricks;
        static std::vector<Power> powers;
        static Background* background;

        static Json::Value data;

        static Uint32 startTime;
};
