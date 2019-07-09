#pragma once

//sdl
#include <SDL.h>

#include "Globals.h"

//class for handling timers
class Timer
{
	public:
		void Start();
		void Stop();
		void Pause();
		void Unpause();
		int GetTicks();
		

	private:
		Uint32 startTick = 0;
		Uint32 pauseTick = 0;
		bool paused = false;
		bool started = false;
};

void TimerHandle(Timer* cap, Timer* fps);