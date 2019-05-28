#pragma once

struct Keyboard
{
	bool w, s, a, d;
	bool plus, minus;
	bool leftBracket, rightBracket;
}keyboard;

struct Mouse
{
	bool left, middle, right;
	int x, y;
}mouse;

eType currentBrushType = TYPE_SAND;
int currentBrushSize = 1;

void UpdateEventStructs(SDL_Event event)
{
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				running = false;
				break;

			case SDLK_a:
				currentBrushType = TYPE_WALL;
				keyboard.a = true;
				break;

			case SDLK_d:
				keyboard.d = true;
				break;

			case SDLK_w:
				keyboard.w = true;
				currentBrushType = TYPE_WATER;
				break;

			case SDLK_s:
				keyboard.s = true;
				currentBrushType = TYPE_SAND;
				break;

			case SDLK_MINUS:
				keyboard.minus = true;
				break;

			case SDLK_PLUS:
				keyboard.plus = true;
				break;

			case SDLK_LEFTBRACKET:
				keyboard.leftBracket = true;
				currentBrushSize--;
				if (currentBrushSize < 1)
					currentBrushSize = 1;
				break;

			case SDLK_RIGHTBRACKET:
				keyboard.rightBracket = true;
				currentBrushSize++;
				if (currentBrushSize > MAX_BRUSH_SIZE)
					currentBrushSize = MAX_BRUSH_SIZE;
				break;
			}

			
			break;

		case SDL_KEYUP:
			switch (event.key.keysym.sym)
			{
			case SDLK_a:
				keyboard.a = false;
				break;

			case SDLK_d:
				keyboard.d = false;
				break;

			case SDLK_w:
				keyboard.w = false;
				break;

			case SDLK_s:
				keyboard.s = false;
				break;

			case SDLK_MINUS:
				keyboard.minus = false;
				break;

			case SDLK_PLUS:
				keyboard.plus = false;
				break;
			}
			break;

			//on mouse movement
		case SDL_MOUSEMOTION:
			SDL_GetMouseState(&mouse.x, &mouse.y);
			break;			

			//on mouse button down
		case SDL_MOUSEBUTTONDOWN:
			switch (event.button.button)
			{
			case SDL_BUTTON_LEFT:
				mouse.left = true;
				break;

			case SDL_BUTTON_MIDDLE:
				mouse.middle = true;
				break;

			case SDL_BUTTON_RIGHT:
				mouse.right = true;
				break;
			}
			break;

			//on mouse button up
		case SDL_MOUSEBUTTONUP:
			switch (event.button.button)
			{
			case SDL_BUTTON_LEFT:
				mouse.left = false;
				break;

			case SDL_BUTTON_MIDDLE:
				mouse.middle = false;
				break;

			case SDL_BUTTON_RIGHT:
				mouse.right = false;
				break;
			}
			break;
		}		
	}
}

void CreateParticles(eType type, int x, int y)
{
	//TODO: setup a function to sort this automatically, currently doing it by hand. look towards the midpoint circle algorithm

	for(int i = -currentBrushSize; i < currentBrushSize; i++)
		for (int n = -currentBrushSize; n < currentBrushSize; n++)
			CreateParticle(type, mouse.x + i, mouse.y + n);
}

void DestroyParticles(int x, int y)
{
	//make sure we actually have a list first
	if (particleList.size() == 0)
		return;

	//always destroy the first particle
	DestroyParticle(mouse.x, mouse.y);

	//TODO: setup a function to sort this automatically, currently doing it by hand. look towards the midpoint circle algorithm
	for (int i = -currentBrushSize; i < currentBrushSize; i++)
		for (int n = -currentBrushSize; n < currentBrushSize; n++)
			DestroyParticle(mouse.x + i, mouse.y + n);
}



void EventHandle(SDL_Event& event)
{
	UpdateEventStructs(event);//update all our structures handling what buttons are held down currently first

	//on left click paint particles using brush
	if (mouse.left)
		CreateParticles(currentBrushType, mouse.x, mouse.y);
	

	if (mouse.right)
		DestroyParticles(mouse.x, mouse.y);
}