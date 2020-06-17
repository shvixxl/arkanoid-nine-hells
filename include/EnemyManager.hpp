#ifndef GAME
#define GAME
#include "Game.hpp"
#include "Timer.hpp"
#include "EntityManager.hpp"
#endif

class Attack {

};

class Enemy {
    public:
        Enemy(Json::Value data);
        ~Enemy();

        void Clean();

        void Update();
        void Render();

    private:
        Timer* attack_timer;
        std::string attack;

        Timer* animation_timer;
        int frames;

        SDL_Texture* enemyTexture;
        SDL_Rect enemyTextureRect;
        SDL_Rect enemyRect;

        SDL_Texture* attackTexture;
        SDL_Rect attackTextureRect;
        SDL_Rect attackRect;
};

class EnemyManager {
    public:
        static void Init();

        static void AddEnemy(std::string type);

        static void Update();
        static void Render();

        static bool isEnemies();
        
    private:
        static std::vector<Enemy> enemies;
        static Json::Value data;
};

