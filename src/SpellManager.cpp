#include "../include/SpellManager.hpp"
#include "../include/Window.hpp"


Spell** SpellManager::spells = nullptr;

SDL_Texture* SpellManager::cellTexture = nullptr;
SDL_Texture* SpellManager::spellsTexture = nullptr;
SDL_Texture* SpellManager::numbersTexture = nullptr;

void SpellManager::Init()
{
    cellTexture = Window::LoadTexture("assets/spell_cell.png");
    spellsTexture = Window::LoadTexture("assets/spells.png");
    numbersTexture = Window::LoadTexture("assets/numbers.png");

    spells = new Spell*[SPELLS_COUNT];
    for (int spell = 0; spell < SPELLS_COUNT; ++spell)
    {
        spells[spell] = new Spell(spell);
    }
}

void SpellManager::Clean()
{
    if (spells)
    {
        for (int spell = 0; spell < SPELLS_COUNT; ++spell)
        {
            delete spells[spell];
            spells[spell] = nullptr;
        }

        delete[] spells;
        spells = nullptr;
    }
}

void SpellManager::GenerateSpell()
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
    
    SpellManager::addSpell(spell, 1);
}

void SpellManager::addSpell(Spells spell, int n)
{
    spells[spell]->addCount(n);
}

void SpellManager::Update()
{
    for (int spell = 0; spell < SPELLS_COUNT; ++spell)
    {
        spells[spell]->Update();
    }
}

void SpellManager::Render()
{
    for (int spell = 0; spell < SPELLS_COUNT; ++spell)
    {
        spells[spell]->Render(cellTexture, spellsTexture, numbersTexture);
    }
}

void SpellManager::HandleEvents(SDL_Event* event)
{
    if (event->type == SDL_KEYDOWN && event->key.repeat == 0)
    {
        switch (event->key.keysym.sym)
        {
            case SDLK_SPACE:
                spells[summon_sphere]->TogglePressed();
                break;
            case SDLK_1:
                spells[summon_sphere]->TogglePressed();
                break;
            case SDLK_2:
                spells[displacement]->TogglePressed();
                break;
            case SDLK_3:
                spells[haste]->TogglePressed();
                break;
            case SDLK_4:
                spells[find_path]->TogglePressed();
                break;
            default:
                break;
        }
    }

    if (event->type == SDL_KEYUP && event->key.repeat == 0)
    {
        switch (event->key.keysym.sym)
        {
            case SDLK_SPACE:
                spells[summon_sphere]->TogglePressed();
                SpellSummonSphere(driftglobe);
                break;
            case SDLK_1:
                spells[summon_sphere]->TogglePressed();
                SpellSummonSphere(driftglobe);
                break;
            case SDLK_2:
                spells[displacement]->TogglePressed();
                break;
            case SDLK_3:
                spells[haste]->TogglePressed();
                break;
            case SDLK_4:
                spells[find_path]->TogglePressed();
                break;
            default:
                break;
        }
    }
}

// Spells
void SpellManager::SpellSummonSphere(Spheres type)
{
    if (spells[summon_sphere]->ready() && spells[summon_sphere]->getCount() > 0)
    {
        EntityManager::throwSphere(type);
        
        spells[summon_sphere]->cast(5);
    }
}






Spell::Spell(int spell)
{
    cellTextureRect.h = 32;
    cellTextureRect.w = 32;
    cellTextureRect.x = 0;
    cellTextureRect.y = 0;

    spellsTextureRect.h = 32;
    spellsTextureRect.w = 32;
    spellsTextureRect.x = spellsTextureRect.w * spell;
    spellsTextureRect.y = 0;

    firstNumberTextureRect.h = 6;
    firstNumberTextureRect.w = 4;
    firstNumberTextureRect.x = 0;
    firstNumberTextureRect.y = 0;

    secondNumberTextureRect.h = 6;
    secondNumberTextureRect.w = 4;
    secondNumberTextureRect.x = 0;
    secondNumberTextureRect.y = 0;
    
    cellWindowRect.h = cellTextureRect.h;
    cellWindowRect.w = cellTextureRect.w;

    float padding = (Window::getWidth() - cellWindowRect.w * (SPELLS_COUNT)) / (SPELLS_COUNT + 1);

    cellWindowRect.x = padding + (padding + cellWindowRect.w) * spell;
    cellWindowRect.y = (Window::getTopPaddingH() - cellWindowRect.h) / 2;

    spellsWindowRect.h = spellsTextureRect.h;
    spellsWindowRect.w = spellsTextureRect.w;
    spellsWindowRect.x = cellWindowRect.x;
    spellsWindowRect.y = cellWindowRect.y;

    firstNumberWindowRect.h = firstNumberTextureRect.h;
    firstNumberWindowRect.w = firstNumberTextureRect.w;
    firstNumberWindowRect.x = cellWindowRect.x + 5;
    firstNumberWindowRect.y = cellWindowRect.y + 22;

    secondNumberWindowRect.h = secondNumberTextureRect.h;
    secondNumberWindowRect.w = secondNumberTextureRect.w;
    secondNumberWindowRect.x = cellWindowRect.x + 7;
    secondNumberWindowRect.y = cellWindowRect.y + 22;
    
    count = 0;
    isPressed = 0;

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
        count -= 1;

        castTime = SDL_GetTicks();
        endTime = castTime + s * 1000;
}

bool Spell::ready()
{
    return SDL_GetTicks() > endTime;
}

void Spell::Update()
{
    if (!ready())
    {
        cellTextureRect.x = cellTextureRect.w * ((SDL_GetTicks() - castTime) / ((endTime - castTime) / 5));
        cellTextureRect.y = 64;
    }
    else if (isPressed)
    {
        cellTextureRect.y = 32;
     
        if (count > 0)
        {
            cellTextureRect.x = 0;
        }
        else
        {
            cellTextureRect.x = 32;
        }
    }
    else
    {
        cellTextureRect.x = 0;
        cellTextureRect.y = 0;
    }

    // Numbers
    if (count < 10)
    {
        firstNumberTextureRect.x = firstNumberTextureRect.w * count;
        firstNumberWindowRect.x = cellWindowRect.x + 5;
    }
    else
    {
        firstNumberTextureRect.x = firstNumberWindowRect.w * ((count - (count % 10)) / 10);
        firstNumberWindowRect.x = cellWindowRect.x + 3;
        
        secondNumberTextureRect.x = secondNumberTextureRect.w * (count % 10);
    }
}

void Spell::Render(SDL_Texture* cellTexture, SDL_Texture* spellsTexture, SDL_Texture* numbersTexture)
{
    Window::Render(cellTexture, &cellTextureRect, &cellWindowRect);
    Window::Render(spellsTexture, &spellsTextureRect, &spellsWindowRect);

    if (count < 10)
    {
        Window::Render(numbersTexture, &firstNumberTextureRect, &firstNumberWindowRect);
    }
    else
    {
        Window::Render(numbersTexture, &firstNumberTextureRect, &firstNumberWindowRect);
        Window::Render(numbersTexture, &secondNumberTextureRect, &secondNumberWindowRect);
    }
}

void Spell::TogglePressed()
{
    isPressed = !isPressed;
}

