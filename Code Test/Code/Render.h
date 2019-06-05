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

	//render brush temperature
	brushTemperature.SetText(std::to_string(currentBrushTemperature) + "c");
	brushTemperature.Draw(mainRenderer, 20, 88);

	//render selected particle type and temperature
	if (allParticles[mouse.x][mouse.y] != nullptr)
	{
		selectedParticleTemperature.SetText(std::to_string(allParticles[mouse.x][mouse.y]->temperature) + "c");
		selectedParticleName.SetText(typeNames[allParticles[mouse.x][mouse.y]->type]);
	}
	else
	{
		selectedParticleName.SetText("None");
		selectedParticleTemperature.SetText("0c");
	}	

	selectedParticleName.Draw(mainRenderer, 20, 172);
	selectedParticleTemperature.Draw(mainRenderer, 20, 206);


	//render the current framerate
	currentFrameRate.SetText(std::to_string(static_cast<int>(avgFPS)));
	currentFrameRate.Draw(mainRenderer, 20, WINDOW_HEIGHT - 34);

	

	SDL_RenderPresent(mainRenderer);
	countedFrames++;
}