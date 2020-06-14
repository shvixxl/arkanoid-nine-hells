#include "../include/GameManager.hpp"


Scenes GameManager::currentScene;
std::string GameManager::currentLevel;

Json::Value GameManager::menuData;

Button** GameManager::buttons = nullptr;
int GameManager::buttonsCount;
int GameManager::selectedButton;

std::vector<Object> GameManager::objects;

Ship** GameManager::ships;
int GameManager::shipsCount;
int GameManager::selectedShip;

Sphere** GameManager::spheres;
int GameManager::spheresCount;
int GameManager::selectedSphere;


bool GameManager::Init()
{
    std::ifstream file;

    // Load data files
    //
    // menu
    file.open("data/menu.json");
    if (!file.is_open())
    {
        printf("Some data files is missing!");
        return false;
    }
    file >> menuData;
    file.close();

    // Default scene is Main Menu
    currentScene = SCENES_NULL;

    return true;
}

void GameManager::Clean()
{
    GameManager::clearMenu();
    GameManager::clearGame();
}

int GameManager::HandleEvents(SDL_Event* event)
{
    if (currentScene == main_menu)
    {
        if (event->type == SDL_KEYDOWN)
        {
            // Select previous button
            if (event->key.keysym.sym == SDLK_UP ||
                event->key.keysym.sym == SDLK_w)
            {
                changeSelectedButton(selectedButton - 1);
            }
            // Select next button
            else if (event->key.keysym.sym == SDLK_DOWN ||
                     event->key.keysym.sym == SDLK_s)
            {
                changeSelectedButton(selectedButton + 1);
            }
            // Press selected button
            else if (event->key.keysym.sym == SDLK_SPACE ||
                     event->key.keysym.sym == SDLK_RETURN ||
                     event->key.keysym.sym == SDLK_RIGHT)
            {
                switch (selectedButton)
                {
                    // "Continue"
                    case 0:
                        if (!currentLevel.empty())
                        {
                            GameManager::LoadScene(level);
                        }
                        break;

                    // "New game"
                    case 1:
                        GameManager::LoadScene(new_game);
                        break;

                    // "Settings"
                    case 2:
                        GameManager::LoadScene(scoreboard);
                        break;

                    // "Exit"
                    case 3:
                        GameManager::LoadScene(quit_game);
                        break;

                    default:
                        break;
                }
            }
            else if (event->key.keysym.sym == SDLK_ESCAPE)
            {
                if (currentLevel.empty())
                {
                    GameManager::LoadScene(quit_game);
                }
                else
                {
                    GameManager::LoadScene(level);
                }
            }
        }
    }
    else if (currentScene == new_game)
    {
        if (event->type == SDL_KEYDOWN)
        {
            // Select previous button
            if (event->key.keysym.sym == SDLK_UP ||
                event->key.keysym.sym == SDLK_w)
            {
                changeSelectedButton(selectedButton - 1);
            }
            // Select next button
            else if (event->key.keysym.sym == SDLK_DOWN ||
                     event->key.keysym.sym == SDLK_s)
            {
                changeSelectedButton(selectedButton + 1);
            }
            // Choose next option
            else if (event->key.keysym.sym == SDLK_RIGHT ||
                     event->key.keysym.sym == SDLK_d)
            {
                switch (selectedButton)
                {
                    case 0:
                        break;

                    case 1:
                        selectedShip += 1;
                        if (selectedShip >= shipsCount)
                        {
                            selectedShip = 0;
                        }
                        break;

                    case 2:
                        selectedSphere += 1;
                        if (selectedSphere >= spheresCount)
                        {
                            selectedSphere = 0;
                        }
                        break;

                    default:
                        break;
                }
            }
            // Choose previous option
            else if (event->key.keysym.sym == SDLK_LEFT ||
                     event->key.keysym.sym == SDLK_a)
            {
                switch (selectedButton)
                {
                    case 0:
                        break;

                    case 1:
                        selectedShip -= 1;
                        if (selectedShip < 0)
                        {
                            selectedShip = shipsCount - 1;
                        }
                        break;

                    case 2:
                        selectedSphere -= 1;
                        if (selectedSphere < 0)
                        {
                            selectedSphere = spheresCount - 1;
                        }
                        break;

                    default:
                        break;
                }
            }
            else if (event->key.keysym.sym == SDLK_RETURN)
            {
                currentLevel = "avernus";
                
                GameManager::clearGame();

                MapManager::Init(currentLevel.c_str());
                EntityManager::Init(ships[selectedShip]->getType().c_str(), spheres[selectedSphere]->getType().c_str());
                SpellManager::Init();

                EntityManager::addShip();

                SpellManager::addSpell(summon_sphere, 99);
                SpellManager::addSpell(displacement, 99);
                SpellManager::addSpell(haste, 99);
                SpellManager::addSpell(find_path, 99);

                GameManager::LoadScene(level);
            }
            else if (event->key.keysym.sym == SDLK_ESCAPE)
            {
                GameManager::LoadScene(main_menu);
            }
        }
    }
    else if (currentScene == quit_game)
    {
        if (event->type == SDL_KEYDOWN)
        {
            // Select previous button
            if (event->key.keysym.sym == SDLK_LEFT ||
                event->key.keysym.sym == SDLK_a)
            {
                changeSelectedButton(selectedButton - 1);
            }
            // Select next button
            else if (event->key.keysym.sym == SDLK_RIGHT ||
                     event->key.keysym.sym == SDLK_d)
            {
                changeSelectedButton(selectedButton + 1);
            }
            // Press selected button
            else if (event->key.keysym.sym == SDLK_SPACE ||
                     event->key.keysym.sym == SDLK_RETURN)
            {
                switch (selectedButton)
                {
                    // "No"
                    case 0:
                        GameManager::LoadScene(main_menu);
                        break;

                    // "Yes"
                    case 1:
                        return -1;
                        break;

                    default:
                        break;
                }
            }
            // Press "No"
            else if (event->key.keysym.sym == SDLK_ESCAPE)
            {
                GameManager::LoadScene(main_menu);
            }
        }
    }
    else if (currentScene == level)
    {
        // Open menu in game
        if (event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_ESCAPE)
        {
            GameManager::LoadScene(main_menu);
        }
        else
        {
            SpellManager::HandleEvents(event);
            EntityManager::ShipHandleEvents(event);
        }
    }

    return 0;
}

