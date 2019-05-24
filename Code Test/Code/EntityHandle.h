#pragma once

void CreateParticle(eType type, int x, int y)
{
	//check that we have no entity in this section to begin with
	if (allParticles[x][y] != NULL)
		if (allParticles[x][y]->type != TYPE_NONE)
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
	}
}

void EntityHandle()
{
	//handling entities
	for(int i = 0; i < particleList.size(); i++)
		particleList.at(i)->HandlePhysics();	
}