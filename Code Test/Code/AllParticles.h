#pragma once

#include <vector>

#include "Particles.h"

#include "Globals.h"

class AllParticle
{
public:
	AllParticle();

	Particle* allParticles[WINDOW_WIDTH][WINDOW_HEIGHT];
	std::vector<Particle> particleList;

	void MoveParticles(int x1, int y1, int x2, int y2);
	void AddParticle(Particle p, int x, int y);
	void DeleteParticle(int x, int y);
};