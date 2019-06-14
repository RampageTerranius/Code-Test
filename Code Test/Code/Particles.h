#pragma once

//used to help objects keep track of what they are for rendering purposes as the lists will be using base particles that are using polymorphism to determine how to handle physics
enum ParticleType
{
	TYPE_WALL,
	TYPE_SAND,
	TYPE_WATER,
	TYPE_ICE,
	TYPE_THERMALFLUID,
	TYPE_ACID,
	TYPE_STEAM,
	TYPE_PLANT,
	TYPE_SALT,
	TYPE_SALTWATER,
	TYPE_SALTICE,

	//the following must ALWAYS be at the end
	TYPE_SOURCE,
	TYPE_TOTALTYPES
};

//used to give a name to each particle type
std::string typeNames[] = { "Wall",
							"Sand",
							"Water",
							"Ice",
							"Thermal Fluid",
							"Acid",
							"Steam",
							"Plant",
							"Salt",
							"Salt Water",
							"Salt Ice",

							//the following must ALWAYS be at the end
							"Source - "
							"TotalTypes"};//you should never be using this final string, if you are your working with a non existant particale type

class Particle
{
	public:
		Particle();
		void Reset();		
		virtual void Draw();
		bool CheckIfAtBottom();
		bool CheckIfAtTop();
		virtual bool HandleEvents();
		virtual void HandlePhysics();

		int x, y;
		int weight;
		int health;
		float temperature;
		float thermalConductivity;
		ParticleType type;
};

//wall particles
//does not move
class Wall : public Particle
{
public:
	Wall(int newX, int newY, float newTemperature);
	void Draw();
	void HandlePhysics();
};

//plant particles
//does not move but can convert neighbouring water particles into plant particles
class Plant : public Wall
{
public:
	Plant(int newX, int newY, float newTemperature);
	void Draw();
	bool HandleEvents();
	void HandlePhysics();
};

//sand particles
class Sand : public Particle
{
public:
	Sand(int newX, int newY, float newTemperature);
	void Draw();
	void HandlePhysics();
};

//water particles
//always attempts to move downwards if nothing is in way, will always try to move either left or right if cant move down
//can freeze into water
class Water : public Particle
{
public:
	Water(int newX, int newY, float newTemperature);
	void Draw();
	bool HandleEvents();
	void HandlePhysics();
};

//basically just water but has a very high thermal conductivity
class ThermalFluid : public Water
{
public:
	ThermalFluid(int newX, int newY, float newTemperature);
	void Draw();
	bool HandleEvents();
	void HandlePhysics();
};

//basicalyl jsut water but it damaged blocks around it
class Acid : public Water
{
public:
	Acid(int newX, int newY, float newTemperature);
	void Draw();
	bool HandleEvents();
	void HandlePhysics();
};

//salt water particles
//basically just standard water but a bit mroe heavier and will damage any plants it touches
class SaltWater : public Water
{
public:
	SaltWater(int newX, int newY, float newTemperature);
	void Draw();
	bool HandleEvents();
	void HandlePhysics();
};

//salt particle
//basically sand but can combine with water to become salt water
class Salt : public Sand
{
public:
	Salt(int newX, int newY, float newTemperature);
	void Draw();
	bool HandleEvents();
	void HandlePhysics();
};

//ice particles
//does not move
//can melt into water
class Ice : public Particle
{
public:
	Ice(int newX, int newY, float newTemperature);
	void Draw();
	bool HandleEvents();
	void HandlePhysics();
};

//salt ice particles
//does not move
//can melt into salt water
class SaltIce : public Particle
{
public:
	SaltIce(int newX, int newY, float newTemperature);
	void Draw();
	bool HandleEvents();
	void HandlePhysics();
};

//steam particles
//move upwards in a randomised direction, somewhat like a reverese water particle
//turns back into water when cools down
class Steam : public Particle
{
public:
	Steam(int newX, int newY, float newTemperature);
	void Draw();
	bool HandleEvents();
	void HandlePhysics();
};

//source particle
//creates other particles of the given source type around it
//can be a source for any other type of block, useful for self repairing walls or for self replenishing water for example
class Source : public Particle
{
public:
	Source(ParticleType type, int newX, int newY, float newTemperature);
	void Draw();
	bool HandleEvents();
	void HandlePhysics();

	ParticleType sourceType;
};


//array and vector list handling all data to do with our entities
Particle* allParticles[WINDOW_WIDTH][WINDOW_HEIGHT];
std::vector<Particle*> particleList;

