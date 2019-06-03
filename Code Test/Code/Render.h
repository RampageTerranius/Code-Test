#pragma once

void Render()
{
	SDL_SetRenderDrawColor(mainRenderer, 0, 0, 0, 255);
	SDL_RenderClear(mainRenderer);

	//render all particles
	for (int i = 0; i < particleList.size(); i++)
		particleList.at(i)->Draw();	

	//render what type of brush is selected at the top left
	brushName.SetText(typeNames[currentBrushType]);
	brushName.Draw(mainRenderer, 20, 20);

	//render what brush size is selected under that
	brushSize.SetText(std::to_string(currentBrushSize));
	brushSize.Draw(mainRenderer, 20, 54);

	SDL_RenderPresent(mainRenderer);
	countedFrames++;
}