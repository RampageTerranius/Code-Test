#include "Particles.h"

#include <algorithm>
#include <iostream>

// https://codingforspeed.com/using-faster-psudo-random-generator-xorshift/
uint32_t xor128(void) {
	static uint32_t x = 123456789;
	static uint32_t y = 362436069;
	static uint32_t z = 521288629;
	static uint32_t w = 88675123;
	uint32_t t;
	t = x ^ (x << 11);
	x = y; y = z; z = w;
	return w = w ^ (w >> 19) ^ (t ^ (t >> 8));
}

// http://sdl.beuc.net/sdl.wiki/Pixel_Access
void EditPixel(int x, int y, Uint32 pixel)
{
	int bpp = mainSurface->format->BytesPerPixel;
	/* Here p is the address to the pixel we want to set */
	Uint8* p = (Uint8*)mainSurface->pixels + y * mainSurface->pitch + x * bpp;

	switch (bpp) {
	case 1:
		*p = pixel;
		break;

	case 2:
		*(Uint16*)p = pixel;
		break;

	case 3:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
			p[0] = (pixel >> 16) & 0xff;
			p[1] = (pixel >> 8) & 0xff;
			p[2] = pixel & 0xff;
		}
		else {
			p[0] = pixel & 0xff;
			p[1] = (pixel >> 8) & 0xff;
			p[2] = (pixel >> 16) & 0xff;
		}
		break;

	case 4:
		*(Uint32*)p = pixel;
		break;
	}
}

// Draws the heat for a particle at the given X/Y coordinates.
void DrawParticleHeat(int x, int y)
{	
	float tempHighest = 100;
	float tempLowest = -20;

	Particle* p = allParticles[x][y];

	float ratio = 2 * (p->temperature - tempLowest) / (tempHighest - tempLowest);

	int r, g, b;

	r = g = b = 0;

	if (p->temperature >= tempHighest)
		r = 255;
	else if (p->temperature <= tempLowest)
		b = 255;
	else
	{
		r = std::max(0, (int)(255 * (ratio - 1)));
		b = std::max(0, (int)(255 * (1 - ratio)));
		g = 255 - b - r;
	}

	EditPixel(x, y, SDL_MapRGB(mainSurface->format, r, g, b));	
}

Particle* allParticles[WINDOW_WIDTH][WINDOW_HEIGHT];
std::vector<ParticleType> ParticleTypes = std::vector<ParticleType>();

ParticleType* FindParticleType(std::string particleName)
{
	for (size_t i = 0; i < ParticleTypes.size(); i++)
		if (ParticleTypes[i].name == particleName)
			return &ParticleTypes[i];

	return nullptr;
}


void CreateParticle(std::string particleName, int x, int y, float temp, bool asSource)
{
	// Make sure were not trying to create an entity off screen.
	if (x < 0 || x >= WINDOW_WIDTH || y < 0 || y >= WINDOW_HEIGHT)
		return;

	// Check that we have no entity in this section to begin with.
	if (allParticles[x][y] != nullptr)
		return;

	for (auto & iterator : ParticleTypes)
	{
		if (iterator.name == particleName)
		{

			Particle* newParticle = new Particle(iterator, x, y, temp);

			newParticle->isSource = asSource;

			allParticles[x][y] = newParticle;
			return;
		}
	}
}

// Overload for default mode.
void CreateParticle(std::string particleName, int x, int y, float temp)
{
	CreateParticle(particleName, x, y, temp, false);
}

// Unlocks all particles that neighbour the given coordinate.
void UnlockNeighbourParticles(int x, int y)
{
	if (x < 0 || x >= WINDOW_WIDTH)
		return;

	if (y < 0 || y >= WINDOW_HEIGHT)
		return;

	int up, down, left, right;
	left = right = x;
	up = down = y;
	up--;
	down++;
	left--;
	right++;

	// Upwards.
	if (up >= 0)
	{
		if (allParticles[x][up] != nullptr)
			allParticles[x][up]->locked = false;

		// Up right.
		if (right < WINDOW_WIDTH)
			if (allParticles[right][up] != nullptr)
				allParticles[right][up]->locked = false;

		// Up left.
		if (left >= 0)
			if (allParticles[left][up] != nullptr)
				allParticles[left][up]->locked = false;
	}
		

	if (down < WINDOW_HEIGHT)
	{
		if (allParticles[x][down] != nullptr)
			allParticles[x][down]->locked = false;

		// Up right.
		if (right < WINDOW_WIDTH)
			if (allParticles[right][down] != nullptr)
				allParticles[right][down]->locked = false;

		// Up left.
		if (left >= 0)
			if (allParticles[left][down] != nullptr)
				allParticles[left][down]->locked = false;
	}

	if (left >= 0)
		if (allParticles[left][y] != nullptr)
			allParticles[left][y]->locked = false;	

	if (right < WINDOW_WIDTH)
		if (allParticles[right][y] != nullptr)
			allParticles[right][y]->locked = false;
}

// Destroys the particle at the given location and wipes the memory of it.
void DestroyParticle(int x, int y)
{
	if (x < 0 || x >= WINDOW_WIDTH)
		return;

	if (y < 0 || y >= WINDOW_HEIGHT)
		return;

	// Check if a particle even exists in the area we want to delete and cancel if there is none.
	if (allParticles[x][y] == nullptr)
		return;

	UnlockNeighbourParticles(x, y);

	delete allParticles[x][y];	
	allParticles[x][y] = nullptr;
}

