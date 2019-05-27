#pragma once

void Shutdown()
{
	//destroy the main window before shutdown
	SDL_DestroyWindow(mainWindow);

	//destroy the main renderer before shutdown
	SDL_DestroyRenderer(mainRenderer);

	//clear up the pointer array used for the particles
	for (int i = 0; i < WINDOW_WIDTH - 1; i++)
		for (int n = 0; n < WINDOW_HEIGHT - 1; n++)
			delete allParticles[i][n];
	
	SDL_Quit();
}