#ifndef GAME
#define GAME
#include "Game.hpp"
#include "Timer.hpp"
#include "EntityManager.hpp"
#include "MapManager.hpp"
#include "SpellManager.hpp"
#include "Window.hpp"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>
#endif

enum Scenes
{
    main_menu,
    new_game,
    settings_menu,
    quit_game,
    level,
    SCENES_COUNT = level + 1,
    SCENES_NULL
};

class Button
{
    public:
        Button(const char* text, int x, int y);
        ~Button();
        
        void Update();
        void Render();

        void changeText(const char* text);

        bool Available() { return isAvailable; }
        void toggleAvailable();
        void toggleSelected();

    private:
        bool isAvailable;
        bool isSelected;

        SDL_Texture* texture;
        SDL_Texture* textureAvailable;
        SDL_Texture* textureSelected;

        SDL_Rect windowRect;
};

class Object
{
    public:
        Object(SDL_Texture* texture, int x, int y);
        ~Object();
        
        void Clean();
        void Render();

    private:
        SDL_Texture* texture;

        SDL_Rect windowRect;
};

class GameManager
{
    public:
        static bool Init();
        static void Clean();

        static int HandleEvents(SDL_Event* event);
        static void Update();
        static void Render();

        static void LoadScene(Scenes scene);
        
        static void Continue();

        static void LoadDefaults();
        static void LoadSave();

        static void changeSelectedButton(int newButton);
        static void clearMenu();
        
        static Uint32 UpdateBackground(Uint32 interval, void*);

    private:
        static Scenes currentScene;
        static Levels currentLevel;

        static Json::Value config;

        static Json::Value menuData;

        static Button** buttons;
        static int buttonsCount;
        static int selectedButton;

        static std::vector<Object> objects;
};

