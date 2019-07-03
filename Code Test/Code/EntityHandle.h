#pragma once
//runs code to do with all particles currently loaded
void EntityHandle()
{
	if (!globals.pauseParticles)
	{
		for (Particle* i : globals.particleList)
			i->HandleEvents();

		//handling entities
		for (Particle* i : globals.particleList)
			i->HandlePhysics();
	}
}