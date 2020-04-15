#include "Render.h"

#include "TTF.h"
#include "Globals.h"
#include "Particles.h"
#include "Mouse.h"

#include <SDL.h>

void Render()
{
	// Lock the main surface for direct editing.
	SDL_LockSurface(mainSurface);

	// Set the surface to black.
	SDL_FillRect(mainSurface, nullptr, 0x000000);

	// Render all particles.
	for (int i = WINDOW_WIDTH - 1; i >= 0; i--)
		for (int n = WINDOW_HEIGHT - 1; n >= 0; n--)
			if (allParticles[i][n] != nullptr)
				allParticles[i][n]->Draw();
	// TODO: particles themselves have a switch in their Draw() function for handling view type
	// switch this so that we check the type ahead of time to help speed up the rendering a bit.

	SDL_UnlockSurface(mainSurface);

	SDL_Texture* pixelTexture = SDL_CreateTextureFromSurface(mainRenderer, mainSurface);
	SDL_RenderCopy(mainRenderer, pixelTexture, nullptr, nullptr);

	SDL_DestroyTexture(pixelTexture);

	// Once we have finished printing the particles we may print the UI overtop.
	
	// Render a box showing the brush location.
	if (renderBrush)
	{
		SDL_SetRenderDrawColor(mainRenderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);

		if (currentBrushSize == 1)
			SDL_RenderDrawPoint(mainRenderer, mouse.x, mouse.y);
		else
		{
			// Draw left side of box.
			for (int i = currentBrushSize; i > -currentBrushSize; i--)
				SDL_RenderDrawPoint(mainRenderer, mouse.x - currentBrushSize, mouse.y + i);

			// Draw right side of box.
			for (int i = currentBrushSize; i > -currentBrushSize; i--)
				SDL_RenderDrawPoint(mainRenderer, mouse.x + currentBrushSize, mouse.y + i);

			// Draw top of box.
			for (int i = currentBrushSize; i > -currentBrushSize; i--)
				SDL_RenderDrawPoint(mainRenderer, mouse.x + i, mouse.y - currentBrushSize);

			// Draw bottom of box.
			for (int i = currentBrushSize; i > -currentBrushSize; i--)
				SDL_RenderDrawPoint(mainRenderer, mouse.x + i, mouse.y + currentBrushSize);
		}
	}

	//render what type of brush is selected at the top left.
	if (!createAsSource)
		brushName.SetText(mainRenderer, ParticleTypes[currentBrushType].name);
	else
		brushName.SetText(mainRenderer, "Source - " + ParticleTypes[currentBrushType].name);
	brushName.Draw(mainRenderer, 20, 20);

	// Render what brush size is selected under that.
	brushSize.SetText(mainRenderer, std::to_string(currentBrushSize));
	brushSize.Draw(mainRenderer, 20, 54);

	// Render brush temperature.
	brushTemperature.SetText(mainRenderer, std::to_string(currentBrushTemperature) + "c");
	brushTemperature.Draw(mainRenderer, 20, 88);

	// Render selected particle type and temperature.
	if (allParticles[mouse.x][mouse.y] != nullptr)
	{
		selectedParticleTemperature.SetText(mainRenderer, std::to_string(allParticles[mouse.x][mouse.y]->temperature) + "c");

		// Check if source block.
		if (!allParticles[mouse.x][mouse.y]->isSource)
			selectedParticleName.SetText(mainRenderer, allParticles[mouse.x][mouse.y]->type->name);
		else		
			selectedParticleName.SetText(mainRenderer, "Source - " + allParticles[mouse.x][mouse.y]->type->name);
		

		selectedParticleHealth.SetText(mainRenderer, "HP: " + std::to_string(allParticles[mouse.x][mouse.y]->health) + "|" + std::to_string(allParticles[mouse.x][mouse.y]->type->startingHealth));
	}
	else
	{
		selectedParticleName.SetText(mainRenderer, "None");
		selectedParticleTemperature.SetText(mainRenderer, "0c");
		selectedParticleHealth.SetText(mainRenderer, "HP: 0|0");
	}

	selectedParticleName.Draw(mainRenderer, 20, 172);
	selectedParticleTemperature.Draw(mainRenderer, 20, 206);
	selectedParticleHealth.Draw(mainRenderer, 20, 240);


	// Render the current framerate.
	currentFrameRate.SetText(mainRenderer, std::to_string(static_cast<int>(avgFPS)));
	currentFrameRate.Draw(mainRenderer, 20, WINDOW_HEIGHT - 34);

	// Render the frame and increase the counter frames.
	std::cout << avgFPS << "\n";
	

	SDL_RenderPresent(mainRenderer);
	countedFrames++;	
}