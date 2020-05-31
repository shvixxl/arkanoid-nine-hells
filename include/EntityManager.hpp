#ifndef GAME
#define GAME
#include "Game.hpp"
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

        void Power();

        SDL_Rect getRect() { return windowRect; }
        float getSpeed() { return speed; }

    private:
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

