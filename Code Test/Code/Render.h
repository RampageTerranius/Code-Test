#pragma once

void Render()
{
	SDL_SetRenderDrawColor(mainRenderer, 0, 0, 0, 255);
	SDL_RenderClear(mainRenderer);

	//render all particles
	for (int i = 0; i < particleList.size(); i++)
		particleList.at(i)->Draw();	

	SDL_RenderPresent(mainRenderer);
	countedFrames++;
}