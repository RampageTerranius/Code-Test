#pragma once

void Render()
{
	SDL_SetRenderDrawColor(mainRenderer, 0, 0, 0, 255);
	SDL_RenderClear(mainRenderer);

	//render all entities
	for (int i = 0; i < entityList.size(); i++)
	{
		Entity e = entityList.at(i);

		switch (e.type)
		{
			case TYPE_SAND:
				
				SDL_SetRenderDrawColor(mainRenderer, 207, 226, 34, 0);
				SDL_RenderDrawPoint(mainRenderer, e.x, e.y);
				break;

			case TYPE_WALL:
				SDL_SetRenderDrawColor(mainRenderer, 87, 87, 87, 0);
				SDL_RenderDrawPoint(mainRenderer, e.x, e.y);
				break;
		}
	}

	SDL_RenderPresent(mainRenderer);
	countedFrames++;
}