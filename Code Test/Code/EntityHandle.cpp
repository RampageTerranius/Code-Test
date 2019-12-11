#include "EntityHandle.h"
#include "Particles.h"

// Runs code to do with all particles currently loaded.
void EntityHandle()
{
	if (!pauseParticles)
	{
		// Handling Events.
		for (int i = WINDOW_WIDTH - 1; i >= 0; i--)
			for (int n = WINDOW_HEIGHT - 1; n >= 0; n--)
				if (allParticles[i][n] != nullptr)
						allParticles[i][n]->HandleEvents();

		// Handling physics.
		for (int i = WINDOW_WIDTH - 1; i >= 0; i--)
			for (int n = WINDOW_HEIGHT - 1; n >= 0; n--)
				if (allParticles[i][n] != nullptr)
					if (!allParticles[i][n]->locked)
						allParticles[i][n]->HandlePhysics();		
	}
}