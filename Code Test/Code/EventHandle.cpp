#include "EventHandle.h"

#include "Mouse.h"
#include "Keyboard.h"
#include "Globals.h"
#include "Particles.h"

#include <SDL.h>

// Used to switch the brush type back or forth automatically.
void SwitchBrushType(bool gotoNext)
{
	// Temp variables used to determine how many particles there is and the current brush type.
	int i = currentBrushType;

	// If told to goto next brush.
	if (gotoNext)
	{
		i++;

		// Make sure we havent gone over the struct size.
		if (i > TYPE_TOTALTYPES - 2)
			i = 0;

		// Cast the int as particleType and update the current brush.
		currentBrushType = static_cast<ParticleType>(i);
	}
	else// If told to goto last brush type.
	{
		i--;

		// Make sure we havent gone under the struct size.
		if (i < 0)
			i = TYPE_TOTALTYPES - 2;

		// Cast the int as particleType and update the current brush.
		currentBrushType = static_cast<ParticleType>(i);
	}
}

// Handles all key/mouse events.
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
				currentBrushSize--;
				if (currentBrushSize < 1)
					currentBrushSize = 1;
				break;

			case SDLK_d:
				keyboard.d = true;
				currentBrushSize++;
				if (currentBrushSize > MAX_BRUSH_SIZE)
					currentBrushSize = MAX_BRUSH_SIZE;
				break;

			case SDLK_e:
				keyboard.e = true;
				createAsSource = !createAsSource;
				break;

			case SDLK_w:
				keyboard.w = true;
				SwitchBrushType(true);
				break;

			case SDLK_s:
				keyboard.s = true;
				SwitchBrushType(false);
				break;

			case SDLK_h:
				keyboard.h = true;
				drawHeat = !drawHeat;
				break;

			case SDLK_x:
				keyboard.x = true;
				loopScreen = !loopScreen;
				break;

			case SDLK_MINUS:
				keyboard.minus = true;
				break;

			case SDLK_PLUS:
				keyboard.plus = true;
				break;

			case SDLK_LEFTBRACKET:
				keyboard.leftBracket = true;
				if (keyboard.lShift || keyboard.rShift)
					currentBrushTemperature -= 10;
				else
					currentBrushTemperature--;
				break;

			case SDLK_RIGHTBRACKET:
				keyboard.rightBracket = true;
				if (keyboard.lShift || keyboard.rShift)
					currentBrushTemperature += 10;
				else
					currentBrushTemperature++;
				break;

			case SDLK_LSHIFT:
				keyboard.lShift = true;
				break;

			case SDLK_RSHIFT:
				keyboard.rShift = true;
				break;

			case SDLK_SPACE:
				keyboard.space = true;
				pauseParticles = !pauseParticles;
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

			case SDLK_e:
				keyboard.e = false;
				break;

			case SDLK_w:
				keyboard.w = false;
				break;

			case SDLK_s:
				keyboard.s = false;
				break;

			case SDLK_h:
				keyboard.h = false;
				break;

			case SDLK_x:
				keyboard.x = false;
				break;

			case SDLK_MINUS:
				keyboard.minus = false;
				break;

			case SDLK_PLUS:
				keyboard.plus = false;
				break;

			case SDLK_LSHIFT:
				keyboard.lShift = false;
				break;

			case SDLK_RSHIFT:
				keyboard.rShift = false;
				break;

			case SDLK_SPACE:
				keyboard.space = false;
				break;
			}
			break;

			// On mouse movement.
		case SDL_MOUSEMOTION:
			SDL_GetMouseState(&mouse.x, &mouse.y);
			if (mouse.x < 0)
				mouse.x = 0;
			if (mouse.x > WINDOW_WIDTH - 1)
				mouse.x = WINDOW_WIDTH - 1;
			if (mouse.y < 0)
				mouse.y = 0;
			if (mouse.y > WINDOW_HEIGHT - 1)
				mouse.y = WINDOW_HEIGHT - 1;
			break;

			// On mouse button down.
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

			// On mouse button up.
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

void CreateParticlesAtBrush(ParticleType type, int x, int y, float temperature)
{
	// TODO: setup a function to sort this automatically, currently doing it by hand. look towards the midpoint circle algorithm.
	// Check the size of the brush and create particles in the range given.
	if (currentBrushSize == 1)
		CreateParticle(type, mouse.x, mouse.y, temperature, createAsSource);
	else for (int i = currentBrushSize; i > -currentBrushSize; i--)
		for (int n = currentBrushSize; n > -currentBrushSize; n--)
			CreateParticle(type, mouse.x + i, mouse.y + n, temperature, createAsSource);
}

void DestroyParticlesAtBrush(int x, int y)
{
	// TODO: setup a function to sort this automatically, currently doing it by hand. look towards the midpoint circle algorithm.
	// Check the size of the brush and delete particles in range.
	if (currentBrushSize == 1)
		DestroyParticle(mouse.x, mouse.y);
	else for (int i = currentBrushSize + 1; i > -(currentBrushSize + 1); i--)
		for (int n = currentBrushSize + 1; n > -(currentBrushSize + 1); n--)
		{
			if ((i <= currentBrushSize && i >= -currentBrushSize) && (n <= currentBrushSize && n >= -currentBrushSize))
				DestroyParticle(mouse.x + i, mouse.y + n);// Delete all particles inside of brush
			else if ((i >= 0 && i <= WINDOW_WIDTH) && (n >= 0 && n <= WINDOW_HEIGHT))
				if (allParticles[i][n] != nullptr)
					allParticles[i][n]->locked = false;// Unlock all neighbouring particles of the brush.
		}

	
}

// Handles all keyboard/mouse events.
void EventHandle(SDL_Event& event)
{
	// Update all our structures handling what buttons are held down currently first.
	UpdateEventStructs(event);

	// On left click paint particles using brush.
	if (mouse.left)
		CreateParticlesAtBrush(currentBrushType, mouse.x, mouse.y, (float)currentBrushTemperature);

	// On right click delete particles using brush.
	if (mouse.right)
		DestroyParticlesAtBrush(mouse.x, mouse.y);
}