// Used to move particles between two spots.
void MoveParticles(int x1, int y1, int x2, int y2)
{	
	if (x1 < 0 || x1 >= WINDOW_WIDTH || x2 < 0 || x2 >= WINDOW_WIDTH || y1 < 0 || y1 >= WINDOW_HEIGHT || y2 < 0 || y2 >= WINDOW_HEIGHT)
	{
		std::cout << "attempt to move particle that is out of range\n";
		return;
	}

	//get both the particles we want to move.
	std::swap(allParticles[x1][y1], allParticles[x2][y2]);


	// Update the internal location data for the first point now that its been moved.
	if (allParticles[x2][y2] != nullptr)
	{
		allParticles[x2][y2]->point.x = x2;
		allParticles[x2][y2]->point.y = y2;		
	}

	// Update the internal location data for the second point now that its been moved.
	if (allParticles[x1][y1] != nullptr)
	{
		allParticles[x1][y1]->point.x = x1;
		allParticles[x1][y1]->point.y = y1;
	}

	UnlockNeighbourParticles(x1, y1);
	UnlockNeighbourParticles(x2, y2);
}

// Attempts to even out temperatures between the two given points using the given thermal change value.
void EvenOutTemperatures(int x1, int y1, int x2, int y2, bool change1, bool change2)
{
	Particle* p1 = allParticles[x1][y1];
	Particle* p2 = allParticles[x2][y2];

	// Make sure both points are not blank.
	if (p1 != nullptr && p2 != nullptr)
	{
		// Make sure they arent already the same temp.
		if (p1->temperature == p2->temperature)
			return;		

		// Get the difference between both temperatures in an attempt to either increase or decrease the spread of temperature between objects.
		float calculatedTemperatureDifference = (std::abs(((p1->temperature - p2->temperature) / p1->temperature) * 100)) / temperatureDifferenceDivisor;

		float calculatedTemperatureChange = calculatedTemperatureDifference * std::abs(p1->type->thermalConductivity + p2->type->thermalConductivity);


		// Check if the temperature change will mean they both even each other out or not.
		if (std::abs(p1->temperature - p2->temperature) > calculatedTemperatureChange)
		{
			// First particle is higher temp then second.

			if (p1->temperature > p2->temperature)
			{			
				if (change1)
					p1->temperature -= calculatedTemperatureChange;
				if (change2)
					p2->temperature += calculatedTemperatureChange;
			}
			else// Second particle is higher temp then first.
			{
				if (change1)
					p1->temperature += calculatedTemperatureChange;
				if (change2)
					p2->temperature -= calculatedTemperatureChange;
			}
		}
		else// Both temperatures are close enough to just even them out at same temperature.
		{
			float temp = (p1->temperature + p2->temperature) / 2;
			if (change1)
				p1->temperature = temp;
			if (change2)
				p2->temperature = temp;
		}
	}
}

void EvenOutTemperatures(int x1, int y1, int x2, int y2)
{
	EvenOutTemperatures(x1, y1, x2, y2, true, true);
}

Particle::Particle(ParticleType& newType, int newX, int newY, float newTemperature)
{
	Reset();
	type = &newType;
	point.x = newX;
	point.y = newY;
	temperature = newTemperature;
	health = newType.startingHealth;
}

void Particle::Reset()
{
	type = nullptr;
	point.x = point.y = 0;	
	temperature = 0;
	health = 0;
	locked = false;
}

// Default draw code.
void Particle::Draw()
{
	switch (viewMode)
	{
	// Changes color depending on the type of pixel and its colortype settings.
	case VIEW_DEFAULT:
		switch (type->colorType)
		{
		case COLORTYPE_RGB:
			EditPixel(point.x, point.y, SDL_MapRGBA(mainSurface->format, type->R, type->G, type->B, 0));
			break;
		case COLORTYPE_HEAT:
			DrawParticleHeat(point.x, point.y);
			break;
		case COLORTYPE_ACTIVE:
			if (locked)
				EditPixel(point.x, point.y, SDL_MapRGBA(mainSurface->format, 255, 0, 0, 0));
			else
				EditPixel(point.x, point.y, SDL_MapRGBA(mainSurface->format, 255, 255, 255, 0));
			break;
		case COLORTYPE_RANDOMIZED:
			EditPixel(point.x, point.y, SDL_MapRGBA(mainSurface->format, (xor128() % 256), (xor128() % 256), (xor128() % 256), 0));
			break;
		}
		break;

	// Changes color depending on the heat of the pixel.
	case VIEW_HEAT:
		DrawParticleHeat(point.x, point.y);
		break;

	// Changes between white and red depending on if the particle is physics locked, white = unlocked red = locked.
	case VIEW_ACTIVE:
		if (locked)
			EditPixel(point.x, point.y, SDL_MapRGBA(mainSurface->format, 255, 0, 0, 0));
		else
			EditPixel(point.x, point.y, SDL_MapRGBA(mainSurface->format, 255, 255, 255, 0));
		break;
	}
}

