#include "ParticleTypes.h"

ParticleEffect::ParticleEffect()
{
	effectData = std::vector<std::string>();
	effectType = EFFECT_NULL;
}

ParticleType::ParticleType()
{
	name = "NO NAME";
	movementType = MOVEMENTTYPE_IMMOBILE;
	weight = -1;
	startingHealth = -1;
	thermalConductivity = 0;

	ascendRate = 0;
	descendRate = 0;
	sidewardsRate = 0;
	noMovementRate = 100;

	Flammability = 0;

	colorType = COLORTYPE_RGB;
	R = 0;
	G = 0;
	B = 0;

	particleEffects = std::vector<ParticleEffect>();
}