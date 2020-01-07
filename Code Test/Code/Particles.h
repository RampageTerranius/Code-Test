#pragma once

#include "Globals.h"
#include "ParticleTypes.h"

#include <SDL.h>

#include <string>
#include <vector>
#include <iostream>

extern std::vector<ParticleType> ParticleTypes;

class Particle
{
	public:
		Particle(ParticleType& newType, int newX, int newY, float newTemperature);
		void Reset();		
		void Draw();
		bool CheckIfAtBottom();
		bool CheckIfAtTop();
		void HandleEvents();
		void HandlePhysics();

		SDL_Point point;
		bool isSource;
		int health;
		float temperature;
		ParticleType* type;

		bool locked;
};

void CreateParticle(std::string type, int x, int y, float temp, bool asSource);
void CreateParticle(std::string type, int x, int y, float temp);

void DestroyParticle(int x, int y);
void MoveParticles(int x1, int y1, int x2, int y2);
bool DropParticle(int x, int y, bool randomize);
bool DropParticle(int x, int y);
bool AscendParticle(int x, int y, bool randomize);
void EvenOutTemperatures(int x1, int y1, int x2, int y2);

extern Particle* allParticles[WINDOW_WIDTH][WINDOW_HEIGHT];