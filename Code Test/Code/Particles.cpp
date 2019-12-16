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

// Return celcius converted into kelvin.
float GetKelvin(float celcius)
{
	return celcius + 273.15f;
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

void CreateParticle(ParticleType type, int x, int y, float temp, bool asSource)
{
	// Make sure were not trying to create an entity off screen.
	if (x < 0 || x >= WINDOW_WIDTH || y < 0 || y >= WINDOW_HEIGHT)
		return;

	// Check that we have no entity in this section to begin with.
	if (allParticles[x][y] != nullptr)
	{
		// NOTE: these debug options cause MAJOR lag when adding particles, do nto use htem unless absolutely needed.
		//std::cout << "Attempt to create a particle of type " + typeNames[type] + " at " + std::to_string(x) + "|" + std::to_string(y) + " when a particle already exists here of type " + typeNames[allParticles[x][y]->type] + "\n";
		return;
	}	

	// Check what type of entity we need to create and assign it the required data.
	if (!asSource)
	{
		switch (type)
		{
		case TYPE_WALL:
			allParticles[x][y] = new Wall(x, y, temp);
			return;

		case TYPE_SAND:
			allParticles[x][y] = new Sand(x, y, temp);
			return;

		case TYPE_WATER:
			allParticles[x][y] = new Water(x, y, temp);
			return;

		case TYPE_ICE:
			allParticles[x][y] = new Ice(x, y, temp);
			return;

		case TYPE_THERMALFLUID:
			allParticles[x][y] = new ThermalFluid(x, y, temp);
			return;

		case TYPE_ACID:
			allParticles[x][y] = new Acid(x, y, temp);
			return;

		case TYPE_STEAM:
			allParticles[x][y] = new Steam(x, y, temp);
			return;

		case TYPE_PLANT:
			allParticles[x][y] = new Plant(x, y, temp);
			return;

		case TYPE_SALT:
			allParticles[x][y] = new Salt(x, y, temp);
			return;

		case TYPE_SALTWATER:
			allParticles[x][y] = new SaltWater(x, y, temp);
			return;

		case TYPE_SALTICE:
			allParticles[x][y] = new SaltIce(x, y, temp);
			return;

		case TYPE_GLITCH:
			allParticles[x][y] = new Glitch(x, y, temp);
			return;

		case TYPE_STONE:
			allParticles[x][y] = new Stone(x, y, temp);
			return;

		case TYPE_LAVA:
			allParticles[x][y] = new Lava(x, y, temp);
			return;

		case TYPE_FIRE:
			allParticles[x][y] = new Fire(x, y, temp);
			return;

		case TYPE_GAS:
			allParticles[x][y] = new Gas(x, y, temp);
			return;

		case TYPE_LIGHTGAS:
			allParticles[x][y] = new LightGas(x, y, temp);
			return;

		case TYPE_HEAVYGAS:
			allParticles[x][y] = new HeavyGas(x, y, temp);
			return;

		case TYPE_HEATPAD:
			allParticles[x][y] = new HeatPad(x, y, temp);
			return;

		default:
			// NOTE: these debug options cause MAJOR lag when adding particles, do nto use htem unless absolutely needed.
			//std::cout << "Attempt to create a unknown particle type at " + std::to_string(x) + "|" + std::to_string(y) + "\n";
			break;
		}
	}
	else
	{
		allParticles[x][y] = new Source(type, x, y, temp);
		return;
	}
}

// Overload for default mode.
void CreateParticle(ParticleType type, int x, int y, float temp)
{
	CreateParticle(type, x, y, temp, false);
}

// Destroys the particle at the given location and wipes the memory of it.
void DestroyParticle(int x, int y)
{
	// Check if a particle even exists in the area we want to delete and cancel if there is none.
	if (allParticles[x][y] == nullptr)
		return;

	delete allParticles[x][y];	
	allParticles[x][y] = nullptr;
}

// Used to move particles between two spots.
void MoveParticles(int x1, int y1, int x2, int y2)
{
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
}

// Unlocks all particles that neighbour the given coordinate.
void UnlockNeighbourParticles(int x, int y)
{
	int up, down, left, right;
	left = right = x;
	up = down = y;
	up--;
	down++;
	left--;
	right++;	

	if (up >= 0)
	{
		Particle* pUp = allParticles[x][up];
		if (pUp != nullptr)
			pUp->locked = false;
	}

	if (down < WINDOW_HEIGHT)
	{
		Particle* pDown = allParticles[x][down];
		if (pDown != nullptr)
			pDown->locked = false;
	}

	if (left >= 0)
	{
		Particle* pLeft = allParticles[left][y];
		if (pLeft != nullptr)
			pLeft->locked = false;
	}

	if (right < WINDOW_WIDTH)
	{
		Particle* pRight = allParticles[right][y];
		if (pRight != nullptr)
			pRight = false;
	}
}

// Attempt to drop the particle downwards.
bool DropParticle(int x, int y, bool randomize)
{
	int tempX = x;

	Particle* pDown = allParticles[tempX][y + 1];

	// Attempt to randomize the way it will move while going down.
	if (randomize)
	{
		tempX += ((xor128() % 3) - 1);

		if (tempX < 0)
			return false;

		if (tempX >= WINDOW_WIDTH)
			return false;
	}

	// If there is no particle directly below then just drop down.
	if (pDown == nullptr)
	{
		// Update the array.
		UnlockNeighbourParticles(x, y);
		MoveParticles(x, y, tempX, y + 1);
		return true;
	}
	// Other wise if the particle directly below has less weight then swap particles.
	else if (pDown->weight >= 0)
		if (allParticles[x][y]->weight > pDown->weight)
		{
			UnlockNeighbourParticles(x, y);
			MoveParticles(x, y, tempX, y + 1);			
			return true;
		}

	return false;
}

// Overload for legacy version of DropParticle.
bool DropParticle(int x, int y)
{
	return DropParticle(x, y, false);
}

// Attempt to ascend the particle directly upwards.
bool AscendParticle(int x, int y, bool randomize)
{
	int tempX = x;

	Particle* pUp = allParticles[tempX][y - 1];

	// Attempt to randomize the way it will move while going up.
	if (randomize)
	{
		tempX += ((xor128() % 3) - 1);

		if (tempX < 0)
			return false;

		if (tempX >= WINDOW_WIDTH)
			return false;
	}

	// If there is no particle directly above then just ascend.
	if (pUp == nullptr)
	{
		// Update the array.
		UnlockNeighbourParticles(x, y);
		MoveParticles(x, y, tempX, y - 1);
		return true;
	}
	// Other wise if the particle directly above has more weight then swap particles.
	else if (pUp->weight != -1)
		if (allParticles[x][y]->weight < pUp->weight)
		{
			UnlockNeighbourParticles(x, y);
			MoveParticles(x, y, tempX, y - 1);
			return true;
		}

	return false;
}

// Attempts to even out temperatures between the two given points using the given thermal change value.
void EvenOutTemperatures(int x1, int y1, int x2, int y2)
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

		float calculatedTemperatureChange = calculatedTemperatureDifference * std::abs(p1->thermalConductivity + p2->thermalConductivity);


		// Check if the temperature change will mean they both even each other out or not.
		if (std::abs(p1->temperature - p2->temperature) > calculatedTemperatureChange)
		{
			// First particle is higher temp then second.
			if (p1->temperature > p2->temperature)
			{
				if (p1->type != TYPE_HEATPAD)
					p1->temperature -= calculatedTemperatureChange;
				if (p2->type != TYPE_HEATPAD)
					p2->temperature += calculatedTemperatureChange;
			}
			else// Second particle is higher temp then first.
			{
				if (p1->type != TYPE_HEATPAD)
					p1->temperature += calculatedTemperatureChange;
				if (p2->type != TYPE_HEATPAD)
					p2->temperature -= calculatedTemperatureChange;
			}
		}
		else// Both temperatures are close enough to just even them out at same temperature.
		{
			float temp = (p1->temperature + p2->temperature) / 2;
			if (p1->type != TYPE_HEATPAD)
				p1->temperature = temp;
			if (p2->type != TYPE_HEATPAD)
				p2->temperature = temp;
		}
	}
}

