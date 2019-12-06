#include "Main.h"

int main(int argc, char* argv[])
{
	// Run setup and check that it was sucessful.
	running = Setup();

	// Handles all input given by the user.
	SDL_Event event;

	// Timers for handling fps.
	Timer capTimer;
	Timer fpsTimer;	

	fpsTimer.Start();

	while(running)
	{
		// Restart the fps cap timer.
		capTimer.Start();

		// Handle any input from the user.
		EventHandle(event);

		// Handle all entitys gravity/movement etc.
		EntityHandle();

		// Render the current scene.
		Render();

		// Check timer and make sure to keep the engien running at the correct fps.
		TimerHandle(&capTimer, &fpsTimer);
	}	
	
	// Shutdown the engine and cleanup data.
	Shutdown();

	return 1;
}
