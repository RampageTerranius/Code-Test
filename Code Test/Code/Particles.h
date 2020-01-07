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
	EFFECT_DELETE_NEIGHBOURS,
	EFFECT_CREATE_PARTICLE_IN_SURROUNDING,
	EFFECT_DAMAGE_NEIGHBOURS,
	EFFECT_BURN_NEIGHBOURS,
	EFFECT_MERGE_WITH_OTHER_PARTICLE_TYPE,
	EFFECT_CHANGE_TYPE_AT_BELOW_TEMPERATURE,
	EFFECT_CHANGE_TYPE_AT_ABOVE_TEMPERATURE,
	EFFECT_SPREAD_TO_NEIGHBOURS,
	EFFECT_OVERRIDE_NEIGHBOUR_TYPE_WITH_SELF
};

class ParticleEffect
{
public:
	ParticleEffectType effectType;
	std::vector<std::string> effectData;
};

enum MovementType
{
	MOVEMENTTYPE_IMMOBILE,
	MOVEMENTTYPE_PILE,
	MOVEMENTTYPE_LIQUID,
	MOVEMENTTYPE_AIRBORN,
};

enum ColorType
{
	COLORTYPE_RGB,
	COLORTYPE_HEAT,
	COLORTYPE_ACTIVE,
	COLORTYPE_RANDOMIZED
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

	ColorType colorType;
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