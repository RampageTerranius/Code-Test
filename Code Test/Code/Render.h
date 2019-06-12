#pragma once

void Render()
{
	//resent renderer
	SDL_SetRenderDrawColor(mainRenderer, 0, 0, 0, 255);
	SDL_RenderClear(mainRenderer);

	//render all particles	
	for (size_t i = 0; i < particleList.size(); i++)
	{
		if (drawHeat)
		{
			float tempHighest = 80;
			float tempMiddle = 0;
			float tempLowest = -50;

			Particle* particle = particleList.at(i);

			int r, g, b;

			r = g = b = 20;

			float temp = particle->temperature;
			
			if (particle->temperature >= tempMiddle)
			{
				r = 255 * (int)(1.0 - (std::abs(particle->temperature - tempHighest) / (particle->temperature + tempHighest)));
				if (r > 255)
					r = 255;
			}
			else
			{
				b = 255 * (int)(std::abs(particle->temperature - tempLowest) / (particle->temperature + tempLowest));
				if (b > 255)
					b = 255;
			}

			SDL_SetRenderDrawColor(mainRenderer, r, g, b, 0);
			SDL_RenderDrawPoint(mainRenderer, particle->x, particle->y);
		}
		else
			particleList.at(i)->Draw();

	}

	//render a box showing the brush location
	if (renderBrush)
	{
		SDL_SetRenderDrawColor(mainRenderer, 255, 255, 255, 0);

		if (currentBrushSize == 1)		
			SDL_RenderDrawPoint(mainRenderer, mouse.x, mouse.y);
		else
		{			
			//draw left side of box
			for (int i = currentBrushSize; i > -currentBrushSize; i--)
				SDL_RenderDrawPoint(mainRenderer, mouse.x - currentBrushSize, mouse.y + i);			

			//draw right side of box
			for (int i = currentBrushSize; i > -currentBrushSize; i--)
				SDL_RenderDrawPoint(mainRenderer, mouse.x + currentBrushSize, mouse.y + i);			

			//draw top of box
			for (int i = currentBrushSize; i > -currentBrushSize; i--)
				SDL_RenderDrawPoint(mainRenderer, mouse.x + i, mouse.y - currentBrushSize);

			//draw bottom of box
			for (int i = currentBrushSize; i > -currentBrushSize; i--)
				SDL_RenderDrawPoint(mainRenderer, mouse.x + i, mouse.y + currentBrushSize);
		}
	}

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

	//render the frame and increase the counter frames
	SDL_RenderPresent(mainRenderer);
	countedFrames++;
}