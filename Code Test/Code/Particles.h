#pragma once

#include "Globals.h"
#include "ParticleTypes.h"

#include <SDL.h>

#include <string>
#include <vector>
#include <iostream>

enum ParticleEffectType
{
	EFFECT_NULL,
	EFFECT_DELETE_SURROUNDING,
	EFFECT_CREATE_PARTICLE_IN_SURROUNDING,
	EFFECT_DAMAGE_SURROUNDING,
	EFFECT_DAMAGE_SURROUNDING_INCLUDING_SAME_TYPE,
	EFFECT_BURN_SURROUNDING,
	EFFECT_MERGE_WITH_OTHER_PARTICLE_TYPE,
	EFFECT_RANDOMIZED_COLORS_PER_PRINT,
	EFFECT_RANDOMIZED_COLORS_ON_CREATION,
	EFFECT_CHANGE_TYPE_AT_LOW_TEMPERATURE,
	EFFECT_CHANGE_TYPE_AT_HIGH_TEMPERATURE
};

class ParticleEffect
{
public:
	ParticleEffectType effectType;
	std::string effectData;
};

enum MovementType
{
	MOVEMENTTYPE_IMMOBILE,
	MOVEMENTTYPE_PILE,
	MOVEMENTTYPE_LIQUID,
	MOVEMENTTYPE_AIRBORN,
};

class ParticleType
{
public:
	ParticleType();

	std::string name;
	MovementType movementType;
	int weight;
	int startingHealth;
	float thermalConductivity;

	int ascendRate;
	int descendRate;
	int sidewardsRate;
	int noMovementRate;

	int Flammability;

	int R;
	int G;
	int B;

	std::vector<ParticleEffect> particleEffects;
};

extern std::vector<ParticleType> ParticleTypes;

class Particle
{
	public:
		Particle(ParticleType newType, int newX, int newY, float newTemperature);
		void Reset();		
		void Draw();
		bool CheckIfAtBottom();
		bool CheckIfAtTop();
		bool HandleEvents();
		void HandlePhysics();

		SDL_Point point;
		bool isSource;
		int health;
		float temperature;
		ParticleType* type;

		bool locked;
};

extern Particle* allParticles[WINDOW_WIDTH][WINDOW_HEIGHT];