Particle::Particle(ParticleType newType, int newX, int newY, float newTemperature)
{
	Reset();
	type = newType;
	point.x = newX;
	point.y = newY;
	temperature = newTemperature;
	thermalConductivity = settingThermalConductivity[type];
	health = settingHealth[type];
	weight = settingWeight[type];
}

void Particle::Reset()
{
	type = TYPE_WALL;
	point.x = point.y = 0;	
	temperature = 0;
	thermalConductivity = 0;
	health = 100;
	weight = -1;
	locked = false;
}

// Default draw code.
void Particle::Draw()
{
	if (!drawHeat)
		EditPixel(point.x, point.y, SDL_MapRGBA(mainSurface->format, settingColor[type][0], settingColor[type][1], settingColor[type][2], settingColor[type][3]));
	else
		DrawParticleHeat(point.x, point.y);
}

bool Particle::HandleEvents()
{
	// Temperature handle.
	if (thermalConductivity > 0)
	{
		// Values to help with determining what ways to calculate.
		int up, down, left, right;
		up = down = point.y;
		left = right = point.x;
		up--;
		down++;
		left--;
		right++;

		bool canGoUp, canGoDown, canGoLeft, canGoRight;
		canGoUp = canGoDown = canGoLeft = canGoRight = false;

		if (up >= 0)
			canGoUp = true;

		if (down < WINDOW_HEIGHT)
			canGoDown = true;

		if (left >= 0)
			canGoLeft = true;

		if (right < WINDOW_WIDTH)
			canGoRight = true;


		// Check and update each direction that we can.
		if (canGoUp)
			EvenOutTemperatures(point.x, point.y, point.x, up);

		if (canGoDown)
			EvenOutTemperatures(point.x, point.y, point.x, down);

		if (canGoLeft)
			EvenOutTemperatures(point.x, point.y, left, point.y);

		if (canGoRight)
			EvenOutTemperatures(point.x, point.y, right, point.y);
	}

	// Health check.
	if (health <= 0)
	{
		DestroyParticle(point.x, point.y);
		return true;
	}

	return false;
}

// Check if the particle is at the bottom of the screen and if we need to loop back to the top.
bool Particle::CheckIfAtBottom()
{
	// Check if at bottom of screen.
	if (point.y == WINDOW_HEIGHT - 1)
	{
		// Check if we need to loop the particles from bottom to top.
		if (loopScreen)
		{
			if (allParticles[point.x][0] == nullptr)
			{
				UnlockNeighbourParticles(point.x, point.y);
				MoveParticles(point.x, point.y, point.x, 0);
			}

			// Even if we cant move return true as we are stil lat the bottom and cant move else where.
			return true;
		}
		else
			return true;// We are not to loop, stop here.
	}

	// We are not at the bottom.
	return false;
}