void GameManager::Update()
{
    if (currentScene == level)
    {
        EntityManager::UpdateSpheres();

        EntityManager::UpdateShip();

        MapManager::Update();

        SpellManager::Update();
    }
}

void GameManager::Render()
{
    if (!currentLevel.empty())
    {
        MapManager::Render();

        EntityManager::RenderShip();

        EntityManager::RenderSpheres();

        SpellManager::Render();
    }

    if (currentScene == main_menu ||
        currentScene == quit_game)
    {
        Window::Blur();

        for (int i = 0; i < buttonsCount; ++i)
        {
            buttons[i]->Render();
        }

        for (size_t i = 0; i < objects.size(); ++i)
        {
            objects.at(i).Render();
        }
    }
    else if (currentScene == new_game)
    {
        Window::Blur();
        for (int i = 0; i < buttonsCount; ++i)
        {
            buttons[i]->Render();
        }

        ships[selectedShip]->Render();
        objects.at(selectedShip).Render();

        spheres[selectedSphere]->Render();
        objects.at(shipsCount + selectedSphere).Render();
    }
}

void GameManager::LoadScene(Scenes scene)
{
    if (scene == main_menu)
    {
        GameManager::clearMenu();

        // Load logo texture
        objects.push_back(Object(Window::LoadTexture(menuData["main_menu"]["logo"].asString().c_str()), -1, 32));

        // Load buttons
        const Json::Value buttons_array = menuData["main_menu"]["buttons"];

        buttonsCount = buttons_array.size();

        buttons = new Button*[buttonsCount];
        for (int i = 0; i < buttonsCount; ++i)
        {
            buttons[i] = new Button(buttons_array[i]["text"].asString().c_str(), -1, 128 + (32) * i);
        }

        // Select by default "Continue" button
        selectedButton = 0;
        buttons[selectedButton]->toggleSelected();

        if (currentLevel.empty())
        {
            buttons[0]->toggleAvailable();
            changeSelectedButton(selectedButton + 1);
        }
    }
    else if (scene == new_game)
    {
        GameManager::clearMenu();

        // Load buttons
        const Json::Value buttons_array = menuData["new_game"]["buttons"];

        buttonsCount = buttons_array.size();

        buttons = new Button*[buttonsCount];
        for (int i = 0; i < buttonsCount; ++i)
        {
            buttons[i] = new Button(buttons_array[i]["text"].asString().c_str(), -1, 8 + 80 * i);
        }

        selectedButton = 0;
        buttons[selectedButton]->toggleSelected();
      
        Json::Value data;
        std::ifstream file;

        // Load ships
        file.open("data/ships.json");
        if (!file.is_open())
        {
            printf("Some data files is missing!");
        }
        file >> data;
        file.close();

        shipsCount = data["ships"].size();
        ships = new Ship*[shipsCount];
        for (int i = 0; i < shipsCount; i++)
        {
            ships[i] = new Ship(data["ships"][i].asString().c_str(), -1, 8 + 80 + 18);
            objects.push_back(Object(Window::LoadText("font/PixelAzureBonds.ttf", data[data["ships"][i].asString().c_str()]["name"].asString().c_str(), 10, {64, 64, 128, 255}), -1, 8 + 160 - 24));
        }
        selectedShip = 0;

        // Load spheres
        file.open("data/spheres.json");
        if (!file.is_open())
        {
            printf("Some data files is missing!");
        }
        file >> data;
        file.close();

        spheresCount = data["spheres"].size();
        spheres = new Sphere*[spheresCount];
        for (int i = 0; i < spheresCount; i++)
        {
            spheres[i] = new Sphere(data["spheres"][i].asString().c_str(), Window::getWidth() / 2, 16 + 160 + 18, 0, 0);
            objects.push_back(Object(Window::LoadText("font/PixelAzureBonds.ttf", data[data["spheres"][i].asString().c_str()]["name"].asString().c_str(), 10, {64, 128, 64, 255}), -1, 8 + 240 - 24));
        }
        selectedSphere = 0;

    }
    else if (scene == quit_game)
    {
        GameManager::clearMenu();

        // Load header
        objects.push_back(Object(Window::LoadText("font/PixelAzureBonds.ttf", menuData["confirmation"]["header"].asString().c_str(), 16, {255, 255, 255, 255}), -1, 64));

        // Load buttons
        const Json::Value buttons_array = menuData["confirmation"]["buttons"];

        buttonsCount = buttons_array.size();

        buttons = new Button*[buttonsCount];
        for (int i = 0; i < buttonsCount; ++i)
        {
            buttons[i] = new Button(buttons_array[i]["text"].asString().c_str(), 64 + 40 * i, 128);
        }

        // Select by default "No" button
        selectedButton = 0;
        buttons[selectedButton]->toggleSelected();
    }
    else if (scene == level)
    {
        GameManager::clearMenu();
    }

    currentScene = scene;
}

