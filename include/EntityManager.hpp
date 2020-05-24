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

        static void UpdateShips();
        static void RenderShips();
        
        static void ShipsHandleEvents(SDL_Event* event);

        static void getNumberShips();
       

        // Functions for spheres
        static void addSphere(Spheres type, int x, int y, float paddle_speed);
        
        static void UpdateSpheres();
        static void RenderSpheres();
        
        static std::vector<size_t> SpheresCheckCollision(SDL_Rect* objectRect);
        static void SpheresRebound(size_t id, SDL_Rect* objectRect, float objectSpeedX, float objectSpeedY);

        static void getNumberSpheres();

    private:
        static std::vector<Sphere> spheres;
        static std::vector<Ship> ships;
};