// Check if the particle is at the top of the screen and if we need to loop back to the bottom.
bool Particle::CheckIfAtTop()
{
	// Check if at bottom of screen.
	if (point.y == 0)
	{
		// Check if we need to loop the particles from bottom to top.
		if (loopScreen)
		{
			if (allParticles[point.x][WINDOW_HEIGHT - 1] == nullptr)
			{
				UnlockNeighbourParticles(point.x, point.y);
				MoveParticles(point.x, point.y, point.x, WINDOW_HEIGHT - 1);
			}

			// Even if we cant move return true as we are still at the bottom and cant move else where.
			return true;
		}
		else
			return true;// We are not to loop, stop here.
	}

	// We are not at the bottom.
	return false;
}


// Default particle physics.
void Particle::HandlePhysics()
{
}

// Solid immobile particles.
// Particles that stay in spot and do not move at all.
SolidImmobile::SolidImmobile(ParticleType newType, int newX, int newY, float newTemperature) : Particle (newType, newX, newY, newTemperature)
{
}

bool SolidImmobile::HandleEvents()
{
	Particle::HandleEvents();

	return false;
}

// Solid mobile particles.
// Particles that drop downwards.
SolidMobile::SolidMobile(ParticleType newType, int newX, int newY, float newTemperature) : Particle(newType, newX, newY, newTemperature)
{

}

bool SolidMobile::HandleEvents()
{
	Particle::HandleEvents();

	return false;
}

void SolidMobile::HandlePhysics()
{

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
	Particle* pLeftDown = allParticles[left][down];
	Particle* pRightDown = allParticles[right][down];

	// Check if we are surrounded.
	if (pLeftDown != nullptr && pDown != nullptr && pRightDown != nullptr)
	{
		// If we are check if we can move via gravity with the surrounding blocks.

		bool canGoLeft, canGoRight;
		canGoLeft = canGoRight = false;

		if (pLeftDown->weight != -1)
			if (pCenter->weight < pLeftDown->weight)
				canGoLeft = true;

		if (pRightDown->weight != -1)
			if (pCenter->weight < pRightDown->weight)
				canGoRight = true;

		// Can go both ways, randomize the direction.
		if (canGoLeft && canGoRight)
		{
			switch (xor128() % 2)
			{
				// Go left.
			case 0:
				UnlockNeighbourParticles(point.x, point.y);
				MoveParticles(left, down, point.x, point.y);
				return;

				// Go right.
			case 1:
				UnlockNeighbourParticles(point.x, point.y);
				MoveParticles(right, down, point.x, point.y);
				return;
			}
		}
		// Can only go left.
		else if (canGoLeft)
		{
			UnlockNeighbourParticles(point.x, point.y);
			MoveParticles(left, down, point.x, point.y);
			return;
		}
		// Can only go roght.
		else if (canGoRight)
		{
			UnlockNeighbourParticles(point.x, point.y);
			MoveParticles(right, down, point.x, point.y);
			return;
		}

		// Neither way is less weight and we know that were surrounded, lock the particle and do not run any further code.
		locked = true;
		return;
	}

	// Since there is an object under this one lets check if we can fall to the left or right.
	if (point.x == 0)// Check if we are on the left most edge.
	{
		// We are on the left most edge, try to drop to the bottom right.
		if (pRightDown == nullptr)
		{
			// Update particles.
			UnlockNeighbourParticles(point.x, point.y);
			MoveParticles(point.x, point.y, right, down);
			return;
		}

		// Even if we cant drop, we know that we can not go anywhere so stop here and lock the particle.
		locked = true;
		return;
	}

	if (point.x == WINDOW_WIDTH - 1)// Making sure were not at the right most edge.
	{
		// We are on the right most edge, try to drop to the bottom left.
		if (pLeftDown == nullptr)
		{
			// Update particles.
			UnlockNeighbourParticles(point.x, point.y);
			MoveParticles(point.x, point.y, left, down);
			return;
		}

		// Even if we cant drop, we know that we can not go anywhere so stop here and lock the particle.
		locked = true;
		return;
	}

	// If the bottom left is empty and the bottom right isnt then drop to the bottom left.
	if (pLeftDown == nullptr && pRightDown != nullptr)
	{
		// Update particles.
		UnlockNeighbourParticles(point.x, point.y);
		MoveParticles(point.x, point.y, left, down);
		return;
	}
	// If the bottom right is empty and the bottom left isnt the drop to the bottom right.
	else if (pRightDown == nullptr && pLeftDown != nullptr)
	{
		// Update particles.
		UnlockNeighbourParticles(point.x, point.y);
		MoveParticles(point.x, point.y, right, down);
		return;
	}
	// If both sides are free randomly choose one direction and drop down that side.	
	else if (pRightDown == nullptr && pLeftDown == nullptr)
	{

		switch (xor128() % 2)
		{
			// Go left.
		case 0:
			// Update particles.
			UnlockNeighbourParticles(point.x, point.y);
			MoveParticles(point.x, point.y, left, down);
			return;

			// Go right.
		case 1:
			// Update particles.
			UnlockNeighbourParticles(point.x, point.y);
			MoveParticles(point.x, point.y, right, down);
			return;
		}
	}
}

// Liquid particles.
// Particles that drop downwards and attempt to flatten themselves out on surfaces.

Liquid::Liquid(ParticleType newType, int newX, int newY, float newTemperature) : Particle(newType, newX, newY, newTemperature)
{

}

bool Liquid::HandleEvents()
{
	Particle::HandleEvents();

	return false;
}