void GameManager::changeSelectedButton(int newButton)
{
    buttons[selectedButton]->toggleSelected();

    if (newButton < 0)
    {
        selectedButton = buttonsCount - 1;
    }
    else if (newButton >= buttonsCount)
    {
        selectedButton = 0;
    }
    else
    {
        selectedButton = newButton;
    }

    buttons[selectedButton]->toggleSelected();
}

void GameManager::clearMenu()
{
    // Clear menu
    if (buttons)
    {
        for (int i = 0; i < buttonsCount; ++i)
        {
            delete buttons[i];
            buttons[i] = nullptr;
        }

        delete[] buttons;
        buttons = nullptr;
    }

    // Clear objects
    for (size_t i = 0; i < objects.size(); ++i)
    {
        objects.at(i).Clean();
    }
    objects.clear();

    // Clear ships
    if (ships)
    {
        for (int i = 0; i < shipsCount; ++i)
        {
            delete ships[i];
            ships[i] = nullptr;
        }

        delete[] ships;
        ships = nullptr;
    }
    
    // Clear spheres
    if (spheres)
    {
        for (int i = 0; i < spheresCount; ++i)
        {
            spheres[i]->Clean();
            delete spheres[i];
            spheres[i] = nullptr;
        }

        delete[] spheres;
        spheres = nullptr;
    }
}

void GameManager::clearGame()
{
    EntityManager::Clean();
    MapManager::Clean();
    SpellManager::Clean();
}






Button::Button(const char* text, int x, int y)
{
    SDL_Color textureColor = {50, 50, 50, 255};
    SDL_Color textureColorAvailable = {150, 150, 150, 255};
    SDL_Color textureColorSelected = {250, 250, 250, 255};

    // Load textures for text
    texture = Window::LoadText("font/PixelAzureBonds.ttf", text, 10, textureColor);
    textureAvailable = Window::LoadText("font/PixelAzureBonds.ttf", text, 10, textureColorAvailable);
    textureSelected = Window::LoadText("font/PixelAzureBonds.ttf", text, 10, textureColorSelected);

    // Initialize width and height of the button
    int w, h;
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);

    windowRect.w = w;
    windowRect.h = h;

    // Set postition of the button to the center of the window if no x provided
    if (x != -1)
    {
        windowRect.x = x;
    }
    else
    {
        windowRect.x = Window::getWidth() / 2 - windowRect.w / 2;
    }

    windowRect.y = y;

    isAvailable = true;
    isSelected = false;
}

Button::~Button()
{
    SDL_DestroyTexture(texture);
    SDL_DestroyTexture(textureAvailable);
    SDL_DestroyTexture(textureSelected);
}

void Button::Update()
{

}

void Button::Render()
{
    if (isSelected)
    {
        Window::Render(textureSelected, NULL, &windowRect);
    }
    else if (isAvailable)
    {
        Window::Render(textureAvailable, NULL, &windowRect);
    }
    else
    {
        Window::Render(texture, NULL, &windowRect);
    }

}

void Button::changeText(const char* text)
{
    printf("%s", text);
}

void Button::toggleAvailable()
{
    isAvailable = !isAvailable;
}

void Button::toggleSelected()
{
    isSelected = !isSelected;
}






Object::Object(SDL_Texture* texture, int x, int y)
{
    this->texture = texture;

    int w, h;
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    windowRect.w = w;
    windowRect.h = h;

    if (x != -1)
    {
        windowRect.x = x;
    }
    else
    {
        windowRect.x = Window::getWidth() / 2 - windowRect.w / 2;
    }
    windowRect.y = y;
}

Object::~Object()
{

}

void Object::Clean()
{
    SDL_DestroyTexture(texture);
}

void Object::Render()
{
    Window::Render(texture, NULL, &windowRect);
}

