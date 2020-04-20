#include "EntityHandle.h"
#include "Particles.h"

// Runs code to do with all particles currently loaded.
void EntityHandle()
{
	if (!pauseParticles || tempUnpause)
	{
		// TODO: switch across to a chunk based system to lessen cycle time, look towards QuadTree implementation.
		// Handling Events.		
		for (int n = WINDOW_HEIGHT - 1; n >= 0; n--)
			for (int i = WINDOW_WIDTH - 1; i >= 0; i--)
			{
				Particle* p = allParticles[i][n];

				if (p != nullptr)
					if (p->eventGate == currentEventGate)
					{
						p->HandleEvents();
						p->eventGate = !currentEventGate;
					}
			}				

		// Handling physics.
		for (int n = WINDOW_HEIGHT - 1; n >= 0; n--)
			for (int i = WINDOW_WIDTH - 1; i >= 0; i--)
			{
				Particle* p = allParticles[i][n];

				if (p != nullptr)
				{
					if (!p->locked)
						if (p->physicsGate == currentPhysicsGate)						
							p->HandlePhysics();						

					p->physicsGate = !currentPhysicsGate;
				}
			}

		currentPhysicsGate = !currentPhysicsGate;
		currentEventGate = !currentEventGate;

		tempUnpause = false;
	}
}