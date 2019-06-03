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

ParticleType currentBrushType = TYPE_SAND;
int currentBrushSize = 4;

//used to switch the brush type back or forth automaticlaly
void SwitchBrushType(bool gotoNext)
{
	//temp variables used to determine how many particles there is and the current brush type
	int i = currentBrushType;

	//if told to goto next brush
	if (gotoNext)
	{
		i++;

		//make sure we havent gone over the struct size
		if (i > TYPE_TOTALTYPES - 1)	
			i = 0;		

		//cast the int as eType and update the current brush
		currentBrushType = static_cast<ParticleType>(i);
	}
	else//if told to goto last brush type
	{
		i--;

		//make sure we havent gone under the struct size
		if (i < 0)		
			i = TYPE_TOTALTYPES - 1;

		//cast the int as eType and update the current brush
		currentBrushType = static_cast<ParticleType>(i);
	}
}

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
				keyboard.leftBracket = true;
				currentBrushSize--;
				if (currentBrushSize < 1)
					currentBrushSize = 1;
				break;

			case SDLK_d:
				keyboard.d = true;
				keyboard.rightBracket = true;
				currentBrushSize++;
				if (currentBrushSize > MAX_BRUSH_SIZE)
					currentBrushSize = MAX_BRUSH_SIZE;
				break;

			case SDLK_w:
				keyboard.w = true;
				SwitchBrushType(true);
				break;

			case SDLK_s:
				keyboard.s = true;
				SwitchBrushType(false);
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

void CreateParticles(ParticleType type, int x, int y)
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