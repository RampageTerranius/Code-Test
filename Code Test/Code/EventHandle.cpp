#include "EventHandle.h"

//sdl
#include <SDL.h>

#include "ParticleTypes.h"
#include "Particles.h"

//used to switch the brush type back or forth automaticlaly
void SwitchBrushType(bool gotoNext)
{
	//temp variables used to determine how many particles there is and the current brush type
	int i = globals.currentBrushType;

	//if told to goto next brush
	if (gotoNext)
	{
		i++;

		//make sure we havent gone over the struct size
		if (i > TYPE_TOTALTYPES - 2)
			i = 0;

		//cast the int as particleType and update the current brush
		globals.currentBrushType = static_cast<ParticleType>(i);
	}
	else//if told to goto last brush type
	{
		i--;

		//make sure we havent gone under the struct size
		if (i < 0)
			i = TYPE_TOTALTYPES - 2;

		//cast the int as particleType and update the current brush
		globals.currentBrushType = static_cast<ParticleType>(i);
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
				globals.running = false;
				break;

			case SDLK_a:
				globals.keyboard.a = true;
				globals.currentBrushSize--;
				if (globals.currentBrushSize < 1)
					globals.currentBrushSize = 1;
				break;

			case SDLK_d:
				globals.keyboard.d = true;
				globals.currentBrushSize++;
				if (globals.currentBrushSize > globals.MAX_BRUSH_SIZE)
					globals.currentBrushSize = globals.MAX_BRUSH_SIZE;
				break;

			case SDLK_e:
				globals.keyboard.e = true;
				globals.createAsSource = !globals.createAsSource;
				break;

			case SDLK_w:
				globals.keyboard.w = true;
				SwitchBrushType(true);
				break;

			case SDLK_s:
				globals.keyboard.s = true;
				SwitchBrushType(false);
				break;

			case SDLK_h:
				globals.keyboard.h = true;
				globals.drawHeat = !globals.drawHeat;
				break;

			case SDLK_x:
				globals.keyboard.x = true;
				globals.loopScreen = !globals.loopScreen;
				break;

			case SDLK_MINUS:
				globals.keyboard.minus = true;
				break;

			case SDLK_PLUS:
				globals.keyboard.plus = true;
				break;

			case SDLK_LEFTBRACKET:
				globals.keyboard.leftBracket = true;
				if (globals.keyboard.lShift || globals.keyboard.rShift)
					globals.currentBrushTemperature -= 10;
				else
					globals.currentBrushTemperature--;
				break;

			case SDLK_RIGHTBRACKET:
				globals.keyboard.rightBracket = true;
				if (globals.keyboard.lShift || globals.keyboard.rShift)
					globals.currentBrushTemperature += 10;
				else
					globals.currentBrushTemperature++;
				break;

			case SDLK_LSHIFT:
				globals.keyboard.lShift = true;
				break;

			case SDLK_RSHIFT:
				globals.keyboard.rShift = true;
				break;

			case SDLK_SPACE:
				globals.keyboard.space = true;
				globals.pauseParticles = !globals.pauseParticles;
				break;
			}

			break;

		case SDL_KEYUP:
			switch (event.key.keysym.sym)
			{
			case SDLK_a:
				globals.keyboard.a = false;
				break;

			case SDLK_d:
				globals.keyboard.d = false;
				break;

			case SDLK_e:
				globals.keyboard.e = false;
				break;

			case SDLK_w:
				globals.keyboard.w = false;
				break;

			case SDLK_s:
				globals.keyboard.s = false;
				break;

			case SDLK_h:
				globals.keyboard.h = false;
				break;

			case SDLK_x:
				globals.keyboard.x = false;
				break;

			case SDLK_MINUS:
				globals.keyboard.minus = false;
				break;

			case SDLK_PLUS:
				globals.keyboard.plus = false;
				break;

			case SDLK_LSHIFT:
				globals.keyboard.lShift = false;
				break;

			case SDLK_RSHIFT:
				globals.keyboard.rShift = false;
				break;

			case SDLK_SPACE:
				globals.keyboard.space = false;
				break;
			}
			break;

			//on mouse movement
		case SDL_MOUSEMOTION:
			SDL_GetMouseState(&globals.mouse.x, &globals.mouse.y);
			if (globals.mouse.x < 0)
				globals.mouse.x = 0;
			if (globals.mouse.x > WINDOW_WIDTH - 1)
				globals.mouse.x = WINDOW_WIDTH - 1;
			if (globals.mouse.y < 0)
				globals.mouse.y = 0;
			if (globals.mouse.y > WINDOW_HEIGHT - 1)
				globals.mouse.y = WINDOW_HEIGHT - 1;
			break;

			//on mouse button down
		case SDL_MOUSEBUTTONDOWN:
			switch (event.button.button)
			{
			case SDL_BUTTON_LEFT:
				globals.mouse.left = true;
				break;

			case SDL_BUTTON_MIDDLE:
				globals.mouse.middle = true;
				break;

			case SDL_BUTTON_RIGHT:
				globals.mouse.right = true;
				break;
			}
			break;

			//on mouse button up
		case SDL_MOUSEBUTTONUP:
			switch (event.button.button)
			{
			case SDL_BUTTON_LEFT:
				globals.mouse.left = false;
				break;

			case SDL_BUTTON_MIDDLE:
				globals.mouse.middle = false;
				break;

			case SDL_BUTTON_RIGHT:
				globals.mouse.right = false;
				break;
			}
			break;
		}
	}
}

void CreateParticlesAtBrush(ParticleType type, int x, int y, float temperature)
{
	//TODO: setup a function to sort this automatically, currently doing it by hand. look towards the midpoint circle algorithm
	if (globals.currentBrushSize == 1)
		CreateParticle(type, globals.mouse.x, globals.mouse.y, temperature, globals.createAsSource);
	else for (int i = globals.currentBrushSize; i > -globals.currentBrushSize; i--)
		for (int n = globals.currentBrushSize; n > -globals.currentBrushSize; n--)
			CreateParticle(type, globals.mouse.x + i, globals.mouse.y + n, temperature, globals.createAsSource);
}

void DestroyParticlesAtBrush(int x, int y)
{
	//make sure we actually have a list first
	if (globals.particleList.size() == 0)
		return;

	//TODO: setup a function to sort this automatically, currently doing it by hand. look towards the midpoint circle algorithm
	if (globals.currentBrushSize == 1)
		DestroyParticle(globals.mouse.x, globals.mouse.y);
	else for (int i = globals.currentBrushSize; i > -globals.currentBrushSize; i--)
		for (int n = globals.currentBrushSize; n > -globals.currentBrushSize; n--)
			DestroyParticle(globals.mouse.x + i, globals.mouse.y + n);
}

//handles all keyboard/mouse events
void EventHandle(SDL_Event& event)
{
	UpdateEventStructs(event);//update all our structures handling what buttons are held down currently first

	//on left click paint particles using brush
	if (globals.mouse.left)
		CreateParticlesAtBrush(globals.currentBrushType, globals.mouse.x, globals.mouse.y, (float)globals.currentBrushTemperature);

	if (globals.mouse.right)
		DestroyParticlesAtBrush(globals.mouse.x, globals.mouse.y);
}