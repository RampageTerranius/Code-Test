#include "EntityHandle.h"
#include "Particles.h"

//runs code to do with all particles currently loaded
void EntityHandle()
{
	if (!pauseParticles)
	{
		// Handling particle events.
		for (Node* node = particleList.front; node != nullptr; node = node->next)
			allParticles[node->x][node->y]->HandleEvents();

		// Handling physics.
		for (Node* node = particleList.front; node != nullptr; node = node->next)	
			if (!allParticles[node->x][node->y]->locked)
				allParticles[node->x][node->y]->HandlePhysics();		
	}
}