void Liquid::HandlePhysics()
{
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

		if (pLeft->weight != -1)
			if (pCenter->weight < pLeft->weight)
				canGoLeft = true;

		if (pRight->weight != -1)
			if (pCenter->weight < pRight->weight)
				canGoRight = true;

		// Can go either way.
		if (canGoLeft && canGoRight)
		{
			switch (xor128() % 2)
			{
				// Go left.
			case 0:
				UnlockNeighbourParticles(point.x, point.y);
				MoveParticles(left, point.y, point.x, point.y);
				return;

				// Go right.
			case 1:
				UnlockNeighbourParticles(point.x, point.y);
				MoveParticles(right, point.y, point.x, point.y);
				return;
			}
		} 
		// Can only go left.
		else if (canGoLeft)
		{
			UnlockNeighbourParticles(point.x, point.y);
			MoveParticles(left, point.y, point.x, point.y);
			return;
		}
		// Can only go right.
		else if (canGoRight)
		{
			UnlockNeighbourParticles(point.x, point.y);
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
			UnlockNeighbourParticles(point.x, point.y);
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
			UnlockNeighbourParticles(point.x, point.y);
			MoveParticles(point.x, point.y, left, point.y);
			return;
		}
		return;// Even if we cant drop, we know that we can not go anywhere so stop here.
	}

	// If left is empty only.
	if (pLeft == nullptr && pRight != nullptr)
	{
		UnlockNeighbourParticles(point.x, point.y);
		MoveParticles(point.x, point.y, left, point.y);
	}
	// If right is empty only.
	else if (pRight == nullptr && pLeft != nullptr)
	{
		UnlockNeighbourParticles(point.x, point.y);
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
			UnlockNeighbourParticles(point.x, point.y);
			MoveParticles(point.x, point.y, left, point.y);
			return;

			// Go right.
		case 1:
			// Update particles.
			UnlockNeighbourParticles(point.x, point.y);
			MoveParticles(point.x, point.y, right, point.y);
			return;
		}
	}
}

// Airborn particles.
// Particles that float around in the air either going directly up or slowly hovering around.
Airborn::Airborn(ParticleType newType, int newAscendRate, int newDescendRate, int newSidewardsRate, int newNoMovementRate, int newX, int newY, float newTemperature) : Particle(newType, newX, newY, newTemperature)
{
	ascendRate = newAscendRate;
	descendRate = newDescendRate;
	sidewardsRate = newSidewardsRate;
	noMovementRate = newNoMovementRate;
}

bool Airborn::HandleEvents()
{
	Particle::HandleEvents();

	return false;
}

void Airborn::HandlePhysics()
{
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

	int random = xor128() % 100;

	// Check if should ascend.
	if (random < ascendRate)
	{
		// Check if we are at the top of the screen and if we can loop around.
		if (!CheckIfAtTop())
			AscendParticle(point.x, point.y, false);
	}
	// Check if should descend.
	else if (random < (ascendRate + descendRate))
	{
		// Check if we are at the top of the screen and if we can loop around.
		if (!CheckIfAtBottom())
			DropParticle(point.x, point.y, false);
	}
	// Check if should go left or right.
	else if (random < (ascendRate + descendRate + sidewardsRate))
	{
		bool canGoLeft, canGoRight;
		canGoLeft = canGoRight = false;

		// Check if we are surrounded.
		if (left >= 0)
		{
			pLeft = allParticles[left][point.y];
			if (pLeft == nullptr)
				canGoLeft = true;
			else
			{
				if (pLeft->weight != -1)
					if (pCenter->weight < pLeft->weight)
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
				if (pRight->weight != -1)
					if (pCenter->weight < pRight->weight)
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
				UnlockNeighbourParticles(point.x, point.y);
				MoveParticles(left, point.y, point.x, point.y);
				break;

				// Go right.
			case 1:
				UnlockNeighbourParticles(point.x, point.y);
				MoveParticles(right, point.y, point.x, point.y);
				break;
			}
		}
		// Can only go left.
		else if (canGoLeft)
		{
			UnlockNeighbourParticles(point.x, point.y);
			MoveParticles(left, point.y, point.x, point.y);
		}
		// Can only go right.
		else
		{
			UnlockNeighbourParticles(point.x, point.y);
			MoveParticles(right, point.y, point.x, point.y);
		}		

		// Neither way is less weight and we know that were surrounded, do not run any further code.
		locked = true;
		return;	
	}	
}

// Wall particles.
Wall::Wall(int newX, int newY, float newTemperature) : SolidImmobile(TYPE_WALL, newX, newY, newTemperature)
{	
}

// Sand particles.
Sand::Sand(int newX, int newY, float newTemperature) : SolidMobile(TYPE_SAND, newX, newY, newTemperature)
{	
}

// Water particles.
Water::Water(int newX, int newY, float newTemperature) : Liquid(TYPE_WATER, newX, newY, newTemperature)
{
}

bool Water::HandleEvents()
{
	// Handle default events, this includes handling temperature shifts.
	if (Particle::HandleEvents())
		return true;

	// Check if the water should freeze.
	if (temperature < waterFreezePoint)
	{
		int newX = point.x;
		int newY = point.y;
		float newTemp = temperature;

		DestroyParticle(point.x, point.y);
		CreateParticle(TYPE_ICE, newX, newY, newTemp);

		return true;
	}
	// Check if should turn into steam.
	else if (temperature > waterBoilIntoSteamPoint)
	{
		int newX = point.x;
		int newY = point.y;
		float newTemp = temperature;

		DestroyParticle(point.x, point.y);
		CreateParticle(TYPE_STEAM, newX, newY, newTemp);

		return true;
	}

	return false;
}

