#ifndef GAME
#define GAME
#include "Game.hpp"
#include "Timer.hpp"
#endif


enum ShipMovementDirections
{
    MOVE_LEFT = 1,
    MOVE_RIGHT = 2
};


class Ship
{
    public:
        Ship(const char* type, int x, int y);
        ~Ship();

        void Update();
        void Render();

        void Move(int MOVE);

        void Slow();

        void Displace();
        void Haste();

        SDL_Rect getRect() { return paddleRect; }
        float getSpeed() { return speed; }

        std::string getType() { return type; }

    private:
        std::string type;
        Json::Value data;

        // To determine direction (MOVE_LEFT, MOVE_RIGHT)
        int move;

        float x;

        // Current speed
        float speed;

        // Value by which speed changes
        float speedBoost;
        float speedSlow;

        SDL_Texture* texture = nullptr;

        SDL_Rect textureRect;
        SDL_Rect windowRect;
        SDL_Rect paddleRect;

        // Spell effects
        bool isDisplaced;
        bool isHasted;

        Timer* animation_timer;
        int lastX;
        int frames;
        int frame;
};


class Sphere
{
    public:
        Sphere(const char* type, int x, int y, float speedX, float speedY);
        ~Sphere();

        void Clean();

        int Update();
        void Render();

        bool CheckCollision(SDL_Rect* objectRect);
        void Rebound(SDL_Rect* objectRect, float objectSpeedX, float objectSpeedY);

        void FindPath();

        SDL_Rect getRect() { return windowRect; }
        
        std::string getType() { return type; }

    private:
        std::string type;
        Json::Value data;

        int movement_type;
        float max_speed;

        float x;
        float y;

        float speedY;
        float speedX;

        Timer* animation_timer;
        int frames;
        int frame;

        Timer* find_path_delay;
        SDL_Rect** find_path_dots;

        SDL_Texture* texture;

        SDL_Rect textureRect;
        SDL_Rect windowRect;

};

class EntityManager
{
    public:
        static void Init(const char* ship, const char* sphere);
        static void Clean();

        // Functions for ships
        static void addShip();

        static void UpdateShip();
        static void RenderShip();
        
        static void ShipHandleEvents(SDL_Event* event);
        static bool ShipCheckCollision(SDL_Rect* objectRect);


        // Functions for spheres
        static void throwSphere();
        static void addSphere(const char* type, int x, int y, float speedX, float speedY);
        
        static void UpdateSpheres();
        static void RenderSpheres();
        
        static std::vector<size_t> SpheresCheckCollision(SDL_Rect* objectRect);
        static void SpheresRebound(size_t id, SDL_Rect* objectRect, float objectSpeedX, float objectSpeedY);

        static void getNumberSpheres();

    private:
        static std::string ship_type;
        static std::string sphere_type;

        static std::vector<Sphere> spheres;
        static Ship* ship;
};

