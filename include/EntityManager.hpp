#ifndef GAME
#define GAME
#include "Game.hpp"
#include "Timer.hpp"
#endif



enum Ships
{
    skyship,
    SHIPS_COUNT = skyship + 1
};

enum ShipMovementDirections
{
    MOVE_LEFT = 1,
    MOVE_RIGHT = 2
};

class Ship
{
    public:
        Ship(Ships type);
        ~Ship();

        void Update();
        void Render();

        void Move(int MOVE);

        void Slow();

        void Displace();
        void Haste();

        SDL_Rect getRect() { return paddleRect; }
        float getSpeed() { return speed; }

    private:
        Json::Value data;
        Ships type;

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




enum Spheres
{
    driftglobe,
    SPHERES_COUNT = driftglobe + 1
};

class Sphere
{
    public:
        Sphere(Spheres type, int x, int y, float speedX, float speedY);
        ~Sphere();

        void Clean();

        int Update();
        void Render();

        bool CheckCollision(SDL_Rect* objectRect);
        void Rebound(SDL_Rect* objectRect, float objectSpeedX, float objectSpeedY);

        void FindPath();

        SDL_Rect getRect() { return windowRect; }

    private:
        Json::Value data;
        Spheres type;
       
        int movement_type;
        float max_speed;

        float x;
        float y;

        float speedY;
        float speedX;

        Timer* animation_timer;
        int frames;
        int frame;

        SDL_Texture* texture;

        SDL_Rect textureRect;
        SDL_Rect windowRect;

};



class EntityManager
{
    public:
        static void Clean();

        // Functions for ships
        static void addShip(Ships type);

        static void UpdateShip();
        static void RenderShip();
        
        static void ShipHandleEvents(SDL_Event* event);
        static bool ShipCheckCollision(SDL_Rect* objectRect);


        // Functions for spheres
        static void throwSphere(Spheres type);
        static void addSphere(Spheres type, int x, int y, float speedX, float speedY);
        
        static void UpdateSpheres();
        static void RenderSpheres();
        
        static std::vector<size_t> SpheresCheckCollision(SDL_Rect* objectRect);
        static void SpheresRebound(size_t id, SDL_Rect* objectRect, float objectSpeedX, float objectSpeedY);

        static void getNumberSpheres();

    private:
        static std::vector<Sphere> spheres;
        static Ship* ship;
};

