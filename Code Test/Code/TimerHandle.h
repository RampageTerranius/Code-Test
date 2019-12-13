#pragma once

#include "Globals.h"

#include <SDL.h>

// Class for handling timers.
class Timer
{
	public:
		void Start();
		void Stop();
		void Reset();
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