// Ice particles.
Ice::Ice(int newX, int newY, float newTemperature) : SolidImmobile(TYPE_ICE, newX, newY, newTemperature)
{
}

bool Ice::HandleEvents()
{
	// Handle default events first, this includes temperature shifting.
	if (Particle::HandleEvents())
		return true;

	// Check if ice should melt.
	if (temperature > iceMeltPoint)
	{
		int newX = point.x;
		int newY = point.y;
		float newTemp = temperature;

		DestroyParticle(point.x, point.y);
		CreateParticle(TYPE_WATER, newX, newY, newTemp);
		return true;
	}

	return false;
}

ThermalFluid::ThermalFluid(int newX, int newY, float newTemperature) : Liquid(TYPE_THERMALFLUID, newX, newY, newTemperature)
{
}

// Acid particles.
Acid::Acid(int newX, int newY, float newTemperature) : Liquid(TYPE_ACID, newX, newY, newTemperature)
{
}

// Acid needs to randomly damage other blocks around it that are not acid.
bool Acid::HandleEvents()
{
	if (Particle::HandleEvents())
		return true;

	// Make sure an object exists first.
	if (this == nullptr)
		return true;

	// Values to help with determining what ways to calculate.
	int up, down, left, right;
	up = down = point.y;
	left = right = point.x;
	up--;
	down++;
	left--;
	right++;

	Particle* pUp;
	Particle* pDown;
	Particle* pLeft;
	Particle* pRight;

	// Get our randomized chance ahead of time.
	// Acid is fine to use a single burn chance around it, users will rarely notice that it doesnt randomly damage each surrounding on different intervals.
	int random = xor128() % 100;

	// Check if blocks exist in each direction and attempt to target it for damage.
	if (up >= 0)
	{
		pUp = allParticles[point.x][up];
		if (pUp != nullptr)
			if (pUp->type != TYPE_ACID)
				if (random <= acidDamageChance)
				{
					pUp->health--;
					health--;
				}
	}

	if (down < WINDOW_HEIGHT)
	{
		pDown = allParticles[point.x][down];
		if (pDown != nullptr)
			if (pDown->type != TYPE_ACID)
				if (random <= acidDamageChance)
				{
					pDown->health--;
					health--;
				}
	}

	if (left >= 0)
	{
		pLeft = allParticles[left][point.y];
		if (pLeft != nullptr)
			if (pLeft->type != TYPE_ACID)
				if (random <= acidDamageChance)
				{
					pLeft->health--;
					health--;
				}
	}

	if (right < WINDOW_WIDTH)
	{
		pRight = allParticles[right][point.y];
		if (pRight != nullptr)
			if (pRight->type != TYPE_ACID)
				if (random <= acidDamageChance)
				{
					pRight->health--;
					health--;
				}
	}

	return false;
}

// Steam particles.
Steam::Steam(int newX, int newY, float newTemperature) : Airborn(TYPE_STEAM, steamAscendRate, steamDescendRate, steamSidewardsRate, steamNoMovementRate, newX, newY, newTemperature)
{
}

bool Steam::HandleEvents()
{
	// Handle default events first, this includes temperature shifting.
	if (Particle::HandleEvents())
		return true;

	if (temperature < steamCondensationPoint)
	{
		int newX = point.x;
		int newY = point.y;
		float newTemp = temperature;

		DestroyParticle(point.x, point.y);
		CreateParticle(TYPE_WATER, newX, newY, newTemp);
	}

	return false;
}

// Plant particles.
Plant::Plant(int newX, int newY, float newTemperature) : SolidImmobile(TYPE_PLANT, newX, newY, newTemperature)
{
}

bool Plant::HandleEvents()
{
	// Handle default events first.
	if (Particle::HandleEvents())
		return true;

	int up, down, left, right;

	up = down = point.y;
	left = right = point.x;

	up--;
	down++;
	left--;
	right++;
	   

	bool canGoUp, canGoDown, canGoLeft, canGoRight;
	canGoUp = canGoDown = canGoLeft = canGoRight = false;

	Particle* pUp;
	Particle* pDown;
	Particle* pLeft;
	Particle* pRight;

	// Check if can spread upwards.
	if (up >= 0)
	{
		pUp = allParticles[point.x][up];
		if (pUp != nullptr)
			if (pUp->type == TYPE_WATER)
				if ((xor128() % 100) <= (uint32_t)plantSpreadChance)
					canGoUp = true;
	}

		// Check if can spread downwards.
	if (down < WINDOW_HEIGHT)
	{
		pDown = allParticles[point.x][down];
		if (pDown != nullptr)
			if (pDown->type == TYPE_WATER)
				if ((xor128() % 100) <= (uint32_t)plantSpreadChance)
					canGoDown = true;
	}
	


	// Check if can spread left.
	if (left >= 0)
	{
		pLeft = allParticles[left][point.y];
		if (pLeft != nullptr)
			if (pLeft->type == TYPE_WATER)
				if ((xor128() % 100) <= (uint32_t)plantSpreadChance)
					canGoLeft = true;
	}
			

	// Check if can spread right.
	if (right < WINDOW_WIDTH)
	{
		pRight = allParticles[right][point.y];
		if (pRight != nullptr)
			if (pRight->type == TYPE_WATER)
				if ((xor128() % 100) <= (uint32_t)plantSpreadChance)
					canGoRight = true;
	}
			
	

	// Check what directions we can spread and spread in those directions.
	if (canGoUp)
	{
		int newX = point.x;
		int newY = up;
		float newTemp = temperature;

		DestroyParticle(point.x, up);
		CreateParticle(TYPE_PLANT, newX, newY, newTemp);
	}

	if (canGoDown)
	{
		int newX = point.x;
		int newY = down;
		float newTemp = temperature;

		DestroyParticle(point.x, down);
		CreateParticle(TYPE_PLANT, newX, newY, newTemp);
	}

	if (canGoLeft)
	{
		int newX = left;
		int newY = point.y;
		float newTemp = temperature;

		DestroyParticle(left, point.y);
		CreateParticle(TYPE_PLANT, newX, newY, newTemp);
	}

	if (canGoRight)
	{
		int newX = right;
		int newY = point.y;
		float newTemp = temperature;

		DestroyParticle(right, point.y);
		CreateParticle(TYPE_PLANT, newX, newY, newTemp);
	}

	if (canGoUp || canGoDown || canGoLeft || canGoRight)
		return true;
	else
		return false;
}

