#include "EntityHandle.h"
#include "Particles.h"

//runs code to do with all particles currently loaded
void EntityHandle()
{
	if (!pauseParticles)
	{
		for (Node* node = particleList.front; node != nullptr; node = node->next)
			allParticles[node->x][node->y]->HandleEvents();

		//handling entities
		for (Node* node = particleList.front; node != nullptr; node = node->next)		
				allParticles[node->x][node->y]->HandlePhysics();		
	}
}