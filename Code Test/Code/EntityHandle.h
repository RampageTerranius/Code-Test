#pragma once
//runs code to do with all particles currently loaded
void EntityHandle()
{
	for (int i = 0; i < particleList.size(); i++)
		particleList.at(i)->HandleEvents();

	//handling entities
	for(int i = 0; i < particleList.size(); i++)
		particleList.at(i)->HandlePhysics();	
}