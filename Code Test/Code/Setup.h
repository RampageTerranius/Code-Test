#pragma once

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
	mainWindow = SDL_CreateWindow(PROGRAM_NAME.c_str(), 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, windowFlags);

	Uint32 renderFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
	mainRenderer = SDL_CreateRenderer(mainWindow, -1, renderFlags);

	//setup ttf objects
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
	if (!currentFrameRate.SetFont("pxl.ttf", 24))
	{
		std::cout << "Unable to load font pxl.ttf: " << TTF_GetError();
		return false;
	}

	return true;
}
