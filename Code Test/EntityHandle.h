#pragma once

enum eType
{
	TYPE_NONE,
	TYPE_PLAYER,
	TYPE_WALL
};

class Entity
{	
	public:
		int x, y;
		float velX, velY,
			velXChangePerTick, velYChangePerTick,
			maxVelX, maxVelY;
		bool velXAttemptToCenter, velYAttempToCenter,
			 velXLock, velYLock;
		eType type;
		bool moves;		

		Entity();
};

Entity::Entity()
{
	x = y = velX = velY = velXChangePerTick = velYChangePerTick = maxVelX = maxVelY = 0;
	type = TYPE_NONE;
	moves = false;
	velXAttemptToCenter = velYAttempToCenter = false;
}

std::vector<Entity> entityList;

int AddEntity(Entity e)
{
	entityList.push_back(e);
	return entityList.size() - 1;
}

void EntityHandle()
{
	//handling velocity
	for(int i = 0; i < entityList.size(); i++)
	{
		Entity* e = &entityList.at(i);

		//check if the entity is allowed to move
		if (e->moves)
		{	
			//add extra velocity or remove extra velocity as needed			
			if (!e->velXAttemptToCenter)
				e->velX += e->velXChangePerTick;	
			else if (!e->velXLock)
			{
				if (e->velX > 0)
				{
					e->velX -= e->velXChangePerTick;
					if (e->velX < 0)
						e->velX = 0;
				}
				else if (e->velX < 0)
				{
					e->velX += -e->velXChangePerTick;
					if (e->velX > 0)
						e->velX = 0;
				}
			}


			e->velY += e->velYChangePerTick;

			//make sure the velocity has not gone over the limit
			if (e->velX > e->maxVelX)
				e->velX = e->maxVelX;
			else if ((e->velX < -e->maxVelX))
				e->velX = -e->maxVelX;

			
			if (e->velY > e->maxVelY)
				e->velY = e->maxVelY;
			else if ((e->velY < -e->maxVelY))
				e->velY = -e->maxVelY;

			//check if we need to move left or right
			if (e->velX != 0)
				e->x += e->velX;

			//check if we need to move up or down
			if (e->velY != 0)
				e->y += e->velY;

			//make sure player cant leave screen
			if (e->x < 0)
				e->x = 0;
			else if (e->x > WINDOW_WIDTH - 10)
				e->x = WINDOW_WIDTH - 10;

			if (e->y < 0)
				e->y = 0;
			else if (e->y > WINDOW_HEIGHT - 10)
			{
				e->y = WINDOW_HEIGHT - 10;
				playerData.hasJumped = false;
			}
		}		
	}
}