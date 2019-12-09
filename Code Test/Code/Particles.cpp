#include <algorithm>

#include "Particles.h"

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

float GetKelvin(float celcius)
{
	return celcius + 273.15;
}

Particle* allParticles[WINDOW_WIDTH][WINDOW_HEIGHT];

void CreateParticle(ParticleType type, int x, int y, float temp, bool asSource)
{
	//make sure were not trying to create an entity off screen
	if (x < 0 || x > WINDOW_WIDTH - 1 || y < 0 || y > WINDOW_HEIGHT - 1)
		return;

	//check that we have no entity in this section to begin with
	if (allParticles[x][y] != nullptr)
	{
		// NOTE: these debug options cause MAJOR lag when adding particles, do nto use htem unless absolutely needed.
		//std::cout << "Attempt to create a particle of type " + typeNames[type] + " at " + std::to_string(x) + "|" + std::to_string(y) + " when a particle already exists here of type " + typeNames[allParticles[x][y]->type] + "\n";
		return;
	}	

	//check what type of entity we need to create and assign it the required data as well as update the entityexists list
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

//overload for default mode
void CreateParticle(ParticleType type, int x, int y, float temp)
{
	CreateParticle(type, x, y, temp, false);
}

//destroys the particle at the given location and wipes the memory of it
void DestroyParticle(int x, int y)
{
	//check if a particle even exists in the area we want to delete and cancel if there is none
	if (allParticles[x][y] == nullptr)
		return;

	delete allParticles[x][y];	
	allParticles[x][y] = nullptr;
}

//used to move pointers of particles around
void MoveParticles(int x1, int y1, int x2, int y2)
{
	//get both the particles we want to move
	std::swap(allParticles[x1][y1], allParticles[x2][y2]);

	//update the internal location data for the first point now that its been moved
	if (allParticles[x2][y2] != nullptr)
	{
		allParticles[x2][y2]->point.x = x2;
		allParticles[x2][y2]->point.y = y2;
	}

	//update the internal location data for the second point now that its been moved
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
		if (allParticles[x][up] != nullptr)
			allParticles[x][up]->locked = false;

	if (down <= WINDOW_HEIGHT - 1)
		if (allParticles[x][down] != nullptr)
			allParticles[x][down]->locked = false;

	if (left >= 0)
		if (allParticles[left][y] != nullptr)
			allParticles[left][y]->locked = false;

	if (right <= WINDOW_WIDTH - 1)
		if (allParticles[right][y] != nullptr)
			allParticles[right][y]->locked = false;
}

//attempt to drop the particle downwards
bool DropParticle(int x, int y, bool randomize)
{
	int tempX = x;

	//attempt to randomize the way it will move while going down
	if (randomize)
	{
		tempX += (rand() % 3) - 1;

		if (tempX < 0)
			return false;

		if (tempX > WINDOW_WIDTH - 1)
			return false;
	}

	//if there is no particle directly below then just drop down
	if (allParticles[tempX][y + 1] == nullptr)
	{
		//update the array
		UnlockNeighbourParticles(x, y);
		MoveParticles(x, y, tempX, y + 1);
		return true;
	}
	//other wise if the particle directly below has less weight then swap particles
	else if (allParticles[tempX][y + 1]->weight != -1)
		if (allParticles[x][y]->weight > allParticles[tempX][y + 1]->weight)
		{
			UnlockNeighbourParticles(x, y);
			MoveParticles(x, y, tempX, y + 1);			
			return true;
		}

	return false;
}

//overload for legacy version of DropParticle
bool DropParticle(int x, int y)
{
	return DropParticle(x, y, false);
}

//attempt to ascend the particle directly upwards
bool AscendParticle(int x, int y, bool randomize)
{
	int tempX = x;

	//attempt to randomize the way it will move while going up
	if (randomize)
	{
		tempX += (rand() % 3) - 1;

		if (tempX < 0)
			return false;

		if (tempX > WINDOW_WIDTH - 1)
			return false;
	}

	//if there is no particle directly above then just ascend
	if (allParticles[tempX][y - 1] == nullptr)
	{
		//update the array
		UnlockNeighbourParticles(x, y);
		MoveParticles(x, y, tempX, y - 1);
		return true;
	}
	//other wise if the particle directly above has more weight then swap particles
	else if (allParticles[tempX][y - 1]->weight != -1)
		if (allParticles[x][y]->weight < allParticles[tempX][y - 1]->weight)
		{
			UnlockNeighbourParticles(x, y);
			MoveParticles(x, y, tempX, y - 1);
			return true;
		}

	return false;
}

//attempts to even out temperatures between the two given points using the given thermal change value
void EvenOutTemperatures(int x1, int y1, int x2, int y2)
{
	//make sure both points are not blank
	if (allParticles[x1][y1] != nullptr && allParticles[x2][y2] != nullptr)
	{
		//make sure they arent already the same temp
		if (allParticles[x1][y1]->temperature == allParticles[x2][y2]->temperature)
			return;

		//get the difference between both temperatures in an attempt to either increase or decrease the spread of temperature between objects
		float calculatedTemperatureDifference = (std::abs(((allParticles[x1][y1]->temperature - allParticles[x2][y2]->temperature) / allParticles[x1][y1]->temperature) * 100)) / temperatureDifferenceDivisor;

		float calculatedTemperatureChange = calculatedTemperatureDifference * std::abs(allParticles[x1][y1]->thermalConductivity + allParticles[x2][y2]->thermalConductivity);


		//check if the temperature change will mean they both even each other out or not
		if (std::abs(allParticles[x1][y1]->temperature - allParticles[x2][y2]->temperature) > calculatedTemperatureChange)
		{
			//first particle is higher temp then second
			if (allParticles[x1][y1]->temperature > allParticles[x2][y2]->temperature)
			{
				allParticles[x1][y1]->temperature -= calculatedTemperatureChange;
				allParticles[x2][y2]->temperature += calculatedTemperatureChange;
			}
			else//second particle is higher temp then first
			{
				allParticles[x1][y1]->temperature += calculatedTemperatureChange;
				allParticles[x2][y2]->temperature -= calculatedTemperatureChange;
			}
		}
		else//both temperatures are close enough to just even them out at same temperature
		{
			float temp = (allParticles[x1][y1]->temperature + allParticles[x2][y2]->temperature) / 2;
			allParticles[x1][y1]->temperature = temp;
			allParticles[x2][y2]->temperature = temp;
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

//default draw code
void Particle::Draw()
{
	if (!drawHeat)
		EditPixel(point.x, point.y, SDL_MapRGBA(mainSurface->format, settingColor[type][0], settingColor[type][1], settingColor[type][2], settingColor[type][3]));
	else
		{
			float tempHighest = 80;
			float tempLowest = -20;

			float ratio = 2 * (temperature - tempLowest) / (tempHighest - tempLowest);

			int r, g, b;

			r = std::max(0, (int)(255 * (ratio - 1)));
			b = std::max(0, (int)(255 * (1 - ratio)));
			g = 255 - b - r;

			EditPixel(point.x, point.y, SDL_MapRGB(mainSurface->format, r, g, b));
		}
}

bool Particle::HandleEvents()
{
	//temperature handle
	if (thermalConductivity > 0)
	{
		//values to help with determining what ways to calculate
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

		if (down <= WINDOW_HEIGHT - 1)
			canGoDown = true;

		if (left >= 0)
			canGoLeft = true;

		if (right <= WINDOW_WIDTH - 1)
			canGoRight = true;


		//check and update each direction that we can
		if (canGoUp)
			EvenOutTemperatures(point.x, point.y, point.x, up);

		if (canGoDown)
			EvenOutTemperatures(point.x, point.y, point.x, down);

		if (canGoLeft)
			EvenOutTemperatures(point.x, point.y, left, point.y);

		if (canGoRight)
			EvenOutTemperatures(point.x, point.y, right, point.y);
	}

	//health check
	if (health <= 0)
	{
		DestroyParticle(point.x, point.y);
		return true;
	}

	return false;
}

//check if the particle is at the bottom of the screen and if we need to loop back to the top
bool Particle::CheckIfAtBottom()
{
	//check if at bottom of screen
	if (point.y == WINDOW_HEIGHT - 1)
	{
		//check if we need to loop the particles from bottom to top
		if (loopScreen)
		{
			if (allParticles[point.x][0] == nullptr)
			{
				UnlockNeighbourParticles(point.x, point.y);
				MoveParticles(point.x, point.y, point.x, 0);
			}

			//even if we cant move return true as we are stil lat the bottom and cant move else where
			return true;
		}
		else
			return true;//we are not to loop, stop here
	}

	//we are not at the bottom
	return false;
}

bool Particle::CheckIfAtTop()
{
	//check if at bottom of screen
	if (point.y == 0)
	{
		//check if we need to loop the particles from bottom to top
		if (loopScreen)
		{
			if (allParticles[point.x][WINDOW_HEIGHT - 1] == nullptr)
			{
				UnlockNeighbourParticles(point.x, point.y);
				MoveParticles(point.x, point.y, point.x, WINDOW_HEIGHT - 1);
			}

			//even if we cant move return true as we are still at the bottom and cant move else where
			return true;
		}
		else
			return true;//we are not to loop, stop here
	}

	//we are not at the bottom
	return false;
}


//default particle physics
void Particle::HandlePhysics()
{
}

//solid immobile particles
//particles that stay in spot and do not move at all
SolidImmobile::SolidImmobile(ParticleType newType, int newX, int newY, float newTemperature) : Particle (newType, newX, newY, newTemperature)
{
}

bool SolidImmobile::HandleEvents()
{
	Particle::HandleEvents();

	return false;
}

//solid mobile particles
//particles that drop downwards
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

	//make sure we arent already at the bottom level, if we are we dont need to check the physics
	if (CheckIfAtBottom())
		return;

	//check if an object exists under this one
	if (DropParticle(point.x, point.y))
		return;

	int left, right, down;
	left = right = point.x;
	left--;
	right++;
	down = point.y + 1;

	if (left < 0)
		left = 0;

	if (right > WINDOW_WIDTH - 1)
		right = WINDOW_WIDTH - 1;

	//check if we are surrounded
	if (allParticles[left][down] != nullptr && allParticles[point.x][down] != nullptr && allParticles[right][down] != nullptr)
	{
		//if we are check if we can move via gravity with the surrounding blocks

		bool canGoLeft, canGoRight;
		canGoLeft = canGoRight = false;

		if (allParticles[left][down]->weight != -1)
			if (allParticles[point.x][point.y]->weight < allParticles[left][down]->weight)
				canGoLeft = true;

		if (allParticles[right][down]->weight != -1)
			if (allParticles[point.x][point.y]->weight < allParticles[right][down]->weight)
				canGoRight = true;

		if (canGoLeft && canGoRight)
		{
			switch (randomizedDirection)
			{
				//go left
			case 1:
				UnlockNeighbourParticles(point.x, point.y);
				MoveParticles(left, down, point.x, point.y);
				return;

				//go right
			case 2:
				UnlockNeighbourParticles(point.x, point.y);
				MoveParticles(right, down, point.x, point.y);
				return;
			}
		}

		if (canGoLeft)
		{
			UnlockNeighbourParticles(point.x, point.y);
			MoveParticles(left, down, point.x, point.y);
			return;
		}

		if (canGoRight)
		{
			UnlockNeighbourParticles(point.x, point.y);
			MoveParticles(right, down, point.x, point.y);
			return;
		}

		//neither way is less weight and we know that were surrounded, lock the particle and do not run any further code
		locked = true;
		return;
	}

	//since there is an object under this one lets check if we can fall to the left or right
	if (point.x == 0)//check if we are on the left most edge
	{
		//we are on the left most edge, try to drop to the bottom right
		if (allParticles[right][down] == nullptr)
		{
			//update the array
			UnlockNeighbourParticles(point.x, point.y);
			MoveParticles(point.x, point.y, right, down);
			return;
		}

		//even if we cant drop, we know that we can not go anywhere so stop here and lock the particle
		locked = true;
		return;
	}

	if (point.x == WINDOW_WIDTH - 1)//making sure were not at the right most edge
	{
		//we are on the right most edge, try to drop to the bottom left
		if (allParticles[left][down] == nullptr)
		{
			//update the array
			UnlockNeighbourParticles(point.x, point.y);
			MoveParticles(point.x, point.y, left, down);
			return;
		}

		//even if we cant drop, we know that we can not go anywhere so stop here and lock the particle
		locked = true;
		return;
	}

	//if the bottom left is empty and the bottom right isnt then drop to the bottom left
	if (allParticles[left][down] == nullptr && allParticles[right][down] != nullptr)
	{
		//update the array
		UnlockNeighbourParticles(point.x, point.y);
		MoveParticles(point.x, point.y, left, down);
		return;
	}
	//if the bottom right is empty and the bottom left isnt the drop to the bottom right
	else if (allParticles[right][down] == nullptr && allParticles[left][down] != nullptr)
	{
		//update the array
		UnlockNeighbourParticles(point.x, point.y);
		MoveParticles(point.x, point.y, right, down);
		return;
	}
	//if both sides are free randomly choose one direction and drop down that side	
	else if (allParticles[right][down] == nullptr && allParticles[left][down] == nullptr)
	{

		switch (randomizedDirection)
		{
			//go left
		case 1:
			//update the array
			UnlockNeighbourParticles(point.x, point.y);
			MoveParticles(point.x, point.y, left, down);
			return;

			//go right
		case 2:
			//update the array
			UnlockNeighbourParticles(point.x, point.y);
			MoveParticles(point.x, point.y, right, down);
			return;
		}
	}
}

//liquid particles
//particles that drop downwards and attempt to flatten themselves out on surfaces

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
	//make sure we arent already at the bottom level, if we are we dont need to check the physics
	if (CheckIfAtBottom())
		return;

	//check if an object exists under this one
	if (DropParticle(point.x, point.y))
		return;

	int left, right, down;
	left = right = point.x;
	left--;
	right++;
	down = point.y + 1;

	if (left < 0)
		left = 0;

	if (right > WINDOW_WIDTH - 1)
		right = WINDOW_WIDTH - 1;

	//check if we are surrounded
	if (allParticles[left][point.y] != nullptr && allParticles[point.x][down] != nullptr && allParticles[right][point.y] != nullptr)
	{
		//if we are check if we can move via gravity with the surrounding blocks
		bool canGoLeft, canGoRight;
		canGoLeft = canGoRight = false;

		if (allParticles[left][point.y]->weight != -1)
			if (allParticles[point.x][point.y]->weight < allParticles[left][point.y]->weight)
				canGoLeft = true;

		if (allParticles[right][point.y]->weight != -1)
			if (allParticles[point.x][point.y]->weight < allParticles[right][point.y]->weight)
				canGoRight = true;

		//can go either way
		if (canGoLeft && canGoRight)
		{
			switch (randomizedDirection)
			{
				//go left
			case 1:
				UnlockNeighbourParticles(point.x, point.y);
				MoveParticles(left, point.y, point.x, point.y);
				return;

				//go right
			case 2:
				UnlockNeighbourParticles(point.x, point.y);
				MoveParticles(right, point.y, point.x, point.y);
				return;
			}
		}

		//can only go left
		if (canGoLeft)
		{
			UnlockNeighbourParticles(point.x, point.y);
			MoveParticles(left, point.y, point.x, point.y);
			return;
		}

		//can only go right
		if (canGoRight)
		{
			UnlockNeighbourParticles(point.x, point.y);
			MoveParticles(right, point.y, point.x, point.y);
			return;
		}

		//neither way is less weight and we know that were surrounded, do not run any further code
		return;
	}

	//there is an object under this one, try and move to the left or right
	if (point.x == 0)//check if we are on the left most edge
	{
		//we are on the left most edge, try to drop to the bottom right
		if (allParticles[right][point.y] == nullptr)
		{
			//update the array
			UnlockNeighbourParticles(point.x, point.y);
			MoveParticles(point.x, point.y, right, point.y);
			return;
		}
		return;//even if we cant drop, we know that we can not go anywhere so stop here
	}

	if (point.x == WINDOW_WIDTH - 1)//making sure were not at the right most edge
	{
		//we are on the right most edge, try to drop to the bottom left
		if (allParticles[left][point.y] == nullptr)
		{
			//update the array
			UnlockNeighbourParticles(point.x, point.y);
			MoveParticles(point.x, point.y, left, point.y);
			return;
		}
		return;//even if we cant drop, we know that we can not go anywhere so stop here
	}

	//if left is empty only
	if (allParticles[left][point.y] == nullptr && allParticles[right][point.y] != nullptr)
	{
		UnlockNeighbourParticles(point.x, point.y);
		MoveParticles(point.x, point.y, left, point.y);
	}
	//if right is empty only
	else if (allParticles[right][point.y] == nullptr && allParticles[left][point.y] != nullptr)
	{
		UnlockNeighbourParticles(point.x, point.y);
		MoveParticles(point.x, point.y, right, point.y);
	}
	//both ways are free, randomly choose one and move	
	else if (allParticles[right][point.y] == nullptr && allParticles[left][point.y] == nullptr)
	{
		switch (randomizedDirection)
		{
			//go left
		case 1:
			//update the array
			UnlockNeighbourParticles(point.x, point.y);
			MoveParticles(point.x, point.y, left, point.y);
			return;

			//go right
		case 2:
			//update the array
			UnlockNeighbourParticles(point.x, point.y);
			MoveParticles(point.x, point.y, right, point.y);
			return;
		}
	}
}

//airborn particles
//particles that float around in the air either going directly up or slowly hovering around
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
	down--;
	left--;
	right++;

	//check what way we want to move first
	int randomNum = rand() % (ascendRate + descendRate + sidewardsRate + noMovementRate);

	//check if should ascend
	if (randomNum < ascendRate)
	{
		//check if we are at the top of the screen and if we can loop around
		if (!CheckIfAtTop())
			AscendParticle(point.x, point.y, false);
	}
	//check if should descend
	else if (randomNum < (ascendRate + descendRate))
	{
		//check if we are at the top of the screen and if we can loop around
		if (!CheckIfAtBottom())
			DropParticle(point.x, point.y, false);
	}
	//chck if should go left or right
	else if (randomNum < (ascendRate + descendRate + sidewardsRate))
	{
		bool canGoLeft, canGoRight;
		canGoLeft = canGoRight = false;

		//check if we are surrounded
		if (left >= 0)
			if (allParticles[left][point.y] == nullptr)
				canGoLeft = true;
			else
			{
				if (allParticles[left][point.y]->weight != -1)
					if (allParticles[point.x][point.y]->weight < allParticles[left][point.y]->weight)
						canGoLeft = true;
			}

		if (right <= (WINDOW_WIDTH - 1))
			if (allParticles[right][point.y] == nullptr)
				canGoRight = true;
			else
			{
				if (allParticles[right][point.y]->weight != -1)
					if (allParticles[point.x][point.y]->weight < allParticles[right][point.y]->weight)
						canGoRight = true;
			}	

		//can go either way
		if (canGoLeft && canGoRight)
		{
			switch (randomizedDirection)
			{
				//go left
			case 1:
				UnlockNeighbourParticles(point.x, point.y);
				MoveParticles(left, point.y, point.x, point.y);
				break;

				//go right
			case 2:
				UnlockNeighbourParticles(point.x, point.y);
				MoveParticles(right, point.y, point.x, point.y);
				break;
			}
		}
		//can only go left
		else if (canGoLeft)
		{
			UnlockNeighbourParticles(point.x, point.y);
			MoveParticles(left, point.y, point.x, point.y);
		}
		//can only go right
		else
		{
			UnlockNeighbourParticles(point.x, point.y);
			MoveParticles(right, point.y, point.x, point.y);
		}		

		//neither way is less weight and we know that were surrounded, do not run any further code
		locked = true;
		return;	
	}	
}

//wall particles
Wall::Wall(int newX, int newY, float newTemperature) : SolidImmobile(TYPE_WALL, newX, newY, newTemperature)
{	
}

//sand particles
Sand::Sand(int newX, int newY, float newTemperature) : SolidMobile(TYPE_SAND, newX, newY, newTemperature)
{	
}

//water particles
Water::Water(int newX, int newY, float newTemperature) : Liquid(TYPE_WATER, newX, newY, newTemperature)
{
}

bool Water::HandleEvents()
{
	//handle default events, this includes handling temperature shifts
	if (Particle::HandleEvents())
		return true;

	//check if the water should freeze
	if (temperature < waterFreezePoint)
	{
		int newX = point.x;
		int newY = point.y;
		float newTemp = temperature;

		DestroyParticle(point.x, point.y);
		CreateParticle(TYPE_ICE, newX, newY, newTemp);

		return true;
	}
	//check if should turn into steam
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

//ice particles
Ice::Ice(int newX, int newY, float newTemperature) : SolidImmobile(TYPE_ICE, newX, newY, newTemperature)
{
}

bool Ice::HandleEvents()
{
	//handle default events first, this includes temperature shifting
	if (Particle::HandleEvents())
		return true;

	//check if ice should melt	
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

//acid particles
Acid::Acid(int newX, int newY, float newTemperature) : Liquid(TYPE_ACID, newX, newY, newTemperature)
{
}

//acid needs to randomly damage other blocks around it that are not acid
bool Acid::HandleEvents()
{
	if (Particle::HandleEvents())
		return true;

	//make sure an object exists first
	if (this == nullptr)
		return true;

	//values to help with determining what ways to calculate
	int up, down, left, right;
	up = down = point.y;
	left = right = point.x;
	up--;
	down++;
	left--;
	right++;

	bool canDamageUp, canDamageDown, canDamageLeft, canDamageRight;
	canDamageUp = canDamageDown = canDamageLeft = canDamageRight = false;

	//check if blocks exist in each direction and attempt to target it for damage 
	if (up >= 0)
		if (allParticles[point.x][up] != nullptr)
			if (allParticles[point.x][up]->type != TYPE_ACID)
				canDamageUp = true;

	if (down <= WINDOW_HEIGHT - 1)
		if (allParticles[point.x][down] != nullptr)
			if (allParticles[point.x][down]->type != TYPE_ACID)
				canDamageDown = true;

	if (left >= 0)
		if (allParticles[left][point.y] != nullptr)
			if (allParticles[left][point.y]->type != TYPE_ACID)
				canDamageLeft = true;

	if (right <= WINDOW_WIDTH - 1)
		if (allParticles[right][point.y] != nullptr)
			if (allParticles[right][point.y]->type != TYPE_ACID)
				canDamageRight = true;


	//check each direction and attempt to damage any targeted blocks
	if (canDamageUp)
		if ((rand() % acidDamageChance + 1) == 1)
		{
			allParticles[point.x][up]->health--;
			health--;
		}

	if (canDamageDown)
		if ((rand() % acidDamageChance + 1) == 1)
		{
			allParticles[point.x][down]->health--;
			health--;
		}

	if (canDamageLeft)
		if ((rand() % acidDamageChance + 1) == 1)
		{
			allParticles[left][point.y]->health--;
			health--;
		}

	if (canDamageRight)
		if ((rand() % acidDamageChance + 1) == 1)
		{
			allParticles[right][point.y]->health--;
			health--;
		}

	return false;
}

//steam particles
Steam::Steam(int newX, int newY, float newTemperature) : Airborn(TYPE_STEAM, steamAscendRate, steamDescendRate, steamSidewardsRate, steamNoMovementRate, newX, newY, newTemperature)
{
}

bool Steam::HandleEvents()
{
	//handle default events first, this includes temperature shifting
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

//plant particles
Plant::Plant(int newX, int newY, float newTemperature) : SolidImmobile(TYPE_PLANT, newX, newY, newTemperature)
{
}

bool Plant::HandleEvents()
{
	//handle default events first
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

	//check up and down
	if (point.y > 0 && point.y < WINDOW_HEIGHT - 1)
	{
		//check if can spread upwards
		if (allParticles[point.x][up] != nullptr)
			if (allParticles[point.x][up]->type == TYPE_WATER)
			{
				switch (rand() % plantSpreadChance)
				{
				case 0:
					canGoUp = true;
					break;
				}
			}

		//check if cna spread downwards
		if (allParticles[point.x][down] != nullptr)
			if (allParticles[point.x][down]->type == TYPE_WATER)
			{
				switch (rand() % plantSpreadChance)
				{
				case 0:
					canGoDown = true;
					break;
				}
			}
	}

	//check left and right
	if (point.x > 0 && point.x < WINDOW_WIDTH - 1)
	{
		//check if cna spread left
		if (allParticles[left][point.y] != nullptr)
			if (allParticles[left][point.y]->type == TYPE_WATER)
			{
				switch (rand() % plantSpreadChance)
				{
				case 0:
					canGoLeft = true;
					break;
				}
			}

		//check if cna spread right
		if (allParticles[right][point.y] != nullptr)
			if (allParticles[right][point.y]->type == TYPE_WATER)
			{
				switch (rand() % plantSpreadChance)
				{
				case 0:
					canGoRight = true;
					break;
				}
			}
	}

	//check what directions we can spread and spread in those directions
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

//salt particle
Salt::Salt(int newX, int newY, float newTemperature) : SolidMobile(TYPE_SALT, newX, newY, newTemperature)
{
}

bool Salt::HandleEvents()
{
	//default event handling
	if (Particle::HandleEvents())
		return true;

	//salt event handling
	int up, down, left, right;

	up = down = point.y;
	left = right = point.x;

	up--;
	down++;
	left--;
	right++;

	//up
	if (allParticles[point.x][up] != nullptr)
		if (allParticles[point.x][up]->type == TYPE_WATER)
		{
			//get some temporary variables as both particles will be deleted soon and we do not want to possibly call upon a deleted object
			float temp = (allParticles[point.x][up]->temperature + allParticles[point.x][point.y]->temperature) / 2;
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

	//down
	if (allParticles[point.x][down] != nullptr)
		if (allParticles[point.x][down]->type == TYPE_WATER)
		{
			//get some temporary variables as both particles will be deleted soon and we do not want to possibly call downon a deleted object
			float temp = (allParticles[point.x][down]->temperature + allParticles[point.x][point.y]->temperature) / 2;
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

	//left
	if (allParticles[left][point.y] != nullptr)
		if (allParticles[left][point.y]->type == TYPE_WATER)
		{
			//get some temporary variables as both particles will be deleted soon and we do not want to possibly call upon a deleted object
			float temp = (allParticles[left][point.y]->temperature + allParticles[point.x][point.y]->temperature) / 2;
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

	//right
	if (allParticles[right][point.y] != nullptr)
		if (allParticles[right][point.y]->type == TYPE_WATER)
		{
			//get some temporary variables as both particles will be deleted soon and we do not want to possibly call upon a deleted object
			float temp = (allParticles[right][point.y]->temperature + allParticles[point.x][point.y]->temperature) / 2;
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

	return false;
}

//salt water particle
SaltWater::SaltWater(int newX, int newY, float newTemperature) : Liquid(TYPE_SALTWATER, newX, newY, newTemperature)
{
}

bool SaltWater::HandleEvents()
{
	//handle default events, this includes handling temperature shifts
	if (Particle::HandleEvents())
		return true;

	//check if the salt water should freeze
	if (temperature < (waterFreezePoint * saltWaterEventTempMultiplier))
	{
		int newX = point.x;
		int newY = point.y;
		float newTemp = temperature;

		DestroyParticle(point.x, point.y);
		CreateParticle(TYPE_SALTICE, newX, newY, newTemp);

		return true;
	}
	//check if should turn into steam
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

//salt ice particles
SaltIce::SaltIce(int newX, int newY, float newTemperature) : SolidImmobile(TYPE_SALTICE, newX, newY, newTemperature)
{
}

bool SaltIce::HandleEvents()
{
	//handle default events first, this includes temperature shifting
	if (Particle::HandleEvents())
		return true;

	//check if ice should melt	
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

//source particles
//creates copies of its self in the surrounding blocks that are empty
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

	//Source event handle
	//attempt to create new particles around this particle using its sourceType value

	//values to help with determining what ways to calculate
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

	if (down <= WINDOW_HEIGHT - 1)
		if (allParticles[point.x][down] == nullptr)
			CreateParticle(sourceType, point.x, down, temperature);

	if (left >= 0)
		if (allParticles[left][point.y] == nullptr)
			CreateParticle(sourceType, left, point.y, temperature);

	if (right <= WINDOW_WIDTH - 1)
		if (allParticles[right][point.y] == nullptr)
			CreateParticle(sourceType, right, point.y, temperature);

	return false;
}

//glitch particles
Glitch::Glitch(int newX, int newY, float newTemperature) : SolidImmobile(TYPE_GLITCH, newX, newY, newTemperature)
{
}
void Glitch::Draw()
{
	SDL_SetRenderDrawColor(mainRenderer, (rand() % 255), (rand() % 255), (rand() % 255), 0);
	SDL_RenderDrawPoint(mainRenderer, point.x, point.y);
}

bool Glitch::HandleEvents()
{
	if (Particle::HandleEvents())
		return true;

	//Glitch event handle
	//attempt to create new glitch particles around this particle and delete any other blocks that are in the way

	//values to help with determining what ways to calculate
	int up, down, left, right;
	up = down = point.y;
	left = right = point.x;
	up--;
	down++;
	left--;
	right++;

	//attempt to spread to other blocks
	if (up >= 0)
		if (allParticles[point.x][up] == nullptr)
		{
			if ((rand() % glitchSpreadChance) == 0)
				CreateParticle(TYPE_GLITCH, point.x, up, temperature);
		}
		else if (allParticles[point.x][up]->type != TYPE_GLITCH)
			if ((rand() % glitchSpreadChance) == 0)
			{
				DestroyParticle(point.x, up);
				CreateParticle(TYPE_GLITCH, point.x, up, temperature);
			}

	if (down <= WINDOW_HEIGHT - 1)
		if (allParticles[point.x][down] == nullptr)
		{
			if ((rand() % glitchSpreadChance) == 0)
				CreateParticle(TYPE_GLITCH, point.x, down, temperature);
		}
		else if (allParticles[point.x][down]->type != TYPE_GLITCH)
			if ((rand() % glitchSpreadChance) == 0)
			{
				DestroyParticle(point.x, down);
				CreateParticle(TYPE_GLITCH, point.x, down, temperature);
			}

	if (left >= 0)
		if (allParticles[left][point.y] == nullptr)
		{
			if ((rand() % glitchSpreadChance) == 0)
				CreateParticle(TYPE_GLITCH, left, point.y, temperature);
		}
		else if (allParticles[left][point.y]->type != TYPE_GLITCH)
			if ((rand() % glitchSpreadChance) == 0)
			{
				DestroyParticle(left, point.y);
				CreateParticle(TYPE_GLITCH, left, point.y, temperature);
			}

	if (right <= WINDOW_WIDTH - 1)
		if (allParticles[right][point.y] == nullptr)
		{
			if ((rand() % glitchSpreadChance) == 0)
				CreateParticle(TYPE_GLITCH, right, point.y, temperature);
		}
		else if (allParticles[right][point.y]->type != TYPE_GLITCH)
			if ((rand() % glitchSpreadChance) == 0)
			{
				DestroyParticle(right, point.y);
				CreateParticle(TYPE_GLITCH, right, point.y, temperature);
			}

	return false;
}

//stone particles
//can melt into lava at high heat
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

//lava particles
//can turn into stone at low temperatures
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
		if (allParticles[point.x][up] != nullptr)		
			if (settingFlammability[allParticles[point.x][up]->type] > 0)
				if (settingFlammability[allParticles[point.x][up]->type] == 1)
				{
					int newHealth = allParticles[point.x][up]->health;
					int newX = point.x;
					int newY = up;
					float newTemp = temperature;

					DestroyParticle(point.x, up);

					CreateParticle(TYPE_FIRE, newX, newY, newTemp);

					allParticles[point.x][up]->health = newHealth;
				}
				else
				{
					int rando = rand() % settingFlammability[allParticles[point.x][up]->type];

					if (rando == 0)
					{
						int newHealth = allParticles[point.x][up]->health;
						int newX = point.x;
						int newY = up;
						float newTemp = temperature;

						DestroyParticle(point.x, up);

						CreateParticle(TYPE_FIRE, newX, newY, newTemp);

						allParticles[point.x][up]->health = newHealth;
					}
				}		

	if (down <= WINDOW_HEIGHT - 1)
		if (allParticles[point.x][down] != nullptr)
			if (settingFlammability[allParticles[point.x][down]->type] > 0)
				if (settingFlammability[allParticles[point.x][down]->type] == 1)
				{
					int newHealth = allParticles[point.x][down]->health;
					int newX = point.x;
					int newY = down;
					float newTemp = temperature;

					DestroyParticle(point.x, down);

					CreateParticle(TYPE_FIRE, newX, newY, newTemp);

					allParticles[point.x][down]->health = newHealth;
				}
				else
				{
					int rando = rand() % settingFlammability[allParticles[point.x][down]->type];

					if (rando == 0)
					{
						int newHealth = allParticles[point.x][down]->health;
						int newX = point.x;
						int newY = down;
						float newTemp = temperature;

						DestroyParticle(point.x, down);

						CreateParticle(TYPE_FIRE, newX, newY, newTemp);

						allParticles[point.x][down]->health = newHealth;
					}
				}

	if (left >= 0)
		if (allParticles[left][point.y] != nullptr)
			if (settingFlammability[allParticles[left][point.y]->type] > 0)
				if (settingFlammability[allParticles[left][point.y]->type] == 1)
				{
					int newHealth = allParticles[left][point.y]->health;
					int newX = left;
					int newY = point.y;
					float newTemp = temperature;

					DestroyParticle(left, point.y);

					CreateParticle(TYPE_FIRE, newX, newY, newTemp);

					allParticles[left][point.y]->health = newHealth;
				}
				else
				{
					int rando = rand() % settingFlammability[allParticles[left][point.y]->type];

					if (rando == 0)
					{
						int newHealth = allParticles[left][point.y]->health;
						int newX = left;
						int newY = point.y;
						float newTemp = temperature;

						DestroyParticle(left, point.y);

						CreateParticle(TYPE_FIRE, newX, newY, newTemp);

						allParticles[left][point.y]->health = newHealth;
					}
				}		

	if (right <= WINDOW_WIDTH - 1)
		if (allParticles[right][point.y] != nullptr)
			if (settingFlammability[allParticles[right][point.y]->type] > 0)
				if (settingFlammability[allParticles[right][point.y]->type] == 1)
				{
					int newHealth = allParticles[right][point.y]->health;
					int newX = right;
					int newY = point.y;
					float newTemp = temperature;

					DestroyParticle(right, point.y);

					CreateParticle(TYPE_FIRE, newX, newY, newTemp);

					allParticles[right][point.y]->health = newHealth;
				}
				else
				{
					int rando = rand() % settingFlammability[allParticles[right][point.y]->type];

					if (rando == 0)
					{
						int newHealth = allParticles[right][point.y]->health;
						int newX = right;
						int newY = point.y;
						float newTemp = temperature;

						DestroyParticle(right, point.y);

						CreateParticle(TYPE_FIRE, newX, newY, newTemp);

						allParticles[right][point.y]->health = newHealth;
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