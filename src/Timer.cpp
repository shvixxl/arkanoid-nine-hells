#include "../include/Timer.hpp"

Timer::Timer(Uint32 delay)
{
    this->start = SDL_GetTicks();
    this->delay = delay;
}

Timer::~Timer()
{

}

void Timer::Set(Uint32 delay)
{
    this->delay = delay;
}

void Timer::Restart()
{
    this->start = SDL_GetTicks();
}

bool Timer::Ready()
{
    return start + delay < SDL_GetTicks();
}

