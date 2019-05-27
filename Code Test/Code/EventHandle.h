#pragma once

struct Keyboard
{
	bool w, s, a, d;
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

void EventHandle(SDL_Event& event)
{
	UpdateEventStructs(event);//update all our structures handling what buttons are held down currently first

	if (mouse.left)
	{
		CreateParticle(currentBrushType, mouse.x, mouse.y);
		
		CreateParticle(currentBrushType, mouse.x-1, mouse.y);
		CreateParticle(currentBrushType, mouse.x+1, mouse.y);
		CreateParticle(currentBrushType, mouse.x, mouse.y-1);
		CreateParticle(currentBrushType, mouse.x, mouse.y+1);
	}

	if (mouse.right)
	{
		CreateParticle(TYPE_WALL, mouse.x, mouse.y);

		CreateParticle(TYPE_WALL, mouse.x - 1, mouse.y);
		CreateParticle(TYPE_WALL, mouse.x + 1, mouse.y);
		CreateParticle(TYPE_WALL, mouse.x, mouse.y - 1);
		CreateParticle(TYPE_WALL, mouse.x, mouse.y + 1);
	}	
}