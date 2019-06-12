#pragma once
//runs code to do with all particles currently loaded
void EntityHandle()
{
	if (!pauseParticles)
	{
		for (size_t i = 0; i < particleList.size(); i++)
			particleList.at(i)->HandleEvents();

		//handling entities
		for (size_t i = 0; i < particleList.size(); i++)
			particleList.at(i)->HandlePhysics();
	}
}