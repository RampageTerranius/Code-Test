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
		{
			delete allParticles[i][n];
			allParticles[i][n] = nullptr;
		}

	//clean up all TTF files
	brushName.Clear();
	brushSize.Clear();
	brushTemperature.Clear();
	selectedParticleTemperature.Clear();
	selectedParticleName.Clear();
	currentFrameRate.Clear();
	
	SDL_Quit();
}