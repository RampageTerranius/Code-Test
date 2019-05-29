#pragma once

//used to help objects keep track of what they are for rendering purposes as the lists will be using base particles that are using polymorphism to determine how to handle physics
enum eType
{
	TYPE_WALL,
	TYPE_SAND,
	TYPE_WATER,
	TYPE_TOTALTYPES
};

class Particle
{
	public:
		Particle();
		void Reset();
		virtual void Draw();
		virtual void HandlePhysics();

		int x, y;
		int weight;
		eType type;
};

//array and vector list handling all data to do with our entities
Particle* allParticles[WINDOW_WIDTH][WINDOW_HEIGHT];
std::vector<Particle*> particleList;

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

Particle::Particle()
{
	Reset();
}

void Particle::Reset()
{
	x = y = 0;
	weight = -1;
	type = TYPE_WALL;
}

//default draw code
void Particle::Draw()
{
	SDL_SetRenderDrawColor(mainRenderer, 255, 255, 255, 0);
	SDL_RenderDrawPoint(mainRenderer, x, y);
}

//default particle physics
//by default we want to treat everything as sand
void Particle::HandlePhysics()
{
	//make sure we arent already at the bottom level, if we are we dont need to check the physics
	if (y == WINDOW_HEIGHT - 1)
		return;

	//check if an object exists under this one
	if (DropParticle(x, y))
		return;

	int left, right;
	left = right = x;
	left--;
	right++;

	if (left < 0)
		left = 0;

	if (right > WINDOW_WIDTH - 1)
		right = WINDOW_WIDTH - 1;

	//check if we are surrounded
	if (allParticles[left][y - 1] != nullptr && allParticles[x][y - 1] != nullptr && allParticles[right][y - 1] != nullptr)
	{
		//if we are check if we can move via gravity with the surrounding blocks

		bool canGoLeft, canGoRight;
		canGoLeft = canGoRight = false;

		if (allParticles[left][y - 1]->weight != -1)
			if (allParticles[x][y]->weight < allParticles[left][y - 1]->weight)
				canGoLeft = true;

		if (allParticles[right][y - 1]->weight != -1)
			if (allParticles[x][y]->weight < allParticles[right][y - 1]->weight)
				canGoRight = true;

		if (canGoLeft && canGoRight)
		{
			switch (rand() % 2 + 1)
			{
				//go left
			case 1:
				MoveParticles(left, y - 1, x, y);
				return;

				//go right
			case 2:
				MoveParticles(right, y - 1, x, y);
				return;
			}
		}

		if (canGoLeft)
		{
			MoveParticles(left, y - 1, x, y);
			return;
		}

		if (canGoRight)
		{
			MoveParticles(right, y - 1, x, y);
			return;
		}

		//neither way is less weight and we know that were surrounded, do not run any further code
		return;
	}

	
	
	//since there is an object under this one lets check if we can fall to the left or right
	if (x == 0)//check if we are on the left most edge
	{
		//we are on the left most edge, try to drop to the bottom right
		if (allParticles[x + 1][y + 1] == nullptr)
		{
			//update the array
			MoveParticles(x, y, x + 1, y + 1);
			return;
		}
		return;//even if we cant drop, we know that we can not go anywhere so stop here
	}	

	if (x == WINDOW_WIDTH - 1)//making sure were not at the right most edge
	{
		//we are on the right most edge, try to drop to the bottom left
		if (allParticles[x - 1][y + 1] == nullptr)
		{
			//update the array
			MoveParticles(x, y, x - 1, y + 1);
			return;
		}
		return;//even if we cant drop, we know that we can not go anywhere so stop here
	}

	//if the bottom left is empty and the bottom right isnt then drop to the bottom left
	if (allParticles[x - 1][y + 1] == nullptr && allParticles[x + 1][y + 1] != nullptr)
	{
		//update the array
		MoveParticles(x, y, x - 1, y + 1);
		return;
	}

	//if the bottom right is empty and the bottom left isnt the drop to the bottom right
	if (allParticles[x + 1][y + 1] == nullptr && allParticles[x - 1][y + 1] != nullptr)
	{
		//update the array
		MoveParticles(x, y, x + 1, y + 1);
		return;
	}

	//if both sides are free randomly choose one direction and drop down that side	
	if (allParticles[x + 1][y + 1] == nullptr && allParticles[x - 1][y + 1] == nullptr)
	{
		int randomNum = rand() % 2 + 1;

		switch (randomNum)
		{
			//go left
		case 1:
			//update the array
			MoveParticles(x, y, x - 1, y + 1);
			return;

			//go right
		case 2:
			//update the array
			MoveParticles(x, y, x + 1, y + 1);
			return;
		}
	}
}

//wall particles
class Wall : public Particle
{
public:
	Wall(int newX, int newY);
	void Draw();
	void HandlePhysics();
};

Wall::Wall(int newX, int newY)
{
	x = newX;
	y = newY;
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
class Sand : public Particle
{
public:
	Sand(int newX, int newY);
	void Draw();
	void HandlePhysics();
};

Sand::Sand(int newX, int newY)
{
	x = newX;
	y = newY;
	weight = 2;
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
//always attempts to move downwards if nothing is in way, will always try to move either left or right if cant move down
class Water : public Particle
{
public:
	Water(int newX, int newY);
	void Draw();
	void HandlePhysics();
};

Water::Water(int newX, int newY)
{
	x = newX;
	y = newY;
	weight = 1;
	type = TYPE_WATER;
}

void Water::Draw()
{
	SDL_SetRenderDrawColor(mainRenderer, 0, 0, 125, 0);
	SDL_RenderDrawPoint(mainRenderer, x, y);
}

void Water::HandlePhysics()
{
	//make sure we arent already at the bottom level, if we are we dont need to check the physics
	if (y == WINDOW_HEIGHT - 1)
		return;

	//check if an object exists under this one
	if (DropParticle(x, y))
		return;

	int left, right;
	left = right = x;
	left--;
	right++;

	if (left < 0)
		left = 0;

	if (right > WINDOW_WIDTH - 1)
		right = WINDOW_WIDTH - 1;

	//check if we are surrounded
	if (allParticles[left][y] != nullptr && allParticles[x][y - 1] != nullptr && allParticles[right][y] != nullptr)
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

		//can ohly go right
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
		if (allParticles[x + 1][y] == nullptr)
		{
			//update the array
			MoveParticles(x, y, x + 1, y);
			return;
		}
		return;//even if we cant drop, we know that we can not go anywhere so stop here
	}

	if (x == WINDOW_WIDTH - 1)//making sure were not at the right most edge
	{
		//we are on the right most edge, try to drop to the bottom left
		if (allParticles[x - 1][y] == nullptr)
		{
			//update the array
			MoveParticles(x, y, x - 1, y);
			return;
		}
		return;//even if we cant drop, we know that we can not go anywhere so stop here
	}
	
	{
		int randomNum = rand() % 2 + 1;

		switch (randomNum)
		{
			//go left
		case 1:
			//update the array
			if (allParticles[x-1][y] == nullptr)
				MoveParticles(x, y, x - 1, y);
			return;

			//go right
		case 2:
			//update the array
			if (allParticles[x + 1][y] == nullptr)
				MoveParticles(x, y, x + 1, y);
			return;
		}
	}
}
