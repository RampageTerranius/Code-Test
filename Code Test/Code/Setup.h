#pragma once

bool Setup()
{
	//initialize each SDL segment individually
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		SDL_Log("Unable to initialize SDL_VIDEO: %s", SDL_GetError());
		return false;
	}

	if (SDL_Init(SDL_INIT_AUDIO) != 0)
	{
		SDL_Log("Unable to initialize SDL_AUDIO: %s", SDL_GetError());
		return false;
	}

	if (SDL_Init(SDL_INIT_EVENTS) != 0)
	{
		SDL_Log("Unable to initialize SDL_EVENTS: %s", SDL_GetError());
		return false;
	}

	if (SDL_Init(SDL_INIT_TIMER) != 0)
	{
		SDL_Log("Unable to initialize SDL_TIMER: %s", SDL_GetError());
		return false;
	}

	//create main window
	Uint32 windowFlags = SDL_WINDOW_OPENGL;
	mainWindow = SDL_CreateWindow(PROGRAM_NAME.c_str(), 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, windowFlags);

	Uint32 renderFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
	mainRenderer = SDL_CreateRenderer(mainWindow, -1, renderFlags);

	return true;
}
