#pragma once

void CreateParticle(ParticleType type, int x, int y)
{
	//check that we have no entity in this section to begin with
	if (allParticles[x][y] != nullptr)
		return;

	//make sure were not trying to create an entity off screen
	if (x < 0 || x > WINDOW_WIDTH - 1 || y < 0 || y > WINDOW_HEIGHT - 1)
		return;

	//check what type of entity we need to create and assign it the required data as well as update the entityexists list
	switch (type)
	{
	case TYPE_WALL:
		allParticles[x][y] = new Wall(x, y);
		particleList.push_back(allParticles[x][y]);
		break;

	case TYPE_SAND:
		allParticles[x][y] = new Sand(x, y);
		particleList.push_back(allParticles[x][y]);
		break;

	case TYPE_WATER:
		allParticles[x][y] = new Water(x, y);
		particleList.push_back(allParticles[x][y]);
		break;
	}
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
			//clear the vector first
			particleList.erase(particleList.begin() + i);

			//make sure we COMPLETELY wipe the particle, the particle was made by hand via new Particle() and therefore we MUST handle the deletion manually
			delete allParticles[tempX][tempY];
			allParticles[tempX][tempY] = nullptr;
			return;
		}
	}
}

void EntityHandle()
{
	//handling entities
	for(int i = 0; i < particleList.size(); i++)
		particleList.at(i)->HandlePhysics();	
}