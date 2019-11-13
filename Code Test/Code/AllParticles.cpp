#include "AllParticles.h"
#include "Particles.h"

AllParticle allParticles = AllParticle();

AllParticle::AllParticle()
{
	particleList.reserve(WINDOW_WIDTH * WINDOW_HEIGHT);	
}

void AllParticle::MoveParticles(int x1, int y1, int x2, int y2)
{
	// Move the pointer between the two locations without moving the actual particle itsself.
	Particle* temp = allParticles[x1][y1];
	allParticles[x1][y1] = allParticles[x2][y2];
	allParticles[x2][y2] = temp;
}

void AllParticle::AddParticle(Particle p, int x, int y)
{
	particleList.push_back(p);

	// Push the newly created particle back into the allParticles pointer array
	allParticles[x][y] = &particleList[particleList.size() - 1];

	allParticles[x][y]->ID = particleList.size();
}

void AllParticle::DeleteParticle(int x, int y)
{
	particleList.erase(particleList.begin() + allParticles[x][y]->ID);

	allParticles[x][y] = nullptr;
}