#include "Render.h"

#include "TTF.h"
#include "Globals.h"

//sdl
#include <SDL.h>

#include "Particles.h"
#include "Mouse.h"

void Render()
{
	// Lock the main surface for direct editing.
	SDL_LockSurface(mainSurface);

	// Set the surface to black.
	SDL_FillRect(mainSurface, nullptr, 0x000000);

	// render all particles.
	for (int i = WINDOW_WIDTH - 1; i >= 0; i--)
		for (int n = WINDOW_HEIGHT - 1; n >= 0; n--)
	{
		Particle* particle = allParticles[i][n];
		if (drawHeat)
		{
			float tempHighest = 80;
			float tempMiddle = 0;
			float tempLowest = -50;

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
			SDL_RenderDrawPoint(mainRenderer, particle->point.x, particle->point.y);
		}
		else
			particle->Draw();
	}

	SDL_Texture* pixelTexture = SDL_CreateTextureFromSurface(mainRenderer, mainSurface);
	SDL_RenderCopy(mainRenderer, pixelTexture, nullptr, nullptr);
	
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
	if (!createAsSource)
		brushName.SetText(mainRenderer, typeNames[currentBrushType]);
	else
		brushName.SetText(mainRenderer, "Source - " + typeNames[currentBrushType]);
	brushName.Draw(mainRenderer, 20, 20);

	//render what brush size is selected under that
	brushSize.SetText(mainRenderer, std::to_string(currentBrushSize));
	brushSize.Draw(mainRenderer, 20, 54);

	//render brush temperature
	brushTemperature.SetText(mainRenderer, std::to_string(currentBrushTemperature) + "c");
	brushTemperature.Draw(mainRenderer, 20, 88);

	//render selected particle type and temperature
	if (allParticles[mouse.x][mouse.y] != nullptr)
	{
		selectedParticleTemperature.SetText(mainRenderer, std::to_string(allParticles[mouse.x][mouse.y]->temperature) + "c");

		//check if source block
		if (allParticles[mouse.x][mouse.y]->type != TYPE_SOURCE)
			selectedParticleName.SetText(mainRenderer, typeNames[allParticles[mouse.x][mouse.y]->type]);
		else
		{
			Source* s = (Source*)allParticles[mouse.x][mouse.y];
			selectedParticleName.SetText(mainRenderer, typeNames[allParticles[mouse.x][mouse.y]->type] + typeNames[s->sourceType]);
		}
	}
	else
	{
		selectedParticleName.SetText(mainRenderer, "None");
		selectedParticleTemperature.SetText(mainRenderer, "0c");
	}

	selectedParticleName.Draw(mainRenderer, 20, 172);
	selectedParticleTemperature.Draw(mainRenderer, 20, 206);


	//render the current framerate
	currentFrameRate.SetText(mainRenderer, std::to_string(static_cast<int>(avgFPS)));
	currentFrameRate.Draw(mainRenderer, 20, WINDOW_HEIGHT - 34);

	//render the frame and increase the counter frames
	SDL_UnlockSurface(mainSurface);

	std::cout << avgFPS << "\n";
	

	SDL_RenderPresent(mainRenderer);
	countedFrames++;	
}