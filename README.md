# Arkanoid - Nine Hells

College Computer Science course project inspired by the game Arkanoid and the Nine Hells of Baator! 🔥

## 💡 Idea

I came up with the idea to flip the original Arkanoid and make a game where player is controlling one of the Space Jammers they can choose, and throwing one of the available magic spheres on the towers in the Nine Hells plane trying to destroy them to the bottom. Of course, the idea was inspired by Forgotten Realms.

## 🔮 Magic

I decided to add magic spells to the game some of which could increase the ship's speed, size, duplicate the sphere and even predict their trajectory. The energy for the spells was drawn from souls freed from imprisonment in the towers of hell by destroying the tower's blocks.

## ⚙️ Details

The game was developed using C++ and SDL2 library. All the assets were drawn by me, thankfully I had a bit of experience with pencils and pixel-arts. The game had an easy-to-use extension system, so anyone can add a new magic ship or a sphere.

---

![New game](/screenshots/new_game.png)

![Gameplay 2](/screenshots/gameplay2.png)

![Gameplay 3](/screenshots/gameplay3.png)

![Main menu](/screenshots/menu.png)

---

## Usage

1. Clone (you will need to fetch submodules):

    ```shell
    git clone --recurse-submodules https://github.com/shvixxl/arkanoid-nine-hells.git
    ```
    
    ```shell
    cd arkanoid-nine-hells
    ```

2. Build:

    ```shell
    cmake .
    ```
    
    ```shell
    make
    ```

3. Play:

    ```shell
    ./arkanoid-nine-hells
    ```

### Dependencies

- `cmake` (`>=3.17`)
- `sdl2`
- `sdl2_image`
- `sdl2_ttf`
- `jsoncpp`
