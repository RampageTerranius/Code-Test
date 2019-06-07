#pragma once

//used to help objects keep track of what they are for rendering purposes as the lists will be using base particles that are using polymorphism to determine how to handle physics
enum ParticleType
{
	TYPE_WALL,
	TYPE_SAND,
	TYPE_WATER,
	TYPE_ICE,
	TYPE_THERMALFLUID,
	TYPE_TOTALTYPES
};

//used to give a name to each particle type
std::string typeNames[] = { "Wall",
							"Sand",
							"Water",
							"Ice",
							"Thermal Fluid",
							"TotalTypes"};//you should never be using this final string, if you are your working with a non existant particale type

class Particle
{
	public:
		Particle();
		void Reset();		
		virtual void Draw();
		bool CheckIfAtBottom();
		virtual void HandleEvents();
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
	Wall(int newX, int newY, int newTemperature);
	void Draw();
	void HandlePhysics();
};

//sand particles
class Sand : public Particle
{
public:
	Sand(int newX, int newY, int newTemperature);
	void Draw();
	void HandlePhysics();
};

//water particles
//always attempts to move downwards if nothing is in way, will always try to move either left or right if cant move down
//can freeze into water
class Water : public Particle
{
public:
	Water(int newX, int newY, int newTemperature);
	void Draw();
	void HandleEvents();
	void HandlePhysics();
};

class ThermalFluid : public Water
{
public:
	ThermalFluid(int newX, int newY, int newTemperature);
	void Draw();
	void HandleEvents();
	void HandlePhysics();
};

//ice particles
//does not move
//can melt into water
class Ice : public Particle
{
public:
	Ice(int newX, int newY, int newTemperature);
	void Draw();
	void HandleEvents();
	void HandlePhysics();
};

//array and vector list handling all data to do with our entities
Particle* allParticles[WINDOW_WIDTH][WINDOW_HEIGHT];
std::vector<Particle*> particleList;

Particle* CreateParticle(ParticleType type, int x, int y, int temp)
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
	}

	//we didnt have a type, return nullptr
	return nullptr;
}

