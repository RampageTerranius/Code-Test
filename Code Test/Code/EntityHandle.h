#pragma once
//runs code to do with all particles currently loaded
void EntityHandle()
{
	if (!pauseParticles)
	{
		for (auto i : particleList)
			i->HandleEvents();

		//handling entities
		for (auto i : particleList)
			i->HandlePhysics();
	}
}