// Default particle physics.
void Particle::HandlePhysics()
{
	// If this is a source block we want no physics simulated.
	if (isSource)
		return;

	switch (type->movementType)
	{
	case MOVEMENTTYPE_PILE:
	{
		int Down = point.y + 1;
		int Left = point.x - 1;
		int Right = point.x + 1;

		Particle* pDown = nullptr;
		Particle* pLeftDown = nullptr;
		Particle* pRightDown = nullptr;

		// Check if at the bottom of the screen first and update the temp pointers as needed.
		if (point.y == WINDOW_HEIGHT - 1)
		{
			// If we arent looping the screen then cancel all calcs from here as we are at the bottom of the screen already.
			if (!loopScreen)
			{
				locked = true;
				return;
			}

			pDown = allParticles[point.x][0];
			if (point.x > 0)
				pLeftDown = allParticles[Left][0];
			if (point.x < WINDOW_WIDTH - 1)
				pRightDown = allParticles[Right][0];
		}
		else
		{
			pDown = allParticles[point.x][Down];
			if (point.x > 0)
				pLeftDown = allParticles[Left][Down];
			if (point.x < WINDOW_WIDTH - 1)
				pRightDown = allParticles[Right][Down];
		}

		// If the particle directly under this is free then move downwards
		if (pDown == nullptr)
		{
			MoveParticles(point.x, point.y, point.x, Down);
			return;
		}
		// Are we on the left edge of the screen?
		else if (point.x == 0)
		{
			if (pRightDown == nullptr)
			{
				MoveParticles(point.x, point.y, Right, Down);
				return;
			}
			else
			{
				bool canGoDownRight, canGoDown;
				canGoDownRight = canGoDown = false;

				if (type->name != pDown->type->name)
					if (pDown->type->weight >= 0)
						if (type->weight < pDown->type->weight)
							canGoDown = true;

				if (type->name != pRightDown->type->name)
					if (pRightDown->type->weight >= 0)
						if (type->weight < pRightDown->type->weight)
							canGoDownRight = true;

				if (canGoDown && canGoDownRight)
				{
					int i = xor128() % 2;
					switch (i)
					{
					case 0:
						MoveParticles(point.x, point.y, point.x, Down);
						return;
					case 1:
						MoveParticles(point.x, point.y, Right, Down);
						return;
					}
				}
				else if (canGoDown)
				{
					MoveParticles(point.x, point.y, point.x, Down);
					return;
				}
				else if (canGoDownRight)
				{
					MoveParticles(point.x, point.y, Right, Down);
					return;
				}

				// On edge and cant move in any way. Lock particle.
				locked = true;
				return;
			}
		}
		// Are we on the right edge of the screen?
		else if (point.x == WINDOW_WIDTH - 1)
		{
			if (pLeftDown == nullptr)
			{
				MoveParticles(point.x, point.y, Left, Down);
				return;
			}
			else
			{
				bool canGoDownLeft, canGoDown;
				canGoDownLeft = canGoDown = false;

				if (type->name != pDown->type->name)
					if (pDown->type->weight >= 0)
						if (type->weight < pDown->type->weight)
							canGoDown = true;

				if (type->name != pLeftDown->type->name)
					if (pLeftDown->type->weight >= 0)
						if (type->weight < pLeftDown->type->weight)
							canGoDownLeft = true;

				if (canGoDown && canGoDownLeft)
				{
					int i = xor128() % 2;
					switch (i)
					{
					case 0:
						MoveParticles(point.x, point.y, point.x, Down);
						return;
					case 1:
						MoveParticles(point.x, point.y, Left, Down);
						return;
					}
				}
				else if (canGoDown)
				{
					MoveParticles(point.x, point.y, point.x, Down);
					return;
				}
				else if (canGoDownLeft)
				{
					MoveParticles(point.x, point.y, Left, Down);
					return;
				}

				// On edge and cant move in any way. Lock particle.
				locked = true;
				return;
			}
		}
		// We now know that we are unable to move directly down, and we are not on the left edge or right edge.
		// If both the left and right side are free.
		else if (pLeftDown == nullptr && pRightDown == nullptr)
		{
			int i = xor128() % 2;

			switch (i)
			{
			// Go left.
			case 0:
				MoveParticles(point.x, point.y, Left, Down);
				break;
			// Go right.
			case 1:
				MoveParticles(point.x, point.y, Right, Down);
				break;
			}

			return;
		}
		// At this point we know at least left or right is full, check which way we can go.
		else if (pLeftDown == nullptr)
		{
			MoveParticles(point.x, point.y, Left, Down);
			return;
		}
		else if (pRightDown == nullptr)
		{
			MoveParticles(point.x, point.y, Right, Down);
			return;
		}
		// At this point we know we can not move in any way downwards, check weights.
		else
		{	
			bool canGoDownLeft, canGoDownRight, canGoDown;
			canGoDownLeft = canGoDownRight = canGoDown = false;

			if (pDown != nullptr)
				if (type->name != pDown->type->name)
					if (pDown->type->weight >= 0)
						if (type->weight < pDown->type->weight)
							canGoDown = true;

			if (point.x > 0)
				if (pLeftDown != nullptr)
					if (type->name != pLeftDown->type->name)
						if (pLeftDown->type->weight >= 0)
							if (type->weight < pLeftDown->type->weight)
								canGoDownLeft = true;

			if (point.x < WINDOW_WIDTH - 1)
				if (pRightDown != nullptr)
					if (type->name != pRightDown->type->name)
						if (pRightDown->type->weight >= 0)
							if (type->weight < pRightDown->type->weight)
								canGoDownRight = true;

			// If can go any direction.
			if (canGoDownLeft && canGoDown && canGoDownRight)
			{
				int i = xor128() % 3;

				switch (i)
				{
				case 0:
					MoveParticles(point.x, point.y, Left, Down);
					break;
				case 1:
					MoveParticles(point.x, point.y, point.x, Down);
					break;
				case 2:
					MoveParticles(point.x, point.y, Right, Down);
					break;
				}

				return;
			}
			// If can go left or right.
			else if (canGoDownLeft && canGoDownRight)
			{
				int i = xor128() % 2;

				switch (i)
				{
				case 0:
					MoveParticles(point.x, point.y, Left, Down);
					break;
				case 1:
					MoveParticles(point.x, point.y, Right, Down);
					break;
				}

				return;
			}
			// If can go left or down.
			else if (canGoDownLeft && canGoDown)
			{
				int i = xor128() % 2;

				switch (i)
				{
				case 0:
					MoveParticles(point.x, point.y, Left, Down);
					break;
				case 1:
					MoveParticles(point.x, point.y, point.x, Down);
					break;
				}

				return;
			}
			// If can go right or down
			else if (canGoDownRight && canGoDown)
			{
				int i = xor128() % 2;

				switch (i)
				{
				case 0:
					MoveParticles(point.x, point.y, point.x, Down);
					break;
				case 1:
					MoveParticles(point.x, point.y, Right, Down);
					break;
				}

				return;
			}
			// We can only go in one way, check the way
			else
			{
				int i = -1;
				if (canGoDownLeft)
					i = 0;
				else if (canGoDownRight)
					i = 2;
				else if (canGoDown)
					i = 1;
				

				switch (i)
				{
				case 0:
					MoveParticles(point.x, point.y, Left, Down);
					return;
				case 1:
					MoveParticles(point.x, point.y, point.x, Down);
					return;
				case 2:
					MoveParticles(point.x, point.y, Right, Down);
					return;
				}

				//we were unable to move in any way shape or form. Lock the particle.
				locked = true;
			}
		}
	}
		break;
	case MOVEMENTTYPE_LIQUID:
	{
		int Down = point.y + 1;
		int Left = point.x - 1;
		int Right = point.x + 1;

		Particle* pDown = nullptr;
		Particle* pLeft = nullptr;
		Particle* pRight = nullptr;

		// Check if at the bottom of the screen first and update the temp pointers as needed.
		if (point.y == WINDOW_HEIGHT - 1)
		{
			// If we arent looping the screen then cancel all calcs from here as we are at the bottom of the screen already.
			if (!loopScreen)
			{
				locked = true;
				return;
			}

			pDown = allParticles[point.x][0];
			if (point.x > 0)
				pLeft = allParticles[Left][point.y];
			if (point.x < WINDOW_WIDTH - 1)
				pRight = allParticles[Right][point.y];
		}
		else
		{
			pDown = allParticles[point.x][Down];
			if (point.x > 0)
				pLeft = allParticles[Left][point.y];
			if (point.x < WINDOW_WIDTH - 1)
				pRight = allParticles[Right][point.y];
		}


		// If the particle directly under this is free then move downwards
		if (pDown == nullptr)
		{
			MoveParticles(point.x, point.y, point.x, Down);
			return;
		}
		// Are we on the left edge of the screen?
		else if (point.x == 0)
		{
			if (pRight == nullptr)
			{
				MoveParticles(point.x, point.y, Right, point.y);
				return;
			}
			else
			{
				bool canGoRight, canGoDown;
				canGoRight = canGoDown = false;

				if (type->name != pDown->type->name)
					if (pDown->type->weight >= 0)
						if (type->weight < pDown->type->weight)
							canGoDown = true;

				if (type->name != pRight->type->name)
					if (pRight->type->weight >= 0)
						if (type->weight < pRight->type->weight)
							canGoRight = true;

				if (canGoDown && canGoRight)
				{
					int i = xor128() % 2;
					switch (i)
					{
					case 0:
						MoveParticles(point.x, point.y, point.x, Down);
						return;
					case 1:
						MoveParticles(point.x, point.y, Right, point.y);
						return;
					}
				}
				else if (canGoDown)
				{
					MoveParticles(point.x, point.y, point.x, Down);
					return;
				}
				else if (canGoRight)
				{
					MoveParticles(point.x, point.y, Right, point.y);
					return;
				}

				// On edge and cant move in any way. Lock particle.
				locked = true;
				return;
			}
		}
		// Are we on the right edge of the screen?
		else if (point.x == WINDOW_WIDTH - 1)
		{
			if (pLeft == nullptr)
			{
				MoveParticles(point.x, point.y, Left, point.y);
				return;
			}
			else
			{
				bool canGoLeft, canGoDown;
				canGoLeft = canGoDown = false;

				if (type->name != pDown->type->name)
					if (pDown->type->weight >= 0)
						if (type->weight < pDown->type->weight)
							canGoDown = true;

				if (type->name != pLeft->type->name)
					if (pLeft->type->weight >= 0)
						if (type->weight < pLeft->type->weight)
							canGoLeft = true;

				if (canGoDown && canGoLeft)
				{
					int i = xor128() % 2;
					switch (i)
					{
					case 0:
						MoveParticles(point.x, point.y, point.x, Down);
						return;
					case 1:
						MoveParticles(point.x, point.y, Left, point.y);
						return;
					}
				}
				else if (canGoDown)
				{
					MoveParticles(point.x, point.y, point.x, Down);
					return;
				}
				else if (canGoLeft)
				{
					MoveParticles(point.x, point.y, Left, point.y);
					return;
				}

				// On edge and cant move in any way. Lock particle.
				locked = true;
				return;
			}
		}
		// We now know that we are unable to move directly down, and we are not on the left edge or right edge.
		// If both the left and right side are free.
		else if (pLeft == nullptr && pRight == nullptr)
		{
			int i = xor128() % 2;

			switch (i)
			{
				// Go left.
			case 0:
				MoveParticles(point.x, point.y, Left, point.y);
				break;
				// Go right.
			case 1:
				MoveParticles(point.x, point.y, Right, point.y);
				break;
			}

			return;
		}
		// At this point we know at least left or right is full, check which way we can go.
		else if (pLeft == nullptr)
		{
			MoveParticles(point.x, point.y, Left, point.y);
			return;
		}
		else if (pRight == nullptr)
		{
			MoveParticles(point.x, point.y, Right, point.y);
			return;
		}
		// At this point we know we can not move in any way downwards, check weights.
		else
		{
			bool canGoLeft, canGoRight, canGoDown;
			canGoLeft = canGoRight = canGoDown = false;

			if (pDown != nullptr)
				if (type->name != pDown->type->name)
					if (pDown->type->weight >= 0)
						if (type->weight < pDown->type->weight)
							canGoDown = true;

			if (point.x > 0)
				if (pLeft != nullptr)
					if (type->name != pLeft->type->name)
						if (pLeft->type->weight >= 0)
							if (type->weight < pLeft->type->weight)
								canGoLeft = true;

			if (point.x < WINDOW_WIDTH - 1)
				if (pRight != nullptr)
					if (type->name != pRight->type->name)
						if (pRight->type->weight >= 0)
							if (type->weight < pRight->type->weight)
								canGoRight = true;

			// If can go any direction.
			if (canGoLeft && canGoDown && canGoRight)
			{
				int i = xor128() % 3;

				switch (i)
				{
				case 0:
					MoveParticles(point.x, point.y, Left, point.y);
					break;
				case 1:
					MoveParticles(point.x, point.y, point.x, Down);
					break;
				case 2:
					MoveParticles(point.x, point.y, Right, point.y);
					break;
				}

				return;
			}
			// If can go left or right.
			else if (canGoLeft && canGoRight)
			{
				int i = xor128() % 2;

				switch (i)
				{
				case 0:
					MoveParticles(point.x, point.y, Left, point.y);
					break;
				case 1:
					MoveParticles(point.x, point.y, Right, point.y);
					break;
				}

				return;
			}
			// If can go left or down.
			else if (canGoLeft && canGoDown)
			{
				int i = xor128() % 2;

				switch (i)
				{
				case 0:
					MoveParticles(point.x, point.y, Left, point.y);
					break;
				case 1:
					MoveParticles(point.x, point.y, point.x, Down);
					break;
				}

				return;
			}
			// If can go right or down
			else if (canGoRight && canGoDown)
			{
				int i = xor128() % 2;

				switch (i)
				{
				case 0:
					MoveParticles(point.x, point.y, point.x, Down);
					break;
				case 1:
					MoveParticles(point.x, point.y, Right, point.y);
					break;
				}

				return;
			}
			// We can only go in one way, check the way
			else
			{
				int i = -1;
				if (canGoLeft)
					i = 0;
				else if (canGoRight)
					i = 2;
				else if (canGoDown)
					i = 1;


				switch (i)
				{
				case 0:
					MoveParticles(point.x, point.y, Left, point.y);
					return;
				case 1:
					MoveParticles(point.x, point.y, point.x, Down);
					return;
				case 2:
					MoveParticles(point.x, point.y, Right, point.y);
					return;
				}

				//we were unable to move in any way shape or form. Lock the particle.
				locked = true;
			}
		}

		/*
		// Make sure we arent already at the bottom level, if we are we dont need to check the physics.
		if (CheckIfAtBottom())
			return;

		// Check if an object exists under this one.
		if (DropParticle(point.x, point.y))
			return;

		int left, right, down;
		left = right = point.x;
		left--;
		right++;
		down = point.y + 1;

		if (left < 0)
			left = 0;

		if (right >= WINDOW_WIDTH)
			right = WINDOW_WIDTH - 1;

		Particle* pCenter = allParticles[point.x][point.y];
		Particle* pDown = allParticles[point.x][down];
		Particle* pLeft = allParticles[left][point.y];
		Particle* pRight = allParticles[right][point.y];

		// Check if we are surrounded.
		if (pLeft != nullptr && pDown != nullptr && pRight != nullptr)
		{
			// If we are check if we can move via gravity with the surrounding blocks.
			bool canGoLeft, canGoRight;
			canGoLeft = canGoRight = false;

			if (pLeft->type->weight != -1)
				if (pCenter->type->weight < pLeft->type->weight)
					canGoLeft = true;

			if (pRight->type->weight != -1)
				if (pCenter->type->weight < pRight->type->weight)
					canGoRight = true;

			// Can go either way.
			if (canGoLeft && canGoRight)
			{
				switch (xor128() % 2)
				{
					// Go left.
				case 0:
					MoveParticles(left, point.y, point.x, point.y);
					return;

					// Go right.
				case 1:
					MoveParticles(right, point.y, point.x, point.y);
					return;
				}
			}
			// Can only go left.
			else if (canGoLeft)
			{
				MoveParticles(left, point.y, point.x, point.y);
				return;
			}
			// Can only go right.
			else if (canGoRight)
			{
				MoveParticles(right, point.y, point.x, point.y);
				return;
			}

			// Neither way is less weight and we know that were surrounded, do not run any further code.
			return;
		}

		// There is an object under this one, try and move to the left or right.
		if (point.x == 0)// Check if we are on the left most edge.
		{
			// We are on the left most edge, try to drop to the bottom right.
			if (pRight == nullptr)
			{
				// Update particles.
				MoveParticles(point.x, point.y, right, point.y);
				return;
			}
			return;// Even if we cant drop, we know that we can not go anywhere so stop here.
		}

		if (point.x == WINDOW_WIDTH - 1)// Making sure were not at the right most edge.
		{
			// We are on the right most edge, try to drop to the bottom left.
			if (pLeft == nullptr)
			{
				// Update particles.
				MoveParticles(point.x, point.y, left, point.y);
				return;
			}
			return;// Even if we cant drop, we know that we can not go anywhere so stop here.
		}

		// If left is empty only.
		if (pLeft == nullptr && pRight != nullptr)
		{
			MoveParticles(point.x, point.y, left, point.y);
		}
		// If right is empty only.
		else if (pRight == nullptr && pLeft != nullptr)
		{
			MoveParticles(point.x, point.y, right, point.y);
		}
		// Both ways are free, randomly choose one and move.
		else if (pRight == nullptr && pLeft == nullptr)
		{
			switch (xor128() % 2)
			{
				// Go left.
			case 0:
				// Update particles.
				MoveParticles(point.x, point.y, left, point.y);
				return;

				// Go right.
			case 1:
				// Update particles.
				MoveParticles(point.x, point.y, right, point.y);
				return;
			}
		}*/
	}
		break;
	case MOVEMENTTYPE_AIRBORN:
	{
		/*
		int up, down, left, right;
		up = down = point.y;
		left = right = point.x;
		up--;
		down++;
		left--;
		right++;

		Particle* pCenter = allParticles[point.x][point.y];
		Particle* pLeft;
		Particle* pRight;

		int random = xor128() % 100 + 1;

		// Check if should ascend.
		if (random < type->ascendRate)
		{
			// Check if we are at the top of the screen and if we can loop around.
			if (!CheckIfAtTop())
				AscendParticle(point.x, point.y, false);
		}
		// Check if should descend.
		else if (random < (type->ascendRate + type->descendRate))
		{
			// Check if we are at the top of the screen and if we can loop around.
			if (!CheckIfAtBottom())
				DropParticle(point.x, point.y, false);
		}
		// Check if should go left or right.
		else if (random < (type->ascendRate + type->descendRate + type->sidewardsRate))
		{
			bool canGoLeft, canGoRight;
			canGoLeft = canGoRight = false;
			if (left >= 0)
			{
				pLeft = allParticles[left][point.y];
				if (pLeft == nullptr)
					canGoLeft = true;
				else
				{
					if (pLeft->type->weight != -1)
						if (pCenter->type->weight < pLeft->type->weight)
							canGoLeft = true;
				}
			}

			if (right < WINDOW_WIDTH)
			{
				pRight = allParticles[right][point.y];

				if (pRight == nullptr)
					canGoRight = true;
				else
				{
					if (pRight->type->weight != -1)
						if (pCenter->type->weight < pRight->type->weight)
							canGoRight = true;
				}
			}

			// Can go either way.
			if (canGoLeft && canGoRight)
			{
				switch (xor128() % 2)
				{
					// Go left.
				case 0:
					MoveParticles(left, point.y, point.x, point.y);
					break;

					// Go right.
				case 1:
					MoveParticles(right, point.y, point.x, point.y);
					break;
				}
			}
			// Can only go left.
			else if (canGoLeft)
			{
				MoveParticles(left, point.y, point.x, point.y);
			}
			// Can only go right.
			else
			{
				MoveParticles(right, point.y, point.x, point.y);
			}

			// Neither way is less weight and we know that were surrounded, do not run any further code.
			locked = true;
			return;
		}
		*/
	}
		break;
	}
}

