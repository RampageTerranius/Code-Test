#pragma once

//used to help objects keep track of what they are for rendering purposes as the lists will be using base particles that are using polymorphism to determine how to handle physics
enum eType
{
	TYPE_NONE,
	TYPE_WALL,
	TYPE_SAND	
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

Particle::Particle()
{
	Reset();
}

void Particle::Reset()
{
	x = y = 0;
	weight = -1;
	type = TYPE_NONE;
}

//default draw code, we do not want to draw anything if its a base particle
void Particle::Draw()
{

}

//default particle physics
//by default we want to treat everything as sand
void Particle::HandlePhysics()
{
	//make sure we arent already at the bottom level, if we are we dont need to check the physics
	if (y == WINDOW_HEIGHT - 1)
		return;

	//check if an object exists under this one
	if (allParticles[x][y + 1] == NULL)
	{
		//update the array
		allParticles[x][y + 1] = this;
		allParticles[x][y + 1]->y = this->y + 1;
			
		allParticles[x][y] = NULL;
		return;
	}
	
	//since there is an object under this one lets check if we can fall to the left or right
	if (x == 0)//check if we are on the left most edge
	{
		//we are on the left most edge, try to drop to the bottom right
		if (allParticles[x + 1][y + 1] == NULL)
		{
			//update the array
			y += 1;
			x += 1;
			allParticles[x + 1][y + 1] = this;
			
			allParticles[x][y] = NULL;
			return;
		}
		return;//even if we cant drop, we know that we can not go anywhere so stop here
	}		

	if (x == WINDOW_WIDTH - 1)//making sure were not at the right most edge
	{
		//we are on the right most edge, try to drop to the bottom left
		if (allParticles[x - 1][y + 1] == NULL)
		{
			//update the array
			y += 1;
			x -= 1;
			allParticles[x - 1][y + 1] = this;			
			allParticles[x][y] = NULL;
			return;
		}
		return;//even if we cant drop, we know that we can not go anywhere so stop here
	}

	//if the bottom left is empty and the bottom right isnt then drop to the bottom left
	if (allParticles[x - 1][y + 1] == NULL && allParticles[x + 1][y + 1] != NULL)
	{
		//update the array
		y += 1;
		x -= 1;
		allParticles[x - 1][y + 1] = this;
		
		allParticles[x][y] = NULL;
		return;
	}

	//if the bottom right is empty and the bottom left isnt the drop to the bottom right
	if (allParticles[x + 1][y + 1] == NULL && allParticles[x - 1][y + 1] != NULL)
	{
		//update the array
		y += 1;
		x += 1;
		allParticles[x + 1][y + 1] = this;		
		allParticles[x][y] = NULL;
		return;
	}

	//at this stage we know that both sides are free AND we are not on the edge of the screen, randomly choose one direction and drop down that side	
	int randomNum = rand() % 2 + 1;

	switch (randomNum)
	{
		//go left
	case 1:
		//update the array
		y += 1;
		x -= 1;
		allParticles[x - 1][y + 1] = this;

		allParticles[x][y] = NULL;
		return;

		//go right
	case 2:
		//update the array
		y += 1;
		x += 1;
		allParticles[x + 1][y + 1] = this;

		allParticles[x][y] = NULL;
		return;
	}
}


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
	weight = -1;
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
	weight = 1;
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
	allParticles[0][0] = allParticles[0][0];
}
