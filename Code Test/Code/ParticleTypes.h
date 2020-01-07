#pragma once

#include <vector>
#include <string>

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
	EFFECT_OVERRIDE_NEIGHBOUR_TYPE_WITH_SELF,
	EFFECT_LOSE_HP_PER_TICK,
	EFFECT_NEIGHBOUR_PARTICLES_BECOME_SAME_TEMP_OVER_TIME
};

class ParticleEffect
{
public:
	ParticleEffect();
	ParticleEffectType effectType;
	std::vector<std::string> effectData;
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