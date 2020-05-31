#ifndef GAME
#define GAME
#include "Game.hpp"
#include "EntityManager.hpp"
#include "MapManager.hpp"
#include "GameManager.hpp"
#endif

enum Spells
{
    summon_sphere,
    displacement,
    haste,
    find_path,
    SPELLS_COUNT = find_path+1
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
        void Render(SDL_Texture* cellTexture, SDL_Texture* spellsTexture, SDL_Texture* numbersTexture);
        
        void TogglePressed();

    private:
        int count;
        bool isPressed;

        Uint32 castTime;
        Uint32 endTime;

        SDL_Rect cellTextureRect;
        SDL_Rect spellsTextureRect;
        SDL_Rect firstNumberTextureRect;
        SDL_Rect secondNumberTextureRect;
        
        SDL_Rect cellWindowRect;
        SDL_Rect spellsWindowRect;
        SDL_Rect firstNumberWindowRect;
        SDL_Rect secondNumberWindowRect;
};


class SpellManager
{
    public:
        static void Init();
        static void Clean();

        static void GenerateSpell();
        static void addSpell(Spells spell, int n);

        static void Update();
        static void Render();
        static void HandleEvents(SDL_Event* event);

        static void SpellSummonSphere(Spheres type);
        static void SpellDisplacement();
        static void SpellHaste();
        static void SpellFindPath();

    private:
        static Spell** spells;

        static SDL_Texture* cellTexture;
        static SDL_Texture* spellsTexture;
        static SDL_Texture* numbersTexture;
};

