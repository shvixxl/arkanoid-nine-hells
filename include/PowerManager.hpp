#ifndef GAME
#define GAME
#include "Game.hpp"
#include <SDL2/SDL_timer.h>
#endif

enum Spells
{
    summon_sphere,
    displacement,
    haste,
    find_path,
    LAST = find_path
};


class Spell
{
    public:
        Spell(int spell);
        ~Spell();

        int getCount();
        void addCount(int n);

        void cast(int s);
        bool ready();

        void Update();
        void Render(SDL_Texture* cellTexture, SDL_Texture* spellsTexture);
        

    private:
        int count;

        Uint32 castTime;
        Uint32 endTime;

        SDL_Rect cellTextureRect;
        SDL_Rect spellsTextureRect;
        
        SDL_Rect cellWindowRect;
        SDL_Rect spellsWindowRect;
};


class PowerManager
{
    public:
        static void Init();
        static void Clean();

        static void GenerateSpell();
        static void addSpell(Spells spell, int n);

        static void Update();
        static void Render();
        static void HandleEvents();

        static void SpellDisplacement();
        static void SpellSummonSphere();
        static void SpellHaste();
        static void SpellFindPath();

    private:
        static SDL_Rect dockTextureRect;
        static SDL_Rect dockWindowRect;

        static Spell** spells;

        static SDL_Texture* dockTexture;
        static SDL_Texture* cellTexture;
        static SDL_Texture* cellSelectedTexture;
        static SDL_Texture* cellWaitTexture;
        static SDL_Texture* spellsTexture;
        static SDL_Texture* numbersTexture;
};

