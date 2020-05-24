#include "../include/Sphere.hpp"
#include "../include/Window.hpp"

Sphere::Sphere(Spheres type, int x, int y, float speedX, float speedY)
{
    const char* filename;
    switch (type)
    {
        case annihilation:
            filename = "assets/SphereA.png";
            break;

        default:
            filename = "assets/SphereA.png";
            break;
    }

    texture = Window::LoadTexture(filename);

    textureRect.h = 16;
    textureRect.w = 16;
    textureRect.x = 0;
    textureRect.y = 0;

    frames = 4;
    frameDelay = 100;

    windowRect.h = textureRect.h * Window::getMultiplierH();
    windowRect.w = textureRect.w * Window::getMultiplierW();
    windowRect.x = x - windowRect.w / 2;
    windowRect.y = y - windowRect.h / 2;

    this->x = windowRect.x;
    this->y = windowRect.y;
    this->r = windowRect.w / 2;

    maxSpeed = 15;
    reduceSpeed = 0.25;

    this->speedY = maxSpeed + speedY;
    this->speedX = speedX;
}

Sphere::~Sphere()
{
    //SDL_DestroyTexture(texture);
}

void Sphere::Update()
{
    // Movement
    x += speedX;
    y += speedY;

    // Borders collision

    // Top border
    if (y + windowRect.h < 1)
    {

    }
    // Bottom border
    if (y + windowRect.h >= Window::getHeight())
    {
        speedY *= -1;
        y = Window::getHeight() - windowRect.h;
    }
    // Left border
    if (x <= windowRect.w)
    {
        speedX += reduceSpeed * (windowRect.w - x);
    }
    // Right border
    // This is opposite border to the left so here is a little confusing formula:
    //
    // [Window::getWidth - windowRect.w - x] equal to the x on the other side
    // 
    // About [+ 1] in speedX formula... Idk why but this necessary to get
    // the same result as on the other side
    //
    if (Window::getWidth() - windowRect.w - x <= windowRect.w)
    {
        speedX -= reduceSpeed * ((windowRect.w) - ((Window::getWidth() - x - windowRect.w) + 1));
    }



    /* // Magic speed limit */
    /* if (speedX > maxSpeed || speedX < -maxSpeed) */
    /* { */
    /*     speedX -= reduceSpeed * (speedX < 0 ? -1 : 1); */
    /* } */
    /* if (speedY > maxSpeed || speedY < -maxSpeed) */
    /* { */
    /*     speedY -= reduceSpeed * (speedY < 0 ? -1 : 1); */
    /* } */

    windowRect.x = x;
    windowRect.y = y;
}

void Sphere::Render()
{
    // Animation
    textureRect.x = ((SDL_GetTicks() / frameDelay) % frames) * textureRect.w;

    Window::Render(texture, &textureRect, &windowRect);
}

bool Sphere::CheckCollision(SDL_Rect* objectRect)
{
    float centerX = x + r;
    float centerY = y + r;

    float closestX;
    float closestY;

    // Find the closest X on object to the center of Sphere
    if (centerX < objectRect->x)
    {
        closestX = objectRect->x;
    }
    else if (centerX > objectRect->x + objectRect->w)
    {
        closestX = objectRect->x + objectRect->w;
    }
    else
    {
        closestX = centerX;
    }

    // Find the closest Y on object to the center of Sphere
    if (centerY < objectRect->y)
    {
        closestY = objectRect->y;
    }
    else if (centerY > objectRect->y + objectRect->h)
    {
        closestY = objectRect->y + objectRect->h;
    }
    else
    {
        closestY = centerY;
    }

    // If distance between the closest point on object to
    // the center of Sphere is less than radius then there is collision!
    if (pow(centerX - closestX, 2) + pow(centerY - closestY, 2) < pow(r, 2))
    {
        return true;
    }
    
    return false;
}

void Sphere::Rebound(SDL_Rect* objectRect, float objectSpeedX, float objectSpeedY)
{

    float centerX = x + r;
    float centerY = y + r;

    float closestX;
    float closestY;

    // Find the closest X on object to the center of Sphere
    if (centerX < objectRect->x)
    {
        closestX = objectRect->x;
    }
    else if (centerX > objectRect->x + objectRect->w)
    {
        closestX = objectRect->x + objectRect->w;
    }
    else
    {
        closestX = centerX;
    }

    // Find the closest Y on object to the center of Sphere
    if (centerY < objectRect->y)
    {
        closestY = objectRect->y;
    }
    else if (centerY > objectRect->y + objectRect->h)
    {
        closestY = objectRect->y + objectRect->h;
    }
    else
    {
        closestY = centerY;
    }

    float deltaX = abs(closestX - centerX);
    float deltaY = abs(closestY - centerY);

    // Here rebound is happen
    //
    // From inside of the object?? It is possible with high speed
    // of the ball and should be fixed... (TODO)
    if (deltaX == 0 && deltaY == 0)
    {

    }
    // From North or South side. Just change Y direction to the opposite
    // if it isn't done already
    else if (deltaX == 0)
    {
        if ((closestY > centerY && speedY > 0) || (closestY < centerY && speedY < 0))
        {
            speedY *= -1;
        }
    }
    // From West or East side. Just change X direction to the opposite
    // if it isn't done already
    else if (deltaY == 0)
    {
        if ((closestX > centerX && speedX > 0) || (closestX < centerX && speedX < 0))
        {
            speedX *= -1;
        }
    }
    // Corners.. Change (TODO) 
    else
    {
        if ((closestY > centerY && speedY > 0) || (closestY < centerY && speedY < 0))
        {
            speedY *= -1;
        }
        else if ((closestX > centerX && speedX > 0) || (closestX < centerX && speedX < 0))
        {
            speedX *= -1;
        }
    }

    // Transfer speed from object to ball (TODO when "corner rebound" will complete)
    speedX += objectSpeedX;
    speedY += objectSpeedY;
}
