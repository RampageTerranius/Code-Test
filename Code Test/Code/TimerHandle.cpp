#include "TimerHandle.h"
void Timer::Start()
{
	if (!started)
	{
		started = true;
		paused = false;

		startTick = SDL_GetTicks();
	}
}

void Timer::Stop()
{
	if (started)
	{
		started = false;
		paused = false;
	}
}

void Timer::Pause()
{
	if (started && !paused)
	{
		//pause the timer
		paused = true;

		//get the time it was paused
		pauseTick = SDL_GetTicks() - startTick;
	}
}

void Timer::Unpause()
{
	if (paused)
	{
		//unpause
		paused = false;

		//set the time for the timer
		startTick = SDL_GetTicks() - pauseTick;

		//reset the paused tick
		pauseTick = 0;
	}
}

int Timer::GetTicks()
{
	//make sure timer has been started
	if (started)
		//check if the timer is paused or not
		if (paused)
			//if paused we need to get the time it was paused
			return pauseTick;
		else
			//if not paused we need to return the true time
			return SDL_GetTicks() - startTick;

	return 0;
}

void TimerHandle(Timer* cap, Timer* fps)
{
	avgFPS = countedFrames / (fps->GetTicks() / 1000.f);
	if (avgFPS > 2000000)
		avgFPS = 0;

	if (frameRateLimit > 0)
	{
		int frameTicks = cap->GetTicks();
		if (frameTicks < (1000 / frameRateLimit))
		{
			//Wait remaining time
			SDL_Delay((1000 / frameRateLimit) - frameTicks);
		}
	}
}