// Salt particle.
Salt::Salt(int newX, int newY, float newTemperature) : SolidMobile(TYPE_SALT, newX, newY, newTemperature)
{
}

bool Salt::HandleEvents()
{
	// Default event handling.
	if (Particle::HandleEvents())
		return true;

	// Salt event handling.
	int up, down, left, right;

	up = down = point.y;
	left = right = point.x;

	up--;
	down++;
	left--;
	right++;

	Particle* pCenter = allParticles[point.x][point.y];
	Particle* pUp;
	Particle* pDown;
	Particle* pLeft;
	Particle* pRight;

	// Up.
	if (up >= 0)
	{
		pUp = allParticles[point.x][up];
		if (pUp != nullptr)
			if (pUp->type == TYPE_WATER)
			{
				// Get some temporary variables as both particles will be deleted soon and we do not want to possibly call upon a deleted object.
				float temp = (pUp->temperature + pCenter->temperature) / 2;
				int x1, y1, x2, y2;

				x1 = point.x;
				y1 = point.y;
				x2 = point.x;
				y2 = up;

				DestroyParticle(x1, y1);
				DestroyParticle(x2, y2);

				CreateParticle(TYPE_SALTWATER, x2, y2, temp);
				return true;
			}
	}

	// Down.
	if (down < WINDOW_HEIGHT)
	{
		pDown = allParticles[point.x][down];
		if (pDown != nullptr)
			if (pDown->type == TYPE_WATER)
			{
				// Get some temporary variables as both particles will be deleted soon and we do not want to possibly call downon a deleted object.
				float temp = (pDown->temperature + pCenter->temperature) / 2;
				int x1, y1, x2, y2;

				x1 = point.x;
				y1 = point.y;
				x2 = point.x;
				y2 = down;

				DestroyParticle(x1, y1);
				DestroyParticle(x2, y2);

				CreateParticle(TYPE_SALTWATER, x2, y2, temp);
				return true;
			}
	}

	// Left.
	if (left >= 0)
	{
		pLeft = allParticles[left][point.y];
		if (pLeft != nullptr)
			if (pLeft->type == TYPE_WATER)
			{
				// Get some temporary variables as both particles will be deleted soon and we do not want to possibly call upon a deleted object.
				float temp = (pLeft->temperature + pCenter->temperature) / 2;
				int x1, y1, x2, y2;

				x1 = point.x;
				y1 = point.y;
				x2 = left;
				y2 = point.y;

				DestroyParticle(x1, y1);
				DestroyParticle(x2, y2);

				CreateParticle(TYPE_SALTWATER, x2, y2, temp);
				return true;
			}
	}

	//Right.
	if (right < WINDOW_WIDTH)
	{
		pRight = allParticles[right][point.y];
		if (pRight != nullptr)
			if (pRight->type == TYPE_WATER)
			{
				// Get some temporary variables as both particles will be deleted soon and we do not want to possibly call upon a deleted object.
				float temp = (pRight->temperature + pCenter->temperature) / 2;
				int x1, y1, x2, y2;

				x1 = point.x;
				y1 = point.y;
				x2 = right;
				y2 = point.y;

				DestroyParticle(x1, y1);
				DestroyParticle(x2, y2);

				CreateParticle(TYPE_SALTWATER, x2, y2, temp);
				return true;
			}
	}

	return false;
}

// Salt water particle.
SaltWater::SaltWater(int newX, int newY, float newTemperature) : Liquid(TYPE_SALTWATER, newX, newY, newTemperature)
{
}

bool SaltWater::HandleEvents()
{
	// Handle default events, this includes handling temperature shifts.
	if (Particle::HandleEvents())
		return true;

	// Check if the salt water should freeze.
	if (temperature < (waterFreezePoint * saltWaterEventTempMultiplier))
	{
		int newX = point.x;
		int newY = point.y;
		float newTemp = temperature;

		DestroyParticle(point.x, point.y);
		CreateParticle(TYPE_SALTICE, newX, newY, newTemp);

		return true;
	}
	// Check if should turn into steam.
	else if (temperature > (waterBoilIntoSteamPoint * saltWaterEventTempMultiplier))
	{
		int newX = point.x;
		int newY = point.y;
		float newTemp = temperature;

		DestroyParticle(point.x, point.y);
		CreateParticle(TYPE_STEAM, newX, newY, newTemp);

		return true;
	}

	return false;
}

// Salt ice particles.
SaltIce::SaltIce(int newX, int newY, float newTemperature) : SolidImmobile(TYPE_SALTICE, newX, newY, newTemperature)
{
}