Particle* CreateParticle(ParticleType type, int x, int y, float temp, bool asSource)
{
	//check that we have no entity in this section to begin with
	if (allParticles[x][y] != nullptr)
	{
		std::cout << "Attempt to create a particle of type " + typeNames[type] + " at " + std::to_string(x) + "|" + std::to_string(y) + " when a particle already exists here of type " + typeNames[allParticles[x][y]->type] +"\n";
		return nullptr;
	}

	//make sure were not trying to create an entity off screen
	if (x < 0 || x > WINDOW_WIDTH - 1 || y < 0 || y > WINDOW_HEIGHT - 1)
		return nullptr;;

	//check what type of entity we need to create and assign it the required data as well as update the entityexists list
	if (!asSource)
	{
		switch (type)
		{
		case TYPE_WALL:
			allParticles[x][y] = new Wall(x, y, temp);
			particleList.push_back(allParticles[x][y]);
			return allParticles[x][y];

		case TYPE_SAND:
			allParticles[x][y] = new Sand(x, y, temp);
			particleList.push_back(allParticles[x][y]);
			return allParticles[x][y];

		case TYPE_WATER:
			allParticles[x][y] = new Water(x, y, temp);
			particleList.push_back(allParticles[x][y]);
			return allParticles[x][y];

		case TYPE_ICE:
			allParticles[x][y] = new Ice(x, y, temp);
			particleList.push_back(allParticles[x][y]);
			return allParticles[x][y];

		case TYPE_THERMALFLUID:
			allParticles[x][y] = new ThermalFluid(x, y, temp);
			particleList.push_back(allParticles[x][y]);
			return allParticles[x][y];

		case TYPE_ACID:
			allParticles[x][y] = new Acid(x, y, temp);
			particleList.push_back(allParticles[x][y]);
			return allParticles[x][y];

		case TYPE_STEAM:
			allParticles[x][y] = new Steam(x, y, temp);
			particleList.push_back(allParticles[x][y]);
			return allParticles[x][y];

		case TYPE_PLANT:
			allParticles[x][y] = new Plant(x, y, temp);
			particleList.push_back(allParticles[x][y]);
			return allParticles[x][y];

		case TYPE_SALT:
			allParticles[x][y] = new Salt(x, y, temp);
			particleList.push_back(allParticles[x][y]);
			return allParticles[x][y];

		case TYPE_SALTWATER:
			allParticles[x][y] = new SaltWater(x, y, temp);
			particleList.push_back(allParticles[x][y]);
			return allParticles[x][y];

		case TYPE_SALTICE:
			allParticles[x][y] = new SaltIce(x, y, temp);
			particleList.push_back(allParticles[x][y]);
			return allParticles[x][y];

		default:
			std::cout << "Attempt to create a unknown particle type at " + std::to_string(x) + "|" + std::to_string(y) + "\n";
			break;
		}
	}
	else
	{
		allParticles[x][y] = new Source(type, x, y, temp);
		particleList.push_back(allParticles[x][y]);
		return allParticles[x][y];
	}

	//we didnt have a type, return nullptr
	return nullptr;
}

//overload for default mode
Particle* CreateParticle(ParticleType type, int x, int y, float temp)
{
	return CreateParticle(type, x, y, temp, false);
}

//destroys the particle at the given location and wipes the memory of it
void DestroyParticle(int x, int y)
{
	//check if a particle even exists in the area we want to delete and cancel if there is none
	if (allParticles[x][y] == nullptr)
		return;

	//iterate through all particles in the list
	for (size_t i = 0; i < particleList.size() - 1; i++)
	{
		int tempX, tempY;
		tempX = particleList.at(i)->x;
		tempY = particleList.at(i)->y;
		//if the x/y are the same wipe the particle
		if (tempX == x && tempY == y)
		{
			//clear the vector first
			particleList.erase(particleList.begin() + i);

			//make sure we COMPLETELY wipe the particle, the particle was made by hand via new Particle() and therefore we MUST handle the deletion manually		
			delete allParticles[tempX][tempY];
			allParticles[tempX][tempY] = nullptr;

			return;
		}
	}
}