//destroys the particle at the given location and wipes the memory of it
void DestroyParticle(int x, int y)
{
	//iterate through all particles in the list
	for (int i = 0; i < particleList.size() - 1; i++)
	{
		int tempX, tempY;
		tempX = particleList.at(i)->x;
		tempY = particleList.at(i)->y;
		//if the x/y are the same wipe the particle
		if (tempX == x && tempY == y)
		{
			//if there is no data to work with then cancel now
			if (allParticles[tempX][tempY] == nullptr)
				return;

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

bool DropParticle(int x, int y)
{
	if (allParticles[x][y + 1] == nullptr)
	{
		//update the array
		MoveParticles(x, y, x, y + 1);
		return true;
	}
	else if (allParticles[x][y + 1]->weight != -1)
		if (allParticles[x][y]->weight > allParticles[x][y + 1]->weight)
		{
			MoveParticles(x, y, x, y + 1);
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
		float calculatedTemperatureDifference = (std::abs(((allParticles[x1][y1]->temperature - allParticles[x2][y2]->temperature) / allParticles[x1][y1]->temperature) * 100)) / 10;

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

void Particle::HandleEvents()
{
	//temperature handle
	if (thermalConductivity > 0)
	{
		//values to help with determining what ways to calculate
		int up, down, left, right;
		up = x--;
		down = x++;
		left = y--;
		right = y++;

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
			EvenOutTemperatures(x, y, up, y);

		if (canGoDown)
			EvenOutTemperatures(x, y, down, y);

		if (canGoLeft)
			EvenOutTemperatures(x, y, x, left);

		if (canGoRight)
			EvenOutTemperatures(x, y, x, right);
	}

	//health check
	if (health <= 0)	
		DestroyParticle(x, y);
	
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
	if (allParticles[right][down] == nullptr && allParticles[left][down] != nullptr)
	{
		//update the array
		MoveParticles(x, y, right, down);
		return;
	}

	//if both sides are free randomly choose one direction and drop down that side	
	if (allParticles[right][down] == nullptr && allParticles[left][down] == nullptr)
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

Wall::Wall(int newX, int newY, int newTemperature)
{
	Particle::Reset();
	x = newX;
	y = newY;
	weight = wallWeight;
	temperature = (float)newTemperature;
	thermalConductivity = wallThermalConductivity;
	type = TYPE_WALL;
}

void Wall::Draw()
{
	SDL_SetRenderDrawColor(mainRenderer, 87, 87, 87, 0);
	SDL_RenderDrawPoint(mainRenderer, x, y);
}

void Wall::HandlePhysics()
{
	//walls have no physics, in the situation soemthing asks it to run its physics simulation do nothing
}

//sand particles

Sand::Sand(int newX, int newY, int newTemperature)
{
	Particle::Reset();
	x = newX;
	y = newY;
	temperature = (float)newTemperature;
	thermalConductivity = sandThermalConductivity;
	weight = sandWeight;
	type = TYPE_SAND;
}

void Sand::Draw()
{
	SDL_SetRenderDrawColor(mainRenderer, 207, 226, 34, 0);
	SDL_RenderDrawPoint(mainRenderer, x, y);
}

void Sand::HandlePhysics()
{
	Particle::HandlePhysics();
}

//water particles
Water::Water(int newX, int newY, int newTemperature)
{
	Particle::Particle();
	x = newX;
	y = newY;
	temperature = (float)newTemperature;
	thermalConductivity = waterThermalConductivity;
	weight = waterWeight;
	type = TYPE_WATER;
}


void Water::Draw()
{
	SDL_SetRenderDrawColor(mainRenderer, 0, 0, 255, 0);
	SDL_RenderDrawPoint(mainRenderer, x, y);
}

void Water::HandleEvents()
{
	//handle default events, this includes handling temperature shifts
	Particle::HandleEvents();

	//check if the water should freeze
	if (temperature < waterFreezePoint)
	{
		int newX = x;
		int newY = y;
		int newTemp = temperature;
		int newHealth = health;

		DestroyParticle(x, y);
		Particle* tempParticle = CreateParticle(TYPE_ICE, newX, newY, newTemp);
	}
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
	if (allParticles[right][y] == nullptr && allParticles[left][y] != nullptr)
		MoveParticles(x, y, right, y);

	//both ways are free, randomly choose one and move	
	if (allParticles[right][y] == nullptr && allParticles[left][y] == nullptr)
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
Ice::Ice(int newX, int newY, int newTemperature)
{
	Particle::Reset();
	x = newX;
	y = newY;
	temperature = (float)newTemperature;
	thermalConductivity = iceThermalConductivity;
	weight = iceWeight;
	type = TYPE_ICE;
}

void Ice::Draw()
{
	SDL_SetRenderDrawColor(mainRenderer, 100, 255, 255, 0);
	SDL_RenderDrawPoint(mainRenderer, x, y);
}

void Ice::HandleEvents()
{
	//handle default events first, this includes temperature shifting
	Particle::HandleEvents();

	//check if ice should melt
	if (temperature > iceMeltPoint)
	{
		int newX = x;
		int newY = y;
		int newTemp = temperature;
		int newHealth = health;

		DestroyParticle(x, y);
		Particle* tempParticle = CreateParticle(TYPE_WATER, newX, newY, newTemp);
	}
}

void Ice::HandlePhysics()
{
	//Ice does not fall, run no physics
}

ThermalFluid::ThermalFluid(int newX, int newY, int newTemperature) : Water(newX, newY, newTemperature)
{
	thermalConductivity = thermalFluidThermalConductivity;
	weight = thermalFluidWeight;
	type = TYPE_THERMALFLUID;
}

void ThermalFluid::Draw()
{
	SDL_SetRenderDrawColor(mainRenderer, 150, 50, 255, 0);
	SDL_RenderDrawPoint(mainRenderer, x, y);
}

//use default event handling, nothing else required
void ThermalFluid::HandleEvents()
{
	Particle::HandleEvents();
}

//use defautl water physics
void ThermalFluid::HandlePhysics()
{
	Water::HandlePhysics();
}
