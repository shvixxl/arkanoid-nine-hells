#include "../include/PowerManager.hpp"
#include "../include/Window.hpp"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>

SDL_Rect PowerManager::dockTextureRect;
SDL_Rect PowerManager::dockWindowRect;

Spell** PowerManager::spells = nullptr;

SDL_Texture* PowerManager::dockTexture = nullptr;
SDL_Texture* PowerManager::cellTexture = nullptr;
SDL_Texture* PowerManager::cellSelectedTexture = nullptr;
SDL_Texture* PowerManager::cellWaitTexture = nullptr;
SDL_Texture* PowerManager::spellsTexture = nullptr;
SDL_Texture* PowerManager::numbersTexture = nullptr;

void PowerManager::Init()
{
    dockTextureRect.x = 0;
    dockTextureRect.y = 0;
    dockTextureRect.w = 64;
    dockTextureRect.h = 256;

    dockWindowRect.x = Window::getWidth() - dockTextureRect.w * Window::getMultiplierW();
    dockWindowRect.y = 0;
    dockWindowRect.w = dockTextureRect.w * Window::getMultiplierW();
    dockWindowRect.h = dockTextureRect.h * Window::getMultiplierH();

    dockTexture = Window::LoadTexture("assets/dock.png");
    cellTexture = Window::LoadTexture("assets/spell_cell.png");
    cellSelectedTexture = Window::LoadTexture("assets/spell_cell_selected.png");
    cellWaitTexture = Window::LoadTexture("assets/spell_cell_wait.png");
    spellsTexture = Window::LoadTexture("assets/spells.png");

    spells = new Spell*[LAST + 1];
    for (int spell = 0; spell <= LAST; ++spell)
    {
        spells[spell] = new Spell(spell);
    }
}

void PowerManager::Clean()
{
    for (int spell = 0; spell <= LAST; ++spell)
    {
        delete spells[spell];
        spells[spell] = nullptr;
    }

    delete[] spells;
}

void PowerManager::GenerateSpell()
{
    Spells spell;
    int random = rand() % 100;

    if (random < 25)
    {
        spell = summon_sphere;
    }
    else if (random < 50)
    {
        spell = displacement;
    }
    else if (random < 75)
    {
        spell = haste;
    }
    else if (random < 100)
    {
        spell = find_path;   
    }
    
    PowerManager::addSpell(spell, 1);
}

void PowerManager::addSpell(Spells spell, int n)
{
    spells[spell]->addCount(n);
}

void PowerManager::Update()
{
    for (int spell = 0; spell <= LAST; ++spell)
    {
        spells[spell]->Update();
    }
}

void PowerManager::Render()
{
    Window::Render(dockTexture, &dockTextureRect, &dockWindowRect);

    for (int spell = 0; spell <= LAST; ++spell)
    {
        if (spells[spell]->ready())
        {
            spells[spell]->Render(cellTexture, spellsTexture);
        }
        else
        {
            spells[spell]->Render(cellWaitTexture, spellsTexture);
        }
    }
}



Spell::Spell(int spell)
{
    cellTextureRect.h = 32;
    cellTextureRect.w = 32;
    cellTextureRect.x = 0;
    cellTextureRect.y = 0;

    spellsTextureRect.h = 24;
    spellsTextureRect.w = 24;
    spellsTextureRect.x = spellsTextureRect.w * spell;
    spellsTextureRect.y = 0;

    cellWindowRect.h = cellTextureRect.h * Window::getMultiplierH();
    cellWindowRect.w = cellTextureRect.w * Window::getMultiplierW();
    cellWindowRect.x = (256 + 17) * Window::getMultiplierW();
    cellWindowRect.y = (17) * Window::getMultiplierH();

    spellsWindowRect.h = spellsTextureRect.h * Window::getMultiplierH();
    spellsWindowRect.w = spellsTextureRect.w * Window::getMultiplierW();
    spellsWindowRect.x = cellWindowRect.x + 4 * Window::getMultiplierW();
    spellsWindowRect.y = cellWindowRect.y + 4 * Window::getMultiplierH();
   
    count = 0;

    castTime = 0;
    endTime = 0;
}

Spell::~Spell()
{

}

int Spell::getCount()
{
    return count;
}

void Spell::addCount(int n)
{
    count += n;
}

void Spell::cast(int s)
{
    castTime = SDL_GetTicks();
    endTime = castTime + s * 1000;
}

bool Spell::ready()
{
    return endTime - SDL_GetTicks() <= 0;
}

void Spell::Update()
{
    if (!ready())
    {
        cellTextureRect.x = cellTextureRect.w * ((endTime - SDL_GetTicks()) / (SDL_GetTicks() - castTime) / 5);
    }
    else
    {
        cellTextureRect.x = 0;
    }
}

void Spell::Render(SDL_Texture* cellTexture, SDL_Texture* spellsTexture)
{
    Window::Render(cellTexture, &cellTextureRect, &cellWindowRect);
    Window::Render(spellsTexture, &spellsTextureRect, &spellsWindowRect);
}

