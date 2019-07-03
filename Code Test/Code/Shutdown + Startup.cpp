#include "Shutdown + Startup.h"

#include <string>


//sdl
#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>

#include "Particles.h"

bool Setup()
{
	//initialize each SDL segment individually
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		std::cout << ("Unable to initialize SDL_VIDEO: %s", SDL_GetError());
		return false;
	}

	if (SDL_Init(SDL_INIT_AUDIO) != 0)
	{
		std::cout << ("Unable to initialize SDL_AUDIO: %s", SDL_GetError());
		return false;
	}

	if (SDL_Init(SDL_INIT_EVENTS) != 0)
	{
		std::cout << ("Unable to initialize SDL_EVENTS: %s", SDL_GetError());
		return false;
	}

	if (SDL_Init(SDL_INIT_TIMER) != 0)
	{
		std::cout << ("Unable to initialize SDL_TIMER: %s", SDL_GetError());
		return false;
	}

	if (TTF_Init() != 0)
	{
		std::cout << ("Unable to initialize TTF: %s", TTF_GetError());
		return false;
	}

	//create main window
	Uint32 windowFlags = SDL_WINDOW_OPENGL;
	globals.mainWindow = SDL_CreateWindow(globals.PROGRAM_NAME.c_str(), 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, windowFlags);

	Uint32 renderFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
	globals.mainRenderer = SDL_CreateRenderer(globals.mainWindow, -1, renderFlags);

	//setup ttf objects
	if (!globals.brushName.SetFont("pxl.ttf", 24))
	{
		std::cout << "Unable to load font pxl.ttf: " << TTF_GetError();
		return false;
	}

	if (!globals.brushSize.SetFont("pxl.ttf", 24))
	{
		std::cout << "Unable to load font pxl.ttf: " << TTF_GetError();
		return false;
	}
	if (!globals.brushTemperature.SetFont("pxl.ttf", 24))
	{
		std::cout << "Unable to load font pxl.ttf: " << TTF_GetError();
		return false;
	}
	if (!globals.selectedParticleName.SetFont("pxl.ttf", 24))
	{
		std::cout << "Unable to load font pxl.ttf: " << TTF_GetError();
		return false;
	}
	if (!globals.selectedParticleTemperature.SetFont("pxl.ttf", 24))
	{
		std::cout << "Unable to load font pxl.ttf: " << TTF_GetError();
		return false;
	}
	if (!globals.currentFrameRate.SetFont("pxl.ttf", 24))
	{
		std::cout << "Unable to load font pxl.ttf: " << TTF_GetError();
		return false;
	}

	//reserve the particlelist vector to the size of the screen
	globals.particleList.reserve(WINDOW_HEIGHT * WINDOW_WIDTH);

	return true;
}


void Shutdown()
{
	//destroy the main window before shutdown
	SDL_DestroyWindow(globals.mainWindow);

	//destroy the main renderer before shutdown
	SDL_DestroyRenderer(globals.mainRenderer);

	//clear up the pointer array used for the particles
	for (int i = 0; i < WINDOW_WIDTH - 1; i++)
		for (int n = 0; n < WINDOW_HEIGHT - 1; n++)
		{
			delete globals.allParticles[i][n];
			globals.allParticles[i][n] = nullptr;
		}

	//clean up all TTF files
	globals.brushName.Clear();
	globals.brushSize.Clear();
	globals.brushTemperature.Clear();
	globals.selectedParticleTemperature.Clear();
	globals.selectedParticleName.Clear();
	globals.currentFrameRate.Clear();

	SDL_Quit();
}