//used to move pointers of particles around
void MoveParticles(int x1, int y1, int x2, int y2)
{
	//get both the particles we want to move
	std::swap(allParticles[x1][y1], allParticles[x2][y2]);

	//update the internal location data for the first point now that its been moved
	if (allParticles[x2][y2] != nullptr)
	{
		allParticles[x2][y2]->y = y2;
		allParticles[x2][y2]->x = x2;
	}

	//update the internal location data for the second point now that its been moved
	if (allParticles[x1][y1] != nullptr)
	{
		allParticles[x1][y1]->y = y1;
		allParticles[x1][y1]->x = x1;
	}	
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
		MoveParticles(x, y, tempX, y + 1);
		return true;
	}
	//other wise if the particle directly below has less weight then swap particles
	else if (allParticles[tempX][y + 1]->weight != -1)
		if (allParticles[x][y]->weight > allParticles[tempX][y + 1]->weight)
		{
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
		MoveParticles(x, y, tempX, y - 1);
		return true;
	}
	//other wise if the particle directly above has more weight then swap particles
	else if (allParticles[tempX][y - 1]->weight != -1)
		if (allParticles[x][y]->weight < allParticles[tempX][y - 1]->weight)
		{
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

		float calculatedTemperatureChange = calculatedTemperatureDifference * std::abs(allParticles[x1][y1]->thermalConductivity + allParticles[x2][y2]->thermalConductivity) ;


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

Particle::Particle()
{
	Reset();
}

void Particle::Reset()
{
	x = y = 0;
	weight = -1;
	temperature = 0;
	thermalConductivity = 0;
	health = 100;
	type = TYPE_WALL;
}

//default draw code
void Particle::Draw()
{
	SDL_SetRenderDrawColor(mainRenderer, 255, 255, 255, 0);
	SDL_RenderDrawPoint(mainRenderer, x, y);
}

bool Particle::HandleEvents()
{
	//temperature handle
	if (thermalConductivity > 0)
	{
		//values to help with determining what ways to calculate
		int up, down, left, right;
		up = down = y;
		left = right = x;
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


		//check and update each directino that we can
		if (canGoUp)
			EvenOutTemperatures(x, y, x, up);

		if (canGoDown)
			EvenOutTemperatures(x, y, y, down);

		if (canGoLeft)
			EvenOutTemperatures(x, y, left, y);

		if (canGoRight)
			EvenOutTemperatures(x, y, right, y);
	}

	//health check
	if (health <= 0)
	{
		DestroyParticle(x, y);
		return true;
	}

	return false;
}

//check if the particle is at the bottom of the screen and if we need to loop back to the top
bool Particle::CheckIfAtBottom()
{
	//check if at bottom of screen
	if (y == WINDOW_HEIGHT - 1)
	{
		//check if we need to loop the particles from bottom to top
		if (loopScreen)
		{
			if (allParticles[x][0] == nullptr)
				MoveParticles(x, y, x, 0);

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
	if (y == 0)
	{
		//check if we need to loop the particles from bottom to top
		if (loopScreen)
		{
			if (allParticles[x][WINDOW_HEIGHT - 1] == nullptr)
				MoveParticles(x, y, x, WINDOW_HEIGHT - 1);

			//even if we cant move return true as we are stil lat the bottom and cant move else where
			return true;
		}
		else
			return true;//we are not to loop, stop here
	}

	//we are not at the bottom
	return false;
}


//default particle physics
//by default we want to treat everything as sand
void Particle::HandlePhysics()
{
	//make sure we arent already at the bottom level, if we are we dont need to check the physics
	if (CheckIfAtBottom())
		return;

	//check if an object exists under this one
	if (DropParticle(x, y))
		return;

	int left, right, down;
	left = right = x;
	left--;
	right++;
	down = y + 1;

	if (left < 0)
		left = 0;

	if (right > WINDOW_WIDTH - 1)
		right = WINDOW_WIDTH - 1;

	//check if we are surrounded
	if (allParticles[left][down] != nullptr && allParticles[x][down] != nullptr && allParticles[right][down] != nullptr)
	{
		//if we are check if we can move via gravity with the surrounding blocks

		bool canGoLeft, canGoRight;
		canGoLeft = canGoRight = false;

		if (allParticles[left][down]->weight != -1)
			if (allParticles[x][y]->weight < allParticles[left][down]->weight)
				canGoLeft = true;

		if (allParticles[right][down]->weight != -1)
			if (allParticles[x][y]->weight < allParticles[right][down]->weight)
				canGoRight = true;

		if (canGoLeft && canGoRight)
		{
			switch (rand() % 2 + 1)
			{
				//go left
			case 1:
				MoveParticles(left, down, x, y);
				return;

				//go right
			case 2:
				MoveParticles(right, down, x, y);
				return;
			}
		}

		if (canGoLeft)
		{
			MoveParticles(left, down, x, y);
			return;
		}

		if (canGoRight)
		{
			MoveParticles(right, down, x, y);
			return;
		}

		//neither way is less weight and we know that were surrounded, do not run any further code
		return;
	}	
	
	//since there is an object under this one lets check if we can fall to the left or right
	if (x == 0)//check if we are on the left most edge
	{
		//we are on the left most edge, try to drop to the bottom right
		if (allParticles[right][down] == nullptr)
		{
			//update the array
			MoveParticles(x, y, right, down);
			return;
		}
		return;//even if we cant drop, we know that we can not go anywhere so stop here
	}	

	if (x == WINDOW_WIDTH - 1)//making sure were not at the right most edge
	{
		//we are on the right most edge, try to drop to the bottom left
		if (allParticles[left][down] == nullptr)
		{
			//update the array
			MoveParticles(x, y, left, down);
			return;
		}
		return;//even if we cant drop, we know that we can not go anywhere so stop here
	}

	//if the bottom left is empty and the bottom right isnt then drop to the bottom left
	if (allParticles[left][down] == nullptr && allParticles[right][down] != nullptr)
	{
		//update the array
		MoveParticles(x, y, left, down);
		return;
	}
	//if the bottom right is empty and the bottom left isnt the drop to the bottom right
	else if (allParticles[right][down] == nullptr && allParticles[left][down] != nullptr)
	{
		//update the array
		MoveParticles(x, y, right, down);
		return;
	}
	//if both sides are free randomly choose one direction and drop down that side	
	else if (allParticles[right][down] == nullptr && allParticles[left][down] == nullptr)
	{
		int randomNum = rand() % 2 + 1;

		switch (randomNum)
		{
			//go left
		case 1:
			//update the array
			MoveParticles(x, y, left, down);
			return;

			//go right
		case 2:
			//update the array
			MoveParticles(x, y, right, down);
			return;
		}
	}
}

Wall::Wall(int newX, int newY, float newTemperature)
{
	Particle::Reset();
	type = TYPE_WALL;
	x = newX;
	y = newY;
	temperature = newTemperature;
	thermalConductivity = settingThermalConductivity[type];
	health = settingHealth[type];
	weight = settingWeight[type];
}

void Wall::Draw()
{
	SDL_SetRenderDrawColor(mainRenderer, settingColor[type].r, settingColor[type].g, settingColor[type].b, settingColor[type].a);
	SDL_RenderDrawPoint(mainRenderer, x, y);
}

void Wall::HandlePhysics()
{
	//walls have no physics, in the situation soemthing asks it to run its physics simulation do nothing
}

//sand particles

Sand::Sand(int newX, int newY, float newTemperature)
{
	Particle::Reset();
	type = TYPE_SAND;
	x = newX;
	y = newY;
	temperature = newTemperature;
	thermalConductivity = settingThermalConductivity[type];
	health = settingHealth[type];
	weight = settingWeight[type];
	
}

void Sand::Draw()
{
	SDL_SetRenderDrawColor(mainRenderer, settingColor[type].r, settingColor[type].g, settingColor[type].b, settingColor[type].a);
	SDL_RenderDrawPoint(mainRenderer, x, y);
}

void Sand::HandlePhysics()
{
	Particle::HandlePhysics();
}

//water particles
Water::Water(int newX, int newY, float newTemperature)
{
	Particle::Particle();
	type = TYPE_WATER;
	x = newX;
	y = newY;
	temperature = newTemperature;
	thermalConductivity = settingThermalConductivity[type];
	health = settingHealth[type];
	weight = settingWeight[type];
}


void Water::Draw()
{
	SDL_SetRenderDrawColor(mainRenderer, settingColor[type].r, settingColor[type].g, settingColor[type].b, settingColor[type].a);
	SDL_RenderDrawPoint(mainRenderer, x, y);
}

bool Water::HandleEvents()
{
	//handle default events, this includes handling temperature shifts
	if (Particle::HandleEvents())
		return true;

	//check if the water should freeze
	if (temperature < waterFreezePoint)
	{
		int newX = x;
		int newY = y;
		float newTemp = temperature;

		DestroyParticle(x, y);
		Particle* tempParticle = CreateParticle(TYPE_ICE, newX, newY, newTemp);

		return true;
	}
	//check if should turn into steam
	else if (temperature > waterBoilIntoSteamPoint)
	{
		int newX = x;
		int newY = y;
		float newTemp = temperature;

		DestroyParticle(x, y);
		Particle* tempParticle = CreateParticle(TYPE_STEAM, newX, newY, newTemp);

		return true;
	}

	return false;
}

void Water::HandlePhysics()
{
	//make sure we arent already at the bottom level, if we are we dont need to check the physics
	if (CheckIfAtBottom())
		return;

	//check if an object exists under this one
	if (DropParticle(x, y))
		return;

	int left, right, down;
	left = right = x;
	left--;
	right++;
	down = y + 1;

	if (left < 0)
		left = 0;

	if (right > WINDOW_WIDTH - 1)
		right = WINDOW_WIDTH - 1;

	//check if we are surrounded
	if (allParticles[left][y] != nullptr && allParticles[x][down] != nullptr && allParticles[right][y] != nullptr)
	{
		//if we are check if we can move via gravity with the surrounding blocks
		bool canGoLeft, canGoRight;
		canGoLeft = canGoRight = false;

		if (allParticles[left][y]->weight != -1)
			if (allParticles[x][y]->weight < allParticles[left][y]->weight)
				canGoLeft = true;

		if (allParticles[right][y]->weight != -1)
			if (allParticles[x][y]->weight < allParticles[right][y]->weight)
				canGoRight = true;

		//can go either way
		if (canGoLeft && canGoRight)
		{
			switch (rand() % 2 + 1)
			{
				//go left
			case 1:
				MoveParticles(left, y, x, y);
				return;

				//go right
			case 2:
				MoveParticles(right, y, x, y);
				return;
			}
		}

		//can only go left
		if (canGoLeft)
		{
			MoveParticles(left, y, x, y);
			return;
		}

		//can only go right
		if (canGoRight)
		{
			MoveParticles(right, y, x, y);
			return;
		}

		//neither way is less weight and we know that were surrounded, do not run any further code
		return;
	}

	//there is an object under this one, try and move to the left or right
	if (x == 0)//check if we are on the left most edge
	{
		//we are on the left most edge, try to drop to the bottom right
		if (allParticles[right][y] == nullptr)
		{
			//update the array
			MoveParticles(x, y, right, y);
			return;
		}
		return;//even if we cant drop, we know that we can not go anywhere so stop here
	}

	if (x == WINDOW_WIDTH - 1)//making sure were not at the right most edge
	{
		//we are on the right most edge, try to drop to the bottom left
		if (allParticles[left][y] == nullptr)
		{
			//update the array
			MoveParticles(x, y, left, y);
			return;
		}
		return;//even if we cant drop, we know that we can not go anywhere so stop here
	}
	
	//if left is empty only
	if (allParticles[left][y] == nullptr && allParticles[right][y] != nullptr)
		MoveParticles(x, y, left, y);
	//if right is empty only
	else if (allParticles[right][y] == nullptr && allParticles[left][y] != nullptr)
		MoveParticles(x, y, right, y);
	//both ways are free, randomly choose one and move	
	else if (allParticles[right][y] == nullptr && allParticles[left][y] == nullptr)
	{
		int randomNum = rand() % 2 + 1;

		switch (randomNum)
		{
			//go left
		case 1:
			//update the array
			MoveParticles(x, y, left, y);
			return;

			//go right
		case 2:
			//update the array
			MoveParticles(x, y, right, y);
			return;
		}
	}
}

//ice particles
Ice::Ice(int newX, int newY, float newTemperature)
{
	Particle::Reset();
	type = TYPE_ICE;
	x = newX;
	y = newY;
	temperature = newTemperature;
	thermalConductivity = settingThermalConductivity[type];
	health = settingHealth[type];
	weight = settingWeight[type];
}

void Ice::Draw()
{
	SDL_SetRenderDrawColor(mainRenderer, settingColor[type].r, settingColor[type].g, settingColor[type].b, settingColor[type].a);
	SDL_RenderDrawPoint(mainRenderer, x, y);
}

bool Ice::HandleEvents()
{
	//handle default events first, this includes temperature shifting
	if (Particle::HandleEvents())
		return true;

	//check if ice should melt	
	if (temperature > iceMeltPoint)
	{
		int newX = x;
		int newY = y;
		float newTemp = temperature;

		DestroyParticle(x, y);
		Particle* tempParticle = CreateParticle(TYPE_WATER, newX, newY, newTemp);
		return true;
	}

	return false;
}

void Ice::HandlePhysics()
{
	//Ice does not fall, run no physics
}

ThermalFluid::ThermalFluid(int newX, int newY, float newTemperature) : Water(newX, newY, newTemperature)
{
	type = TYPE_THERMALFLUID;
	thermalConductivity = settingThermalConductivity[type];
	health = settingHealth[type];
	weight = settingWeight[type];
}

void ThermalFluid::Draw()
{
	SDL_SetRenderDrawColor(mainRenderer, settingColor[type].r, settingColor[type].g, settingColor[type].b, settingColor[type].a);
	SDL_RenderDrawPoint(mainRenderer, x, y);
}

//use default event handling, nothing else required
bool ThermalFluid::HandleEvents()
{
	Particle::HandleEvents();

	return false;
}

//use default water physics
void ThermalFluid::HandlePhysics()
{
	Water::HandlePhysics();
}

//acid particles
Acid::Acid(int newX, int newY, float newTemperature) : Water(newX, newY, newTemperature)
{
	type = TYPE_ACID;
	thermalConductivity = settingThermalConductivity[type];
	health = settingHealth[type];
	weight = settingWeight[type];
}

void Acid::Draw()
{
	SDL_SetRenderDrawColor(mainRenderer, settingColor[type].r, settingColor[type].g, settingColor[type].b, settingColor[type].a);
	SDL_RenderDrawPoint(mainRenderer, x, y);
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
	up = down = y;
	left = right = x;
	up--;
	down++;
	left--;
	right++;

	bool canDamageUp, canDamageDown, canDamageLeft, canDamageRight;
	canDamageUp = canDamageDown = canDamageLeft = canDamageRight = false;

	//check if blocks exist in each direction and attempt to target it for damage 
	if (up >= 0)
		if (allParticles[x][up] != nullptr)
			if (allParticles[x][up]->type != TYPE_ACID)
				canDamageUp = true;

	if (down <= WINDOW_HEIGHT - 1)
		if (allParticles[x][down] != nullptr)
			if (allParticles[x][down]->type != TYPE_ACID)
					canDamageDown = true;			

	if (left >= 0)
		if (allParticles[left][y] != nullptr)
			if (allParticles[left][y]->type != TYPE_ACID)
				canDamageLeft = true;

	if (right <= WINDOW_WIDTH - 1)
		if (allParticles[right][y] != nullptr)
			if (allParticles[right][y]->type != TYPE_ACID)
				canDamageRight = true;


	//check each direction and attempt to damage any targeted blocks
	if (canDamageUp)	
		if ((rand() % acidDamageChance + 1) == 1)
		{
			allParticles[x][up]->health--;
			health--;
		}

	if (canDamageDown)
		if ((rand() % acidDamageChance + 1) == 1)
		{
			allParticles[x][down]->health--;	
			health--;
		}

	if (canDamageLeft)	
		if ((rand() % acidDamageChance + 1) == 1)
		{
			allParticles[left][y]->health--;	
			health--;
		}

	if (canDamageRight)	
		if ((rand() % acidDamageChance + 1) == 1)
		{
			allParticles[right][y]->health--;
			health--;
		}

	return false;
}

//use default water physics
void Acid::HandlePhysics()
{
	Water::HandlePhysics();
}


//steam particles
Steam::Steam(int newX, int newY, float newTemperature)
{
	Particle::Reset();
	type = TYPE_STEAM;
	x = newX;
	y = newY;
	temperature = newTemperature;
	thermalConductivity = settingThermalConductivity[type];
	health = settingHealth[type];
	weight = settingWeight[type];
}

void Steam::Draw()
{
	SDL_SetRenderDrawColor(mainRenderer, settingColor[type].r, settingColor[type].g, settingColor[type].b, settingColor[type].a);
	SDL_RenderDrawPoint(mainRenderer, x, y);
}

bool Steam::HandleEvents()
{
	//handle default events first, this includes temperature shifting
	if (Particle::HandleEvents())
		return true;

	if (temperature < steamCondensationPoint)
	{
		int newX = x;
		int newY = y;
		float newTemp = temperature;

		DestroyParticle(x, y);
		Particle* tempParticle = CreateParticle(TYPE_WATER, newX, newY, newTemp);
	}

	return false;
}

//custom physics for steam, steam will be the basic gas type
void Steam::HandlePhysics()
{
	int up, left, right;
	up = y;
	left = right = x;
	up--;
	left--;
	right++;

	//check if we are at the top of the screen and if we can loop around
	if (CheckIfAtTop())
		return;

	//attempt to move the particle directly upwards
	if (AscendParticle(x, y, true))
		return;

	//check if we are surrounded
	if (allParticles[left][y] != nullptr && allParticles[right][y] != nullptr)
	{
		//if we are check if we can move via gravity with the surrounding blocks
		bool canGoLeft, canGoRight;
		canGoLeft = canGoRight = false;

		if (allParticles[left][y]->weight != -1)
			if (allParticles[x][y]->weight < allParticles[left][y]->weight)
				canGoLeft = true;

		if (allParticles[right][y]->weight != -1)
			if (allParticles[x][y]->weight < allParticles[right][y]->weight)
				canGoRight = true;

		//can go either way
		if (canGoLeft && canGoRight)
		{
			switch (rand() % 2 + 1)
			{
				//go left
			case 1:
				MoveParticles(left, y, x, y);
				return;

				//go right
			case 2:
				MoveParticles(right, y, x, y);
				return;
			}
		}

		//can only go left
		if (canGoLeft)
		{
			MoveParticles(left, y, x, y);
			return;
		}

		//can only go right
		if (canGoRight)
		{
			MoveParticles(right, y, x, y);
			return;
		}

		//neither way is less weight and we know that were surrounded, do not run any further code
		return;
	}

	//there is an object under this one, try and move to the left or right
	if (x == 0)//check if we are on the left most edge
	{
		//we are on the left most edge, try to drop to the bottom right
		if (allParticles[right][y] == nullptr)
		{
			//update the array
			MoveParticles(x, y, right, y);
			return;
		}
		return;//even if we cant drop, we know that we can not go anywhere so stop here
	}

	if (x == WINDOW_WIDTH - 1)//making sure were not at the right most edge
	{
		//we are on the right most edge, try to drop to the bottom left
		if (allParticles[left][y] == nullptr)
		{
			//update the array
			MoveParticles(x, y, left, y);
			return;
		}
		return;//even if we cant drop, we know that we can not go anywhere so stop here
	}

	//if left is empty only
	if (allParticles[left][y] == nullptr && allParticles[right][y] != nullptr)
		MoveParticles(x, y, left, y);	
	//if right is empty only
	else if (allParticles[right][y] == nullptr && allParticles[left][y] != nullptr)
		MoveParticles(x, y, right, y);	
	//both ways are free, randomly choose one and move	
	else if (allParticles[right][y] == nullptr && allParticles[left][y] == nullptr)
	{
		int randomNum = rand() % 2 + 1;

		switch (randomNum)
		{
			//go left
		case 1:
			//update the array
			MoveParticles(x, y, left, y);
			return;

			//go right
		case 2:
			//update the array
			MoveParticles(x, y, right, y);
			return;
		}
	}
}

//plant particles
Plant::Plant(int newX, int newY, float newTemperature) : Wall(newX, newY, newTemperature)
{
	type = TYPE_PLANT;
	thermalConductivity = settingThermalConductivity[type];
	health = settingHealth[type];
	weight = settingWeight[type];
}


void Plant::Draw()
{
	SDL_SetRenderDrawColor(mainRenderer, settingColor[type].r, settingColor[type].g, settingColor[type].b, settingColor[type].a);
	SDL_RenderDrawPoint(mainRenderer, x, y);	
}

bool Plant::HandleEvents()
{
	//handle default events first
	if (Particle::HandleEvents())
		return true;

	int up, down, left, right;

	up = down = y;
	left = right = x;

	up--;
	down++;
	left--;
	right++;


	bool canGoUp, canGoDown, canGoLeft, canGoRight;
	canGoUp = canGoDown = canGoLeft = canGoRight = false;

	//check up and down
	if (y > 0 && y < WINDOW_HEIGHT - 1)
	{
		//check if can spread upwards
		if (allParticles[x][up] != nullptr)
			if (allParticles[x][up]->type == TYPE_WATER)
			{
				switch (rand() % plantSpreadChance)
				{
				case 0:
					canGoUp = true;
					break;
				}
			}

		//check if cna spread downwards
		if (allParticles[x][down] != nullptr)
			if (allParticles[x][down]->type == TYPE_WATER)
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
	if (x > 0 && x < WINDOW_WIDTH - 1)
	{
		//check if cna spread left
		if (allParticles[left][y] != nullptr)
			if (allParticles[left][y]->type == TYPE_WATER)
			{
				switch (rand() % plantSpreadChance)
				{
				case 0:
					canGoLeft = true;
					break;
				}
			}

		//check if cna spread right
		if (allParticles[right][y] != nullptr)
			if (allParticles[right][y]->type == TYPE_WATER)
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
		int newX = x;
		int newY = up;
		float newTemp = temperature;

		DestroyParticle(x, up);
		Particle* tempParticle = CreateParticle(TYPE_PLANT, newX, newY, newTemp);
	}

	if (canGoDown)
	{
		int newX = x;
		int newY = down;
		float newTemp = temperature;

		DestroyParticle(x, down);
		Particle* tempParticle = CreateParticle(TYPE_PLANT, newX, newY, newTemp);
	}

	if (canGoLeft)
	{
		int newX = left;
		int newY = y;
		float newTemp = temperature;

		DestroyParticle(left, y);
		Particle* tempParticle = CreateParticle(TYPE_PLANT, newX, newY, newTemp);
	}

	if (canGoRight)
	{
		int newX = right;
		int newY = y;
		float newTemp = temperature;

		DestroyParticle(right, y);
		Particle* tempParticle = CreateParticle(TYPE_PLANT, newX, newY, newTemp);
	}

	if (canGoUp || canGoDown || canGoLeft || canGoRight)
		return true;
	else
		return false;
}

//no physics, do not attempt to move at all
void Plant::HandlePhysics()
{

}

//salt particle
Salt::Salt(int newX, int newY, float newTemperature) : Sand(newX, newY, newTemperature)
{
	type = TYPE_SALT;
	thermalConductivity = settingThermalConductivity[type];
	health = settingHealth[type];
	weight = settingWeight[type];
}

void Salt::Draw()
{
	SDL_SetRenderDrawColor(mainRenderer, settingColor[type].r, settingColor[type].g, settingColor[type].b, settingColor[type].a);
	SDL_RenderDrawPoint(mainRenderer, x, y);
}

bool Salt::HandleEvents()
{
	//default event handling
	if (Particle::HandleEvents())
		return true;

	//salt event handling
	int up, down, left, right;

	up = down = y;
	left = right = x;

	up--;
	down++;
	left--;
	right++;

	//up
	if (allParticles[x][up] != nullptr)
		if (allParticles[x][up]->type == TYPE_WATER)
		{
			//get some temporary variables as both particles will be deleted soon and we do not want to possibly call upon a deleted object
			float temp = (allParticles[x][up]->temperature + allParticles[x][y]->temperature) / 2;
			int x1, y1, x2, y2;

			x1 = x;
			y1 = y;
			x2 = x;
			y2 = up;

			DestroyParticle(x1, y1);
			DestroyParticle(x2, y2);

			CreateParticle(TYPE_SALTWATER, x2, y2, temp);
			return true;
		}

	//down
	if (allParticles[x][down] != nullptr)
		if (allParticles[x][down]->type == TYPE_WATER)
		{
			//get some temporary variables as both particles will be deleted soon and we do not want to possibly call downon a deleted object
			float temp = (allParticles[x][down]->temperature + allParticles[x][y]->temperature) / 2;
			int x1, y1, x2, y2;

			x1 = x;
			y1 = y;
			x2 = x;
			y2 = down;

			DestroyParticle(x1, y1);
			DestroyParticle(x2, y2);

			CreateParticle(TYPE_SALTWATER, x2, y2, temp);
			return true;
		}

	//left
	if (allParticles[left][y] != nullptr)
		if (allParticles[left][y]->type == TYPE_WATER)
		{
			//get some temporary variables as both particles will be deleted soon and we do not want to possibly call upon a deleted object
			float temp = (allParticles[left][y]->temperature + allParticles[x][y]->temperature) / 2;
			int x1, y1, x2, y2;

			x1 = x;
			y1 = y;
			x2 = left;
			y2 = y;

			DestroyParticle(x1, y1);
			DestroyParticle(x2, y2);

			CreateParticle(TYPE_SALTWATER, x2, y2, temp);
			return true;
		}

	//right
	if (allParticles[right][y] != nullptr)
		if (allParticles[right][y]->type == TYPE_WATER)
		{
			//get some temporary variables as both particles will be deleted soon and we do not want to possibly call upon a deleted object
			float temp = (allParticles[right][y]->temperature + allParticles[x][y]->temperature) / 2;
			int x1, y1, x2, y2;

			x1 = x;
			y1 = y;
			x2 = right;
			y2 = y;

			DestroyParticle(x1, y1);
			DestroyParticle(x2, y2);

			CreateParticle(TYPE_SALTWATER, x2, y2, temp);
			return true;
		}	

	return false;
}

//use default sand physics
void Salt::HandlePhysics()
{
	Sand::HandlePhysics();	
}

//salt water particle
SaltWater::SaltWater(int newX, int newY, float newTemperature) : Water(newX, newY, newTemperature)
{
	type = TYPE_SALTWATER;
	thermalConductivity = settingThermalConductivity[type];
	health = settingHealth[type];
	weight = settingWeight[type];
}

void SaltWater::Draw()
{
	SDL_SetRenderDrawColor(mainRenderer, settingColor[type].r, settingColor[type].g, settingColor[type].b, settingColor[type].a);
	SDL_RenderDrawPoint(mainRenderer, x, y);
}

bool SaltWater::HandleEvents()
{
	//handle default events, this includes handling temperature shifts
	if (Particle::HandleEvents())
		return true;

	//check if the salt water should freeze
	if (temperature < (waterFreezePoint * saltWaterEventTempMultiplier))
	{
		int newX = x;
		int newY = y;
		float newTemp = temperature;

		DestroyParticle(x, y);
		Particle* tempParticle = CreateParticle(TYPE_SALTICE, newX, newY, newTemp);

		return true;
	}
	//check if should turn into steam
	else if (temperature > (waterBoilIntoSteamPoint * saltWaterEventTempMultiplier))
	{
		int newX = x;
		int newY = y;
		float newTemp = temperature;

		DestroyParticle(x, y);
		Particle* tempParticle = CreateParticle(TYPE_STEAM, newX, newY, newTemp);

		return true;
	}

	return false;
}

//use default sand physics
void SaltWater::HandlePhysics()
{
	Water::HandlePhysics();
}

//salt ice particles
SaltIce::SaltIce(int newX, int newY, float newTemperature)
{
	Particle::Reset();
	type = TYPE_SALTICE;
	x = newX;
	y = newY;
	temperature = newTemperature;
	thermalConductivity = settingThermalConductivity[type];
	health = settingHealth[type];
	weight = settingWeight[type];
}

void SaltIce::Draw()
{
	SDL_SetRenderDrawColor(mainRenderer, settingColor[type].r, settingColor[type].g, settingColor[type].b, settingColor[type].a);
	SDL_RenderDrawPoint(mainRenderer, x, y);
}

bool SaltIce::HandleEvents()
{
	//handle default events first, this includes temperature shifting
	if (Particle::HandleEvents())
		return true;

	//check if ice should melt	
	if (temperature > (iceMeltPoint * saltWaterEventTempMultiplier))
	{
		int newX = x;
		int newY = y;
		float newTemp = temperature;

		DestroyParticle(x, y);
		Particle* tempParticle = CreateParticle(TYPE_SALTWATER, newX, newY, newTemp);
		return true;
	}

	return false;
}

void SaltIce::HandlePhysics()
{
	//salt ice does not fall, run no physics
}

//source particles
//creates copies of its self in the surrounding blocks that are empty
Source::Source(ParticleType newSourceType, int newX, int newY, float newTemperature)
{
	Particle::Reset();
	type = TYPE_SOURCE;
	x = newX;
	y = newY;
	temperature = newTemperature;
	thermalConductivity = settingThermalConductivity[type];
	health = settingHealth[type];
	weight = settingWeight[type];
	sourceType = newSourceType;
}
void Source::Draw()
{
	SDL_SetRenderDrawColor(mainRenderer, settingColor[sourceType].r, settingColor[sourceType].g, settingColor[sourceType].b, settingColor[sourceType].a);
	SDL_RenderDrawPoint(mainRenderer, x, y);
}

bool Source::HandleEvents()
{
	Particle::HandleEvents();

	//Source event handle
	//attempt to create new particles around this particle using its sourceType value

	//values to help with determining what ways to calculate
	int up, down, left, right;
	up = down = y;
	left = right = x;
	up--;
	down++;
	left--;
	right++;

	if (up >= 0)
		if (allParticles[x][up] == nullptr)
			CreateParticle(sourceType, x, up, temperature);

	if (down <= WINDOW_HEIGHT - 1)
		if (allParticles[x][down] == nullptr)
			CreateParticle(sourceType, x, down, temperature);

	if (left >= 0)
		if (allParticles[left][y] == nullptr)
			CreateParticle(sourceType, left, y, temperature);

	if (right <= WINDOW_WIDTH - 1)
		if (allParticles[right][y] == nullptr)
			CreateParticle(sourceType, right, y, temperature);

	return false;
}

//source blocks will not need to move
void Source::HandlePhysics()
{
	//no physics
}
