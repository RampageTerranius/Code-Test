#include "Render.h"

//sdl
#include <SDL.h>

#include "Particles.h"

void Render()
{
	//reset renderer
	SDL_SetRenderDrawColor(globals.mainRenderer, 0, 0, 0, 255);
	SDL_RenderClear(globals.mainRenderer);

	//render all particles	
	for (Particle* i : globals.particleList)
	{
		if (globals.drawHeat)
		{
			float tempHighest = 80;
			float tempMiddle = 0;
			float tempLowest = -50;

			int r, g, b;

			r = g = b = 20;

			float temp = i->temperature;

			if (i->temperature >= tempMiddle)
			{
				r = 255 * (int)(1.0 - (std::abs(i->temperature - tempHighest) / (i->temperature + tempHighest)));
				if (r > 255)
					r = 255;
			}
			else
			{
				b = 255 * (int)(std::abs(i->temperature - tempLowest) / (i->temperature + tempLowest));
				if (b > 255)
					b = 255;
			}

			SDL_SetRenderDrawColor(globals.mainRenderer, r, g, b, 0);
			SDL_RenderDrawPoint(globals.mainRenderer, i->point.x, i->point.y);
		}
		else
			i->Draw();

	}

	//render a box showing the brush location
	if (globals.renderBrush)
	{
		SDL_SetRenderDrawColor(globals.mainRenderer, 255, 255, 255, 0);

		if (globals.currentBrushSize == 1)
			SDL_RenderDrawPoint(globals.mainRenderer, globals.mouse.x, globals.mouse.y);
		else
		{
			//draw left side of box
			for (int i = globals.currentBrushSize; i > -globals.currentBrushSize; i--)
				SDL_RenderDrawPoint(globals.mainRenderer, globals.mouse.x - globals.currentBrushSize, globals.mouse.y + i);

			//draw right side of box
			for (int i = globals.currentBrushSize; i > -globals.currentBrushSize; i--)
				SDL_RenderDrawPoint(globals.mainRenderer, globals.mouse.x + globals.currentBrushSize, globals.mouse.y + i);

			//draw top of box
			for (int i = globals.currentBrushSize; i > -globals.currentBrushSize; i--)
				SDL_RenderDrawPoint(globals.mainRenderer, globals.mouse.x + i, globals.mouse.y - globals.currentBrushSize);

			//draw bottom of box
			for (int i = globals.currentBrushSize; i > -globals.currentBrushSize; i--)
				SDL_RenderDrawPoint(globals.mainRenderer, globals.mouse.x + i, globals.mouse.y + globals.currentBrushSize);
		}
	}

	//render what type of brush is selected at the top left

	if (!globals.createAsSource)
		globals.brushName.SetText(globals.mainRenderer, typeNames[globals.currentBrushType]);
	else
		globals.brushName.SetText(globals.mainRenderer, "Source - " + typeNames[globals.currentBrushType]);
	globals.brushName.Draw(globals.mainRenderer, 20, 20);

	//render what brush size is selected under that
	globals.brushSize.SetText(globals.mainRenderer, std::to_string(globals.currentBrushSize));
	globals.brushSize.Draw(globals.mainRenderer, 20, 54);

	//render brush temperature
	globals.brushTemperature.SetText(globals.mainRenderer, std::to_string(globals.currentBrushTemperature) + "c");
	globals.brushTemperature.Draw(globals.mainRenderer, 20, 88);

	//render selected particle type and temperature
	if (globals.allParticles[globals.mouse.x][globals.mouse.y] != nullptr)
	{
		globals.selectedParticleTemperature.SetText(globals.mainRenderer, std::to_string(globals.allParticles[globals.mouse.x][globals.mouse.y]->temperature) + "c");

		//check if source block
		if (globals.allParticles[globals.mouse.x][globals.mouse.y]->type != TYPE_SOURCE)
			globals.selectedParticleName.SetText(globals.mainRenderer, typeNames[globals.allParticles[globals.mouse.x][globals.mouse.y]->type]);
		else
		{
			Source* s = (Source*)globals.allParticles[globals.mouse.x][globals.mouse.y];
			globals.selectedParticleName.SetText(globals.mainRenderer, typeNames[globals.allParticles[globals.mouse.x][globals.mouse.y]->type] + typeNames[s->sourceType]);
		}
	}
	else
	{
		globals.selectedParticleName.SetText(globals.mainRenderer, "None");
		globals.selectedParticleTemperature.SetText(globals.mainRenderer, "0c");
	}

	globals.selectedParticleName.Draw(globals.mainRenderer, 20, 172);
	globals.selectedParticleTemperature.Draw(globals.mainRenderer, 20, 206);


	//render the current framerate
	globals.currentFrameRate.SetText(globals.mainRenderer, std::to_string(static_cast<int>(globals.avgFPS)));
	globals.currentFrameRate.Draw(globals.mainRenderer, 20, WINDOW_HEIGHT - 34);

	//render the frame and increase the counter frames
	SDL_RenderPresent(globals.mainRenderer);
	globals.countedFrames++;
}