#pragma once

enum eType
{
	TYPE_NONE,
	TYPE_WALL,
	TYPE_SAND,
};

class Entity
{	
	public:
		int x, y;
		eType type;

		Entity();
};

Entity::Entity()
{
	x = y = 0;
	type = TYPE_NONE;
}

std::vector<Entity> entityList;

int AddEntity(Entity e)
{
	entityList.push_back(e);
	return entityList.size() - 1;
}

void CreateEntity(eType type, int x, int y)
{
	//check that we have no entity in this section to begin with
	if (EntityExists[x][y])
		return;

	Entity newEntity;//entity for use with upcoming switch

	//check what type of entity we need to create and assign it the required data as well as update the entityexists list
	switch (type)
	{
	case TYPE_WALL:		
		newEntity.x = x;
		newEntity.y = y;
		newEntity.type = type;

		AddEntity(newEntity);
		EntityExists[x][y] = true;
		break;

	case TYPE_SAND:
		newEntity.x = x;
		newEntity.y = y;
		newEntity.type = type;

		AddEntity(newEntity);
		EntityExists[x][y] = true;
		break;
	}
}

void EntityHandle()
{
	//handling entities
	for(int i = 0; i < entityList.size(); i++)
	{
		//make sure were directly handling the object itself
		Entity* e = &entityList.at(i);

		//check what type of object we are working with and act accordingly
		switch (e->type)
		{
		case TYPE_SAND:
			if (e->y < WINDOW_HEIGHT - 1)//make sure we arent already at the bottom level
			{
				//check if an object exists under this one
				if (!EntityExists[e->x][e->y + 1])
				{
					//update the entityexists table
					EntityExists[e->x][e->y] = false;
					EntityExists[e->x][e->y + 1] = true;

					//update the entity
					e->y += 1;
					break;
				}

				//since there is an object under this one lets check if we can fall to the left or right
				if (e->x != 0)//making sure were not at the left most edge
				{
					//if the bottom left is empty and the bottom right isnt then drop to the bottom left
					if (!EntityExists[e->x - 1][e->y + 1] && EntityExists[e->x + 1][e->y + 1])
					{
						//update the entityexists table
						EntityExists[e->x][e->y] = false;
						EntityExists[e->x - 1][e->y + 1] = true;

						//update the entity
						e->y += 1;
						e->x -= 1;
						break;
					}
				}

				if (e->x != WINDOW_WIDTH - 1)//making sure were not at the right most edge
				{
					//if the bottom right is empty and the bottom left isnt the drop to the bottom right
					if (!EntityExists[e->x + 1][e->y + 1] && EntityExists[e->x - 1][e->y + 1])
					{
						//update the entityexists table
						EntityExists[e->x][e->y] = false;
						EntityExists[e->x + 1][e->y + 1] = true;

						//update the entity
						e->y += 1;
						e->x += 1;
						break;
					}
				}
				//both sides are free, randomly choose one
				if (!EntityExists[e->x + 1][e->y + 1] && !EntityExists[e->x - 1][e->y + 1])
				{
					//check if were on an edge
					if (e->x == 0)//if were on the left most edge then drop to the right
					{
						e->y += 1;
						e->x += 1;
						break;
					} else if (e->x == WINDOW_WIDTH - 1)//if were on the right most edge then drop to the left
					{
						e->y += 1;
						e->x -= 1;
						break;
					}

					//we are nto on either edge so randomly decide which way to drop
					int randomNum = rand() % 2 + 1;

					switch (randomNum)
					{
						//go left
					case 1:
						e->y += 1;
						e->x -= 1;

						break;

						//go right
					case 2:
						e->y += 1;
						e->x += 1;
						break;
					}



				}
			}	
			break;

		default:

			break;
		}
	}
}