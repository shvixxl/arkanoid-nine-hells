#include "../include/GameManager.hpp"
#include <SDL2/SDL_keycode.h>

bool GameManager::isPlaying;

Scenes GameManager::currentScene;
std::string GameManager::currentLevel;

Json::Value GameManager::menuData;

Button** GameManager::buttons = nullptr;
int GameManager::buttonsCount;
int GameManager::selectedButton;

std::vector<Object> GameManager::objects;

int GameManager::levelsCount;
int GameManager::selectedLevel;

Ship** GameManager::ships;
int GameManager::shipsCount;
int GameManager::selectedShip;

Sphere** GameManager::spheres;
int GameManager::spheresCount;
int GameManager::selectedSphere;

int GameManager::firstButton;
std::vector<std::string> GameManager::scoreboard_data;
std::string GameManager::username;
int GameManager::score;

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

    // Load scoreboard
    file.open("scoreboard.txt");
    if (!file.is_open())
    {
        
    }
    else
    {
        std::string line;
        while (std::getline(file, line))
        {
            scoreboard_data.push_back(line);
        }
    }
    file.close();

    isPlaying = false;
    currentScene = SCENES_NULL;

    return true;
}

void GameManager::Clean()
{
    GameManager::clearMenu();
    GameManager::clearGame();

    std::ofstream file("scoreboard.txt");
    for (size_t i = 0; i < scoreboard_data.size(); ++i)
    {
        file << scoreboard_data.at(i) << std::endl;
    }
}

