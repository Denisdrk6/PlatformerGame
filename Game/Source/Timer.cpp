#include "Timer.h"
#include "SDL\include\SDL_timer.h"

// ---------------------------------------------
Timer::Timer()
{
	Start();
}

// ---------------------------------------------
void Timer::Start()
{
	startedAt = SDL_GetTicks();
}

// ---------------------------------------------
uint Timer::Read() const
{
	return SDL_GetTicks() - startedAt;
}

// ---------------------------------------------
float Timer::ReadSec() const
{
	return float(SDL_GetTicks() - startedAt) / 1000.0f;
}