void Particle::HandleEvents()
{
	// Values to help with determining what ways to calculate.
	int up, down, left, right;
	up = down = point.y;
	left = right = point.x;
	up--;
	down++;
	left--;
	right++;

	bool upExists, downExists, leftExists, rightExists;
	upExists = downExists = leftExists = rightExists = false;

	// Determine what directions around this particle we can work with.
	if (up >= 0)
		if (allParticles[point.x][up] != nullptr)
			upExists = true;

	if (down < WINDOW_HEIGHT)
		if (allParticles[point.x][down] != nullptr)
			downExists = true;

	if (left >= 0)
		if (allParticles[left][point.y] != nullptr)
			leftExists = true;

		if (right < WINDOW_WIDTH)
			if (allParticles[right][point.y] != nullptr)
				rightExists = true;

		if (!isSource)
		{
			try
			{
				if (type->particleEffects.size() > 0)			
					for (std::vector<ParticleEffect>::const_iterator iterator = type->particleEffects.begin(), end = type->particleEffects.begin(); iterator != end; ++iterator)
					switch ((*iterator).effectType)
					{
					case EFFECT_CHANGE_TYPE_AT_BELOW_TEMPERATURE:
						if (temperature < stof((*iterator).effectData[1]))
						{
							int newX = point.x;
							int newY = point.y;
							float newTemp = temperature;

							DestroyParticle(point.x, point.y);
							CreateParticle((*iterator).effectData[0], newX, newY, newTemp);
						}
						break;
					case EFFECT_CHANGE_TYPE_AT_ABOVE_TEMPERATURE:
						if (temperature > stof((*iterator).effectData[1]))
						{
							int newX = point.x;
							int newY = point.y;
							float newTemp = temperature;

							DestroyParticle(point.x, point.y);
							CreateParticle((*iterator).effectData[0], newX, newY, newTemp);
						}
						break;

					case EFFECT_DAMAGE_NEIGHBOURS:
					{
						int damageChance = std::stoi((*iterator).effectData[0]);

						if (upExists)
						{
							bool damagePixel = true;
							if ((*iterator).effectData.size() > 0)
								for (std::vector<std::string>::const_iterator dmgIterator = (*iterator).effectData.begin()++, end = (*iterator).effectData.end(); dmgIterator != end; ++dmgIterator)
									if (*dmgIterator == allParticles[point.x][up]->type->name)
									{
										damagePixel = false;
										break;
									}

							if (damagePixel)
								if ((xor128() % 100) < damageChance)
									allParticles[point.x][up]->health--;
						}

						if (downExists)
						{
							bool damagePixel = true;
							if ((*iterator).effectData.size() > 0)
								for (std::vector<std::string>::const_iterator dmgIterator = (*iterator).effectData.begin()++, end = (*iterator).effectData.end(); dmgIterator != end; ++dmgIterator)
									if (*dmgIterator == allParticles[point.x][down]->type->name)
									{
										damagePixel = false;
										break;
									}

							if (damagePixel)
								if ((xor128() % 100) < damageChance)
									allParticles[point.x][down]->health--;
						}

						if (leftExists)
						{
							bool damagePixel = true;
							if ((*iterator).effectData.size() > 0)
								for (std::vector<std::string>::const_iterator dmgIterator = (*iterator).effectData.begin()++, end = (*iterator).effectData.end(); dmgIterator != end; ++dmgIterator)
									if ((*dmgIterator) == allParticles[left][point.y]->type->name)
									{
										damagePixel = false;
										break;
									}

							if (damagePixel)
								if ((xor128() % 100) < damageChance)
									allParticles[left][point.y]->health--;
						}

						if (rightExists)
						{
							bool damagePixel = true;
							if ((*iterator).effectData.size() > 0)
								for (std::vector<std::string>::const_iterator dmgIterator = (*iterator).effectData.begin()++, end = (*iterator).effectData.end(); dmgIterator != end; ++dmgIterator)
									if ((*dmgIterator) == allParticles[right][point.y]->type->name)
									{
										damagePixel = false;
										break;
									}

							if (damagePixel)
								if ((xor128() % 100) < damageChance)
									allParticles[right][point.y]->health--;
						}
					}
					break;

					case EFFECT_SPREAD_TO_NEIGHBOURS:
					{
						int spreadChance = std::stoi((*iterator).effectData[0]);

						if (upExists)
						{
							bool spreadPixel = true;
							if ((*iterator).effectData.size() > 0)
								for (std::vector<std::string>::const_iterator dmgIterator = (*iterator).effectData.begin()++, end = (*iterator).effectData.end(); dmgIterator != end; ++dmgIterator)
									if ((*dmgIterator) == allParticles[point.x][up]->type->name)
									{
										spreadPixel = false;
										break;
									}

							if (spreadPixel)
								if ((xor128() % 100) < spreadChance)
									allParticles[point.x][up]->type = type;
						}

						if (downExists)
						{
							bool spreadPixel = true;
							if ((*iterator).effectData.size() > 0)
								for (std::vector<std::string>::const_iterator dmgIterator = (*iterator).effectData.begin()++, end = (*iterator).effectData.end(); dmgIterator != end; ++dmgIterator)
									if ((*dmgIterator) == allParticles[point.x][down]->type->name)
									{
										spreadPixel = false;
										break;
									}

							if (spreadPixel)
								if ((xor128() % 100) < spreadChance)
									allParticles[point.x][up]->type = type;
						}

						if (leftExists)
						{
							bool spreadPixel = true;
							if ((*iterator).effectData.size() > 0)
								for (std::vector<std::string>::const_iterator dmgIterator = (*iterator).effectData.begin()++, end = (*iterator).effectData.end(); dmgIterator != end; ++dmgIterator)
									if ((*dmgIterator) == allParticles[left][point.y]->type->name)
									{
										spreadPixel = false;
										break;
									}

							if (spreadPixel)
								if ((xor128() % 100) < spreadChance)
									allParticles[point.x][up]->type = type;
						}

						if (rightExists)
						{
							bool spreadPixel = true;
							if ((*iterator).effectData.size() > 0)
								for (std::vector<std::string>::const_iterator dmgIterator = (*iterator).effectData.begin()++, end = (*iterator).effectData.end(); dmgIterator != end; ++dmgIterator)
									if ((*dmgIterator) == allParticles[right][point.y]->type->name)
									{
										spreadPixel = false;
										break;
									}

							if (spreadPixel)
								if ((xor128() % 100) < spreadChance)
									allParticles[point.x][up]->type = type;
						}
					}
					break;

					case EFFECT_OVERRIDE_NEIGHBOUR_TYPE_WITH_SELF:
					{
						int overrideChance = std::stoi((*iterator).effectData[0]);

						if (upExists)
							if ((xor128() % 100) < overrideChance)
								allParticles[point.x][up]->type = type;


						if (downExists)
							if ((xor128() % 100) < overrideChance)
								allParticles[point.x][up]->type = type;


						if (leftExists)
							if ((xor128() % 100) < overrideChance)
								allParticles[point.x][up]->type = type;


						if (rightExists)
							if ((xor128() % 100) < overrideChance)
								allParticles[point.x][up]->type = type;
					}
					break;

					case EFFECT_MERGE_WITH_OTHER_PARTICLE_TYPE:
					{
						int mergeChance = std::stoi((*iterator).effectData[0]);
						if (downExists)
						{
							bool willMerge = true;

							if (mergeChance > 0)
								if ((xor128() % 100) >= mergeChance)
									willMerge = false;

							if (willMerge)
								if (allParticles[point.x][down]->type->name == (*iterator).effectData[1])
								{
									allParticles[point.x][down]->type = FindParticleType((*iterator).effectData[2]);
									DestroyParticle(point.x, point.y);
								}
						}

						if (upExists)
						{
							bool willMerge = true;

							if (mergeChance > 0)
								if ((xor128() % 100) >= mergeChance)
									willMerge = false;

							if (willMerge)
								if (allParticles[point.x][up]->type->name == (*iterator).effectData[1])
								{
									allParticles[point.x][up]->type = FindParticleType((*iterator).effectData[2]);
									DestroyParticle(point.x, point.y);
								}
						}

						if (leftExists)
						{
							bool willMerge = true;

							if (mergeChance > 0)
								if ((xor128() % 100) >= mergeChance)
									willMerge = false;

							if (willMerge)
								if (allParticles[left][point.y]->type->name == (*iterator).effectData[1])
								{
									allParticles[left][point.y]->type = FindParticleType((*iterator).effectData[2]);
									DestroyParticle(point.x, point.y);
								}
						}

						if (rightExists)
						{
							bool willMerge = true;

							if (mergeChance > 0)
								if ((xor128() % 100) >= mergeChance)
									willMerge = false;

							if (willMerge)
								if (allParticles[right][point.y]->type->name == (*iterator).effectData[1])
								{
									allParticles[right][point.y]->type = FindParticleType((*iterator).effectData[2]);
									DestroyParticle(point.x, point.y);
								}
						}
					}
						break;

					case EFFECT_BURN_NEIGHBOURS:
						if (upExists)
							if (allParticles[point.x][up]->type->Flammability > 0)
								if ((xor128() % 100) < allParticles[point.x][up]->type->Flammability)
									allParticles[point.x][up]->type = FindParticleType("Fire");

						if (downExists)
							if (allParticles[point.x][down]->type->Flammability > 0)
								if ((xor128() % 100) < allParticles[point.x][down]->type->Flammability)
									allParticles[point.x][down]->type = FindParticleType("Fire");

						if (leftExists)
							if (allParticles[left][point.y]->type->Flammability > 0)
								if ((xor128() % 100) < allParticles[left][point.y]->type->Flammability)
									allParticles[left][point.y]->type = FindParticleType("Fire");

						if (rightExists)
							if (allParticles[right][point.y]->type->Flammability > 0)
								if ((xor128() % 100) < allParticles[right][point.y]->type->Flammability)
									allParticles[right][point.y]->type = FindParticleType("Fire");
						break;


					case EFFECT_LOSE_HP_PER_TICK:
					{
						int hpLossChance = std::stoi((*iterator).effectData[0]);
						int hpLossPerChance = std::stoi((*iterator).effectData[1]);

						bool takeDamage = true;

						if (hpLossChance > 0)
							if (xor128() % 100 > hpLossChance)
								takeDamage = false;

						if (takeDamage)
							health--;
					}
					break;

					case EFFECT_NEIGHBOUR_PARTICLES_BECOME_SAME_TEMP_OVER_TIME:
						if (upExists)
							EvenOutTemperatures(point.x, point.y, point.x, up, false, true);

						if (downExists)
							EvenOutTemperatures(point.x, point.y, point.x, down, false, true);

						if (leftExists)
							EvenOutTemperatures(point.x, point.y, left, point.y, false, true);

						if (rightExists)
							EvenOutTemperatures(point.x, point.y, right, point.y, false, true);

						break;
					}
			}
			catch (std::exception e)
			{
				std::cout << "EventHandle error on particle :" + std::to_string(point.x) + " " + std::to_string(point.y);
			}

			// Temperature handle.
			if (type->thermalConductivity > 0)
			{
				// Check and update temperatures of each direction that we can.
				if (up >= 0)
					EvenOutTemperatures(point.x, point.y, point.x, up);

				if (down < WINDOW_HEIGHT)
					EvenOutTemperatures(point.x, point.y, point.x, down);

				if (left >= 0)
					EvenOutTemperatures(point.x, point.y, left, point.y);

				if (right < WINDOW_WIDTH)
					EvenOutTemperatures(point.x, point.y, right, point.y);
			}
		}
		else
		{
		// Particle is a source, it has no other events.
		if (up >= 0)
			CreateParticle(type->name, point.x, up, temperature);

		if (down < WINDOW_HEIGHT)
			CreateParticle(type->name, point.x, down, temperature);

		if (left >= 0)
			CreateParticle(type->name, left, point.y, temperature);

		if (right < WINDOW_WIDTH)
			CreateParticle(type->name, right, point.y, temperature);
		}
	

	// Health check.
	if (health <= 0)	
		DestroyParticle(point.x, point.y);
}