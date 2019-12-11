#include "Shutdown + Startup.h"

#include "Particles.h"

#include <SDL.h>
#include <SDL_ttf.h>

#include <string>
#include <iostream>

bool Setup()
{
	// Initialize each SDL segment individually.
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

	// Create main window.
	Uint32 windowFlags = SDL_WINDOW_OPENGL;
	mainWindow = SDL_CreateWindow(PROGRAM_NAME.c_str(), 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, windowFlags);

	mainSurface = SDL_GetWindowSurface(mainWindow);

	Uint32 renderFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
	mainRenderer = SDL_CreateRenderer(mainWindow, -1, renderFlags);

	// Setup ttf objects.
	if (!brushName.SetFont("pxl.ttf", 24))
	{
		std::cout << "Unable to load font pxl.ttf: " << TTF_GetError();
		return false;
	}

	if (!brushSize.SetFont("pxl.ttf", 24))
	{
		std::cout << "Unable to load font pxl.ttf: " << TTF_GetError();
		return false;
	}
	if (!brushTemperature.SetFont("pxl.ttf", 24))
	{
		std::cout << "Unable to load font pxl.ttf: " << TTF_GetError();
		return false;
	}
	if (!selectedParticleName.SetFont("pxl.ttf", 24))
	{
		std::cout << "Unable to load font pxl.ttf: " << TTF_GetError();
		return false;
	}
	if (!selectedParticleTemperature.SetFont("pxl.ttf", 24))
	{
		std::cout << "Unable to load font pxl.ttf: " << TTF_GetError();
		return false;
	}
	if (!currentFrameRate.SetFont("pxl.ttf", 24))
	{
		std::cout << "Unable to load font pxl.ttf: " << TTF_GetError();
		return false;
	}

	// SPEED TESTING LINES
	// DISABLE THESE IF NOT SPEED TESTING
	for (int i = 0; i < WINDOW_WIDTH; i++)
		for (int n = 0; n < (WINDOW_HEIGHT / 2); n++)
			CreateParticle(TYPE_SAND, i, n, 30);

	return true;
}


void Shutdown()
{
	// Destroy the main window before shutdown.
	SDL_DestroyWindow(mainWindow);

	// Destroy the main renderer before shutdown.
	SDL_DestroyRenderer(mainRenderer);

	// Clear up the pointer array used for the particles.
	for (int i = 0; i < WINDOW_WIDTH - 1; i++)
		for (int n = 0; n < WINDOW_HEIGHT - 1; n++)
		{
			delete allParticles[i][n];
			allParticles[i][n] = nullptr;
		}

	// Clean up all TTF files.
	brushName.Clear();
	brushSize.Clear();
	brushTemperature.Clear();
	selectedParticleTemperature.Clear();
	selectedParticleName.Clear();
	currentFrameRate.Clear();

	// Destroy the surface used for drawing particles.
	SDL_FreeSurface(mainSurface);
	if (mainSurface != nullptr)
		mainSurface = nullptr;

	SDL_Quit();
}
