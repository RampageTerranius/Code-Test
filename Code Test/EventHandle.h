#pragma once

struct Keyboard
{
	bool w, s, a, d;
}keyboard;

void EventHandle(SDL_Event& event)
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
				break;

			case SDLK_s:
				keyboard.s = true;
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
		}
	}

	//handle keys being held down here
	if (keyboard.w && !playerData.hasJumped)
	{
		entityList.at(playerEntity).velY = -10;
		playerData.hasJumped = true;		
	}

	if (keyboard.s)	
		entityList.at(playerEntity).velY = 10;	

	if (keyboard.a)
	{
		entityList.at(playerEntity).velX -= 1;
		entityList.at(playerEntity).velXLock = true;
	}
	else
		entityList.at(playerEntity).velXLock = false;

	if (keyboard.d)
	{
		entityList.at(playerEntity).velX += 1;
		entityList.at(playerEntity).velXLock = true;
	}
	else
		entityList.at(playerEntity).velXLock = false;
}