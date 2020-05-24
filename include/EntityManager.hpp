#ifndef GAME
#define GAME
#include "Game.hpp"
#include "Sphere.hpp"
#include "Ship.hpp"
#endif

class EntityManager
{
    public:
        static void Clean();

        // Functions for ships
        static void addShip(Ships type);

        static void UpdateShip();
        static void RenderShip();
        
        static void ShipHandleEvents(SDL_Event* event);


        // Functions for spheres
        static void throwSphere(Sphere type);
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

