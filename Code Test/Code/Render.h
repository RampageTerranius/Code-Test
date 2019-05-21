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
			case TYPE_PLAYER:
				SDL_Rect pl;
				pl.h = 20;
				pl.w = 20;
				pl.x = e.x-10;
				pl.y = e.y-10;
				
				SDL_SetRenderDrawColor(mainRenderer, 0, 0, 255, 255);
				SDL_RenderFillRect(mainRenderer, &pl);
				break;

			case TYPE_WALL:
				SDL_Rect wa;
				wa.h = 20;
				wa.w = 20;
				wa.x = e.x-10;
				wa.y = e.y-10;

				SDL_SetRenderDrawColor(mainRenderer, 255, 255, 255, 255);
				SDL_RenderFillRect(mainRenderer, &wa);
				break;
		}
	}

	SDL_RenderPresent(mainRenderer);
}