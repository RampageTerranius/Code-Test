#pragma once

void Shutdown()
{
	SDL_DestroyWindow(mainWindow);

	SDL_Quit();
}