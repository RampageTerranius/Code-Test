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

//used to move pointers of particles around
void MoveParticles(int x1, int y1, int x2, int y2)
{
	//get both the particles we want to move
	Particle* p1 = allParticles[x1][y1];
	Particle* p2 = allParticles[x2][y2];

	//wipe pointers ahead of time
	allParticles[x1][y1] = NULL;
	allParticles[x2][y2] = NULL;

	//if point one was not null we need to move it to point two
	if (p1 != NULL)
	{
		allParticles[x2][y2] = p1;
		allParticles[x2][y2]->y = y2;
		allParticles[x2][y2]->x = x2;
	}

	//if point two was not null we need to move it to point one
	if (p2 != NULL)
	{
		allParticles[x1][y1] = p2;
		allParticles[x1][y1]->y = y1;
		allParticles[x1][y1]->x = x1;
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

	//for efficiency reasons we will make sure we cna ACTUALLY move before doing any further calculations
	//create some temporary ints to work with
	int a, b, c;
	a = b = c = x;

	a--;//used as left
	c++;//used as right

	if (a < 0)
		a = 0;//if we are out of screen fix

	if (c > WINDOW_WIDTH - 1)
		c = WINDOW_WIDTH - 1;//if we are out of screen fix

	//check if we cna ACTUALLY move, cancel physics if we can not here immediately
	if (allParticles[a][y + 1] != NULL && allParticles[b][y + 1] != NULL && allParticles[c][y + 1] != NULL)
		return;


	//check if an object exists under this one
	if (allParticles[x][y + 1] == NULL)
	{
		//update the array
		MoveParticles(x, y, x, y + 1);
		return;
	}


	
	//since there is an object under this one lets check if we can fall to the left or right
	if (x == 0)//check if we are on the left most edge
	{
		//we are on the left most edge, try to drop to the bottom right
		if (allParticles[x + 1][y + 1] == NULL)
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
		if (allParticles[x - 1][y + 1] == NULL)
		{
			//update the array
			MoveParticles(x, y, x - 1, y + 1);
			return;
		}
		return;//even if we cant drop, we know that we can not go anywhere so stop here
	}

	//if the bottom left is empty and the bottom right isnt then drop to the bottom left
	if (allParticles[x - 1][y + 1] == NULL && allParticles[x + 1][y + 1] != NULL)
	{
		//update the array
		MoveParticles(x, y, x - 1, y + 1);
		return;
	}

	//if the bottom right is empty and the bottom left isnt the drop to the bottom right
	if (allParticles[x + 1][y + 1] == NULL && allParticles[x - 1][y + 1] != NULL)
	{
		//update the array
		MoveParticles(x, y, x + 1, y + 1);
		return;
	}

	//if both sides are free randomly choose one direction and drop down that side	
	if (allParticles[x + 1][y + 1] == NULL && allParticles[x - 1][y + 1] == NULL)
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