bool SaltIce::HandleEvents()
{
	// Handle default events first, this includes temperature shifting.
	if (Particle::HandleEvents())
		return true;

	// Check if ice should melt.
	if (temperature > (iceMeltPoint * saltWaterEventTempMultiplier))
	{
		int newX = point.x;
		int newY = point.y;
		float newTemp = temperature;

		DestroyParticle(point.x, point.y);
		CreateParticle(TYPE_SALTWATER, newX, newY, newTemp);
		return true;
	}

	return false;
}

// Source particles.
// Creates copies of its self in the surrounding blocks that are empty.
Source::Source(ParticleType newSourceType, int newX, int newY, float newTemperature) : SolidImmobile(TYPE_SOURCE, newX, newY, newTemperature)
{
	sourceType = newSourceType;
}
void Source::Draw()
{
	EditPixel(point.x, point.y, SDL_MapRGBA(mainSurface->format, settingColor[sourceType][0], settingColor[sourceType][1], settingColor[sourceType][2], settingColor[sourceType][3]));
}

bool Source::HandleEvents()
{
	if (Particle::HandleEvents())
		return true;

	// Source event handle.
	// Attempt to create new particles around this particle using its sourceType value.

	// Values to help with determining what ways to calculate.
	int up, down, left, right;
	up = down = point.y;
	left = right = point.x;
	up--;
	down++;
	left--;
	right++;

	if (up >= 0)
		if (allParticles[point.x][up] == nullptr)
			CreateParticle(sourceType, point.x, up, temperature);

	if (down < WINDOW_HEIGHT)
		if (allParticles[point.x][down] == nullptr)
			CreateParticle(sourceType, point.x, down, temperature);

	if (left >= 0)
		if (allParticles[left][point.y] == nullptr)
			CreateParticle(sourceType, left, point.y, temperature);

	if (right < WINDOW_WIDTH)
		if (allParticles[right][point.y] == nullptr)
			CreateParticle(sourceType, right, point.y, temperature);

	return false;
}

// Glitch particles.
Glitch::Glitch(int newX, int newY, float newTemperature) : SolidImmobile(TYPE_GLITCH, newX, newY, newTemperature)
{
}
void Glitch::Draw()
{
	EditPixel(point.x, point.y, xor128());
}

bool Glitch::HandleEvents()
{
	if (Particle::HandleEvents())
		return true;

	// Glitch event handle.
	// Attempt to create new glitch particles around this particle and delete any other blocks that are in the way.

	// Values to help with determining what ways to calculate.
	int up, down, left, right;
	up = down = point.y;
	left = right = point.x;
	up--;
	down++;
	left--;
	right++;	

	// Attempt to spread to other blocks.
	if (up >= 0)
	{
		Particle* pUp = allParticles[point.x][up];
		if (pUp == nullptr)
		{
			if (xor128() % 100 <= (uint32_t)glitchSpreadChance)
				CreateParticle(TYPE_GLITCH, point.x, up, temperature);
		}
		else if (pUp->type != TYPE_GLITCH)
			if (xor128() % 100 <= (uint32_t)glitchSpreadChance)
			{
				DestroyParticle(point.x, up);
				CreateParticle(TYPE_GLITCH, point.x, up, temperature);
			}
	}

	if (down < WINDOW_HEIGHT)
	{
		Particle* pDown = allParticles[point.x][down];
		if (pDown == nullptr)
		{
			if (xor128() % 100 <= (uint32_t)glitchSpreadChance)
				CreateParticle(TYPE_GLITCH, point.x, down, temperature);
		}
		else if (pDown->type != TYPE_GLITCH)
			if (xor128() % 100 <= (uint32_t)glitchSpreadChance)
			{
				DestroyParticle(point.x, down);
				CreateParticle(TYPE_GLITCH, point.x, down, temperature);
			}
	}

	if (left >= 0)
	{
		Particle* pLeft = allParticles[left][point.y];
		if (pLeft == nullptr)
		{
			if (xor128() % 100 <= (uint32_t)glitchSpreadChance)
				CreateParticle(TYPE_GLITCH, left, point.y, temperature);
		}
		else if (pLeft->type != TYPE_GLITCH)
			if (xor128() % 100 <= (uint32_t)glitchSpreadChance)
			{
				DestroyParticle(left, point.y);
				CreateParticle(TYPE_GLITCH, left, point.y, temperature);
			}
	}

	if (right < WINDOW_WIDTH)
	{
		Particle* pRight = allParticles[right][point.y];
		if (pRight == nullptr)
		{
			if (xor128() % 100 <= (uint32_t)glitchSpreadChance)
				CreateParticle(TYPE_GLITCH, right, point.y, temperature);
		}
		else if (pRight->type != TYPE_GLITCH)
			if (xor128() % 100 <= (uint32_t)glitchSpreadChance)
			{
				DestroyParticle(right, point.y);
				CreateParticle(TYPE_GLITCH, right, point.y, temperature);
			}
	}

	return false;
}

// Stone particles.
// Can melt into lava at high heat.
Stone::Stone(int newX, int newY, float newTemperature) : SolidMobile(TYPE_STONE, newX, newY, newTemperature)
{
}

bool Stone::HandleEvents()
{
	if (Particle::HandleEvents())
		return true;

	if (temperature > lavaSolidifyTemp)
	{
		int newX = point.x;
		int newY = point.y;
		float newTemp = temperature;

		DestroyParticle(point.x, point.y);
		CreateParticle(TYPE_LAVA, newX, newY, newTemp);

		return true;
	}

	return false;
}

// Lava particles.
// Can turn into stone at low temperatures.
Lava::Lava(int newX, int newY, float newTemperature) : Liquid(TYPE_LAVA, newX, newY, newTemperature)
{
}

