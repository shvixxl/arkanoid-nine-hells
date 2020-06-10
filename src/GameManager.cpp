#include "../include/GameManager.hpp"

Scenes GameManager::currentScene;
Levels GameManager::currentLevel;

Json::Value GameManager::config;

Json::Value GameManager::menuData;

Button** GameManager::buttons = nullptr;
int GameManager::buttonsCount;
int GameManager::selectedButton;

std::vector<Object> GameManager::objects;

bool GameManager::Init()
{
    std::ifstream file;

    // Load configuration file
    file.open("config.json");
    if (file.is_open())
    {
        file >> config;
    }
    else
    {
        printf("Configuration file could not be found! Loading defaults settings...\n");
        GameManager::LoadDefaults();
    }
    file.close();

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
    menuData = menuData[config["language"].asString()];
    file.close();

    // Default scene is Main Menu
    currentScene = SCENES_NULL;
    currentLevel = LEVELS_NULL;

    return true;
}

void GameManager::Clean()
{
    GameManager::clearMenu();

    std::ofstream file;

    // Save configuration file
    file.open("config.json");
    file << config;
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
                        if (currentLevel != LEVELS_NULL)
                        {
                            GameManager::LoadScene(level);
                        }
                        else if (buttons[selectedButton]->Available())
                        {
                            GameManager::LoadSave();
                        }
                        break;

                    // "New game"
                    case 1:
                        GameManager::LoadScene(new_game);
                        break;

                    // "Settings"
                    case 2:
                        GameManager::LoadScene(settings_menu);
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
                if (currentLevel == LEVELS_NULL)
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
    if (currentScene == new_game)
    {
        GameManager::LoadScene(level);
    }
    else if (currentScene == level)
    {
        EntityManager::UpdateSpheres();

        EntityManager::UpdateShip();

        MapManager::Update();

        SpellManager::Update();
    }
}

void GameManager::Render()
{
    if (currentLevel != LEVELS_NULL)
    {
        MapManager::Render();

        EntityManager::RenderShip();

        EntityManager::RenderSpheres();

        SpellManager::Render();
    }

    if (currentScene == main_menu ||
        currentScene == quit_game)
    {
        for (int i = 0; i < buttonsCount; ++i)
        {
            buttons[i]->Render();
        }

        for (size_t i = 0; i < objects.size(); ++i)
        {
            objects.at(i).Render();
        }
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

        // Check whether save file exist or not to deside "Continue" button
        // availability and select "New Game" button if there is no save file
        std::ifstream file("save");
        if (!file.is_open() && currentLevel == LEVELS_NULL)
        {
            buttons[0]->toggleAvailable();
            changeSelectedButton(selectedButton + 1);
        }
        file.close();
    }
    else if (scene == new_game)
    {

    }
    else if (scene == settings_menu)
    {

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
        
        if (currentLevel == LEVELS_NULL)
        {
            MapManager::Init(avernus, 60, 12);
            SpellManager::Init();
            EntityManager::addShip(skyship);
            SpellManager::addSpell(summon_sphere, 99);
            currentLevel = avernus;
        }
    }

    currentScene = scene;
}

void GameManager::LoadDefaults()
{
    std::ofstream file("config.json");

    // Default settings
    config["language"] = "en";

    file << config;
}

void GameManager::LoadSave()
{

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

