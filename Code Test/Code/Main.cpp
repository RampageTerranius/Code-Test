#include "Main.h"

int main(int argc, char* argv[])
{
	//run setup and check that it was sucessful
	running = Setup();

	//handles all input given by the user
	SDL_Event event;

	//timers for handling fps
	Timer capTimer;
	Timer fpsTimer;	

	fpsTimer.Start();

	while(running)
	{
		capTimer.Start();

		//render the current scene
		Render();

		//handle any input from the user
		EventHandle(event);

		//handle all entitys gravity/movement etc
		EntityHandle();

		//check timer and make sure to keep the engien running at the correct fps
		TimerHandle(&capTimer, &fpsTimer);
	}	
	
	Shutdown();

	return 1;
}