bool Lava::HandleEvents()
{
	if (Particle::HandleEvents())
		return true;

	if (temperature < lavaSolidifyTemp)
	{
		int newX = point.x;
		int newY = point.y;
		float newTemp = temperature;

		DestroyParticle(point.x, point.y);
		CreateParticle(TYPE_STONE, newX, newY, newTemp);

		return true;
	}

	return false;
}

Fire::Fire(int newX, int newY, float newTemperature) : Airborn(TYPE_FIRE, fireAscendRate, fireDescendRate, fireSidewardsRate, fireNoMovementRate, newX, newY, newTemperature)
{

}

bool Fire::HandleEvents()
{
	health--;

	if (Particle::HandleEvents())
		return true;

	// Check all surrounding particles and attempt to change them to fire.

	int up, down, left, right;
	up = down = point.y;
	left = right = point.x;
	up--;
	down++;
	left--;
	right++;

	if (up >= 0)
	{
		Particle* pUp = allParticles[point.x][up];
		if (pUp != nullptr)
			if (settingFlammability[pUp->type] > 0)
				if (settingFlammability[pUp->type] == 1)
				{
					int newHealth = pUp->health;
					int newX = point.x;
					int newY = up;
					float newTemp = temperature;

					DestroyParticle(point.x, up);

					CreateParticle(TYPE_FIRE, newX, newY, newTemp);

					pUp->health = newHealth;
				}
				else if (xor128() % 100 <= (uint32_t)settingFlammability[pUp->type])
				{
					int newHealth = pUp->health;
					int newX = point.x;
					int newY = up;
					float newTemp = temperature;

					DestroyParticle(point.x, up);

					CreateParticle(TYPE_FIRE, newX, newY, newTemp);

					pUp->health = newHealth;
				}
	}					

	if (down < WINDOW_HEIGHT)
	{
		Particle* pDown = allParticles[point.x][down];
		if (pDown != nullptr)
			if (settingFlammability[pDown->type] > 0)
				if (settingFlammability[pDown->type] == 1)
				{
					int newHealth = pDown->health;
					int newX = point.x;
					int newY = down;
					float newTemp = temperature;

					DestroyParticle(point.x, down);

					CreateParticle(TYPE_FIRE, newX, newY, newTemp);

					pDown->health = newHealth;
				}
				else if (xor128() % 100 <= (uint32_t)settingFlammability[pDown->type])
				{
					int newHealth = pDown->health;
					int newX = point.x;
					int newY = down;
					float newTemp = temperature;

					DestroyParticle(point.x, down);

					CreateParticle(TYPE_FIRE, newX, newY, newTemp);

					pDown->health = newHealth;
				}
	}				

	if (left >= 0)
	{
		Particle* pLeft = allParticles[left][point.y];
		if (pLeft != nullptr)
			if (settingFlammability[pLeft->type] > 0)
				if (settingFlammability[pLeft->type] == 1)
				{
					int newHealth = pLeft->health;
					int newX = left;
					int newY = point.y;
					float newTemp = temperature;

					DestroyParticle(left, point.y);

					CreateParticle(TYPE_FIRE, newX, newY, newTemp);

					pLeft->health = newHealth;
				}
				else if (xor128() % 100 <= (uint32_t)settingFlammability[pLeft->type])
				{
					int newHealth = pLeft->health;
					int newX = left;
					int newY = point.y;
					float newTemp = temperature;

					DestroyParticle(left, point.y);

					CreateParticle(TYPE_FIRE, newX, newY, newTemp);

					pLeft->health = newHealth;
				}
	}				

	if (right < WINDOW_WIDTH)
	{
		Particle* pRight = allParticles[right][point.y];
		if (pRight != nullptr)
			if (settingFlammability[pRight->type] > 0)
				if (settingFlammability[pRight->type] == 1)
				{
					int newHealth = pRight->health;
					int newX = right;
					int newY = point.y;
					float newTemp = temperature;

					DestroyParticle(right, point.y);

					CreateParticle(TYPE_FIRE, newX, newY, newTemp);

					pRight->health = newHealth;
				}
				else if (xor128() % 100 <= (uint32_t)settingFlammability[pRight->type])
				{
					int newHealth = pRight->health;
					int newX = right;
					int newY = point.y;
					float newTemp = temperature;

					DestroyParticle(right, point.y);

					CreateParticle(TYPE_FIRE, newX, newY, newTemp);

					pRight->health = newHealth;
				}
	}				

	return false;
}

Gas::Gas(int newX, int newY, float newTemperature) : Airborn(TYPE_GAS, gasAscendRate, gasDescendRate, gasSidewardsRate, gasNoMovementRate, newX, newY, newTemperature)
{
}

LightGas::LightGas(int newX, int newY, float newTemperature) : Airborn(TYPE_LIGHTGAS, lightGasAscendRate, lightGasDescendRate, lightGasSidewardsRate, lightGasNoMovementRate, newX, newY, newTemperature)
{
}

HeavyGas::HeavyGas(int newX, int newY, float newTemperature) : Airborn(TYPE_HEAVYGAS, heavyGasAscendRate, heavyGasDescendRate, heavyGasSidewardsRate, heavyGasNoMovementRate, newX, newY, newTemperature)
{
}

HeatPad::HeatPad(int newX, int newY, float newTemperature) : SolidImmobile(TYPE_HEATPAD, newX, newY, newTemperature)
{
}

void HeatPad::Draw()
{
	DrawParticleHeat(point.x, point.y);
}