int GameManager::HandleEvents(SDL_Event* event)
{
    if (currentScene == game_over ||
        currentScene == victory)
    {
        if (event->type == SDL_KEYDOWN)
        {
            if (event->key.keysym.sym == SDLK_SPACE ||
                event->key.keysym.sym == SDLK_RETURN ||
                event->key.keysym.sym == SDLK_ESCAPE)
            {
                if (currentScene == victory)
                {
                    GameManager::LoadScene(input_name);
                }
                else
                {
                    GameManager::LoadScene(scoreboard);
                }
            }
        }
    }
    else if (currentScene == main_menu)
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
                        if (isPlaying)
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
                else if (isPlaying)
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
                        selectedLevel += 1;
                        if (selectedLevel >= levelsCount)
                        {
                            selectedLevel = 0;
                        }
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
                        selectedLevel -= 1;
                        if (selectedLevel < 0)
                        {
                            selectedLevel = levelsCount - 1;
                        }
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
                Json::Value data;
                std::ifstream file("data/levels.json");
                if (!file.is_open())
                {
                    printf("Some data files is missing!");
                }
                file >> data;
                file.close();

                currentLevel = data["levels"][selectedLevel].asString();

                GameManager::clearGame();

                MapManager::Init(currentLevel.c_str());
                EntityManager::Init(ships[selectedShip]->getType().c_str(), spheres[selectedSphere]->getType().c_str());
                SpellManager::Init();

                EntityManager::addShip();

                SpellManager::addSpell(summon_sphere, 5);
                SpellManager::addSpell(displacement, 5);
                SpellManager::addSpell(haste, 5);
                SpellManager::addSpell(find_path, 5);

                GameManager::LoadScene(level);

                isPlaying = true;
            }
            else if (event->key.keysym.sym == SDLK_ESCAPE)
            {
                GameManager::LoadScene(main_menu);
            }
        }
    }
    else if (currentScene == scoreboard)
    {
        if (event->type == SDL_KEYDOWN)
        {
            // Select previous button
            if (event->key.keysym.sym == SDLK_UP ||
                event->key.keysym.sym == SDLK_w)
            {
                selectedButton -= 1;
                if (selectedButton < 0 && firstButton == 0)
                {
                    if ((int) scoreboard_data.size() >= buttonsCount)
                    {
                        firstButton = scoreboard_data.size() - buttonsCount - 1;
                    }
                    selectedButton = buttonsCount;
                }
                GameManager::LoadScene(scoreboard);
            }
            // Select next button
            else if (event->key.keysym.sym == SDLK_DOWN ||
                     event->key.keysym.sym == SDLK_s)
            {
                selectedButton += 1;
                if (selectedButton >= buttonsCount && firstButton == (int) scoreboard_data.size() - 12)
                {
                    firstButton = 0;
                    selectedButton = 0;
                }
                GameManager::LoadScene(scoreboard);
            }
            else if (event->key.keysym.sym == SDLK_DELETE)
            {
                scoreboard_data.erase(scoreboard_data.begin() + selectedButton + firstButton);
                GameManager::LoadScene(scoreboard);
            }
            else if (event->key.keysym.sym == SDLK_ESCAPE ||
                     event->key.keysym.sym == SDLK_LEFT ||
                     event->key.keysym.sym == SDLK_RETURN)
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
    else if (currentScene == input_name)
    {
        if (event->type == SDL_KEYDOWN)
        {
            // Backspace handle
            if (event->key.keysym.sym == SDLK_BACKSPACE && username.length() > 0)
            {
                username.pop_back();
                buttons[0]->changeText(username.c_str());
            }
            // Select current name
            else if (event->key.keysym.sym == SDLK_RETURN && username.length() > 0)
            {
                SDL_StopTextInput();
                GameManager::writeScore();
                GameManager::LoadScene(scoreboard);
            }
        }
        else if (event->type == SDL_TEXTINPUT && event->text.text[0] != ' ')
        {
            username += event->text.text;
            buttons[0]->changeText(username.c_str());
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
        if (!MapManager::isBricks())
        {
            GameManager::LoadScene(victory);
            isPlaying = false;
        }
        else if (!EntityManager::isSpheres() && !MapManager::isSouls() && !SpellManager::isSummonSphere())
        {
            GameManager::LoadScene(game_over);
            isPlaying = false;
        }
    }

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

    if (currentScene == game_over ||
        currentScene == victory ||
        currentScene == main_menu ||
        currentScene == scoreboard ||
        currentScene == input_name ||
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
        
        objects.at(selectedLevel).Render();

        ships[selectedShip]->Render();
        objects.at(levelsCount + selectedShip).Render();

        spheres[selectedSphere]->Render();
        objects.at(levelsCount + shipsCount + selectedSphere).Render();
    }
}

void GameManager::LoadScene(Scenes scene)
{
    if (scene == game_over)
    {
        GameManager::clearMenu();

        objects.push_back(Object(Window::LoadTexture(menuData["game_over"]["logo"].asString().c_str()), -1, -1));
    }
    else if (scene == victory)
    {
        GameManager::clearMenu();

        objects.push_back(Object(Window::LoadTexture(menuData["victory"]["logo"].asString().c_str()), -1, -1));
    }
    else if (scene == main_menu)
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
        
        // Or "New Game"
        if (!isPlaying)
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

        // Load levels
        file.open("data/levels.json");
        if (!file.is_open())
        {
            printf("Some data files is missing!");
        }
        file >> data;
        file.close();

        // Load levels
        levelsCount = data["levels"].size();
        for (int i = 0; i < levelsCount; i++)
        {
            SDL_Color color;
            color.r = data[data["levels"][i].asString()]["color"][0].asInt();
            color.g = data[data["levels"][i].asString()]["color"][1].asInt();
            color.b = data[data["levels"][i].asString()]["color"][2].asInt();
            color.a = data[data["levels"][i].asString()]["color"][3].asInt();

            objects.push_back(Object(Window::LoadText("font/PixelAzureBonds.ttf", data[data["levels"][i].asString()]["name"].asString().c_str(), 16, color), -1, 8 + 10 + 8));
        }
        selectedLevel = 0;

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
            SDL_Color color;
            color.r = data[data["ships"][i].asString()]["color"][0].asInt();
            color.g = data[data["ships"][i].asString()]["color"][1].asInt();
            color.b = data[data["ships"][i].asString()]["color"][2].asInt();
            color.a = data[data["ships"][i].asString()]["color"][3].asInt();

            ships[i] = new Ship(data["ships"][i].asString().c_str(), -1, 8 + 80 + 18);
            objects.push_back(Object(Window::LoadText("font/PixelAzureBonds.ttf", data[data["ships"][i].asString()]["name"].asString().c_str(), 10, color), -1, 8 + 160 - 24));
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
            SDL_Color color;
            color.r = data[data["spheres"][i].asString()]["color"][0].asInt();
            color.g = data[data["spheres"][i].asString()]["color"][1].asInt();
            color.b = data[data["spheres"][i].asString()]["color"][2].asInt();
            color.a = data[data["spheres"][i].asString()]["color"][3].asInt();
            
            spheres[i] = new Sphere(data["spheres"][i].asString().c_str(), Window::getWidth() / 2, 16 + 160 + 18, 0, 0);
            objects.push_back(Object(Window::LoadText("font/PixelAzureBonds.ttf", data[data["spheres"][i].asString()]["name"].asString().c_str(), 10, color), -1, 8 + 240 - 24));
        }
        selectedSphere = 0;
    }
    else if (scene == scoreboard)
    {
        GameManager::clearMenu();
        
        buttonsCount = (int) scoreboard_data.size() < 12 ? scoreboard_data.size() : 12;
        buttons = new Button*[buttonsCount];

        if (currentScene != scoreboard)
        {
            selectedButton = 0;
            firstButton = 0;
        }
        
        if (firstButton + buttonsCount > (int) scoreboard_data.size())
        {
            firstButton = scoreboard_data.size() - buttonsCount;
            if (firstButton < 0)
            {
                firstButton = 0;
            }
        }
        
        if (selectedButton > buttonsCount - 5)
        {
            if (firstButton < (int) scoreboard_data.size() - buttonsCount)
            {
                selectedButton -= 1;
                firstButton += 1;
            }
        }
        else if (selectedButton < 4)
        {
            if (firstButton > 0)
            {
                firstButton -= 1;
                selectedButton += 1;
            }
        }

        int button = 0;
        for (size_t i = firstButton; i < scoreboard_data.size() && button < buttonsCount; i++)
        {
            buttons[button] = new Button((std::to_string(i + 1) + " - " + scoreboard_data.at(i)).c_str(), -1, 8 + 20 * button);
            button++;
        }

        buttons[selectedButton]->toggleSelected();
    }
    else if (scene == quit_game)
    {
        GameManager::clearMenu();

        // Load header
        objects.push_back(Object(Window::LoadText("font/PixelAzureBonds.ttf", menuData["confirmation"]["header"].asString().c_str(), 16, {250, 250, 250, 255}), -1, 64));

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
    else if (scene == input_name)
    {
        GameManager::clearMenu();

        objects.push_back(Object(Window::LoadText("font/PixelAzureBonds.ttf", menuData["input_name"]["header"].asString().c_str(), 16, {250, 250, 250, 255}), -1, 64));

        username = "";
        
        buttonsCount = 1;
        buttons = new Button*[buttonsCount];
        buttons[0] = new Button(username.c_str(), -1, -1);

        SDL_StartTextInput();
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

        buttonsCount = 0;
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

        shipsCount = 0;
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

        spheresCount = 0;
    }
}

void GameManager::clearGame()
{
    EntityManager::Clean();
    MapManager::Clean();
    SpellManager::Clean();
}

void GameManager::addScore(int amount)
{
    score += amount;
}

void GameManager::writeScore()
{
    size_t i;
    for (i = 0; i < scoreboard_data.size(); ++i)
    {
        int score_temp = std::stoi(scoreboard_data.at(i).substr(scoreboard_data.at(i).find(' ') + 1));
        if (score_temp < score)
        {
            break;
        }
    }
    scoreboard_data.emplace(scoreboard_data.begin() + i, username + ' ' + std::to_string(score));
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

    // Set postition of the button to the center of the window if x and/or y not provided
    if (x != -1)
    {
        windowRect.x = x;
    }
    else
    {
        windowRect.x = Window::getWidth() / 2 - windowRect.w / 2;
    }
    if (y != -1)
    {
        windowRect.y = y;
    }
    else
    {
        windowRect.y = Window::getHeight() / 2 - windowRect.h / 2;
    }

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
    SDL_Color textureColor = {50, 50, 50, 255};
    SDL_Color textureColorAvailable = {150, 150, 150, 255};
    SDL_Color textureColorSelected = {250, 250, 250, 255};

    SDL_DestroyTexture(texture);
    SDL_DestroyTexture(textureAvailable);
    SDL_DestroyTexture(textureSelected);

    // Load textures for text
    texture = Window::LoadText("font/PixelAzureBonds.ttf", text, 10, textureColor);
    textureAvailable = Window::LoadText("font/PixelAzureBonds.ttf", text, 10, textureColorAvailable);
    textureSelected = Window::LoadText("font/PixelAzureBonds.ttf", text, 10, textureColorSelected);

    // Initialize width and height of the button
    int w, h;
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);

    windowRect.x = windowRect.x + (windowRect.w - w) / 2;

    windowRect.w = w;
    windowRect.h = h;

    isAvailable = true;
    isSelected = false;
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
    if (y != -1)
    {
        windowRect.y = y;
    }
    else 
    {
        windowRect.y = Window::getHeight() / 2 - windowRect.h / 2;
    }
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

