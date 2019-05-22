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
	SDL_WindowFlags wFlags = SDL_WINDOW_OPENGL;
	mainWindow = SDL_CreateWindow(PROGRAM_NAME.c_str(), 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, wFlags);

	SDL_RendererFlags rFlags = SDL_RENDERER_ACCELERATED;
	mainRenderer = SDL_CreateRenderer(mainWindow, -1, rFlags);

	return true;
}
