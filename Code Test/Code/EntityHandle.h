#pragma once
//runs code to do with all particles currently loaded
void EntityHandle()
{
	if (!pauseParticles)
	{
		for (Particle* i : particleList)
			i->HandleEvents();

		//handling entities
		for (Particle* i : particleList)
			i->HandlePhysics();
	}
}