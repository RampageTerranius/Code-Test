#pragma once

#include "Globals.h"
#include "ParticleTypes.h"

#include <SDL.h>

#include <string>
#include <vector>
#include <iostream>

class Particle
{
	public:
		Particle(ParticleType newType, int newX, int newY, float newTemperature);
		void Reset();		
		virtual void Draw();
		bool CheckIfAtBottom();
		bool CheckIfAtTop();
		virtual bool HandleEvents();
		virtual void HandlePhysics();

		SDL_Point point;
		int weight;
		int health;
		float temperature;
		float thermalConductivity;
		ParticleType type;

		bool locked;
};

extern Particle* allParticles[WINDOW_WIDTH][WINDOW_HEIGHT];

class SolidImmobile : public Particle
{
public:
	SolidImmobile(ParticleType newType, int newX, int newY, float newTemperature);
	virtual bool HandleEvents();
};

class SolidMobile : public Particle
{
public:
	SolidMobile(ParticleType newType, int newX, int newY, float newTemperature);
	virtual bool HandleEvents();
	void HandlePhysics();
};

class Liquid : public Particle
{
public:
	Liquid(ParticleType newType, int newX, int newY, float newTemperature);
	virtual bool HandleEvents();
	void HandlePhysics();
};

class Airborn : public Particle
{
public:
	Airborn(ParticleType newType, int ascendRate, int descendRate, int sidewardsRate, int noMovementRate, int newX, int newY, float newTemperature);
	virtual bool HandleEvents();
	void HandlePhysics();

	int ascendRate;
	int descendRate;
	int sidewardsRate;
	int noMovementRate;
};

void CreateParticle(ParticleType type, int x, int y, float temp, bool asSource);
void CreateParticle(ParticleType type, int x, int y, float temp);

void DestroyParticle(int x, int y);
void MoveParticles(int x1, int y1, int x2, int y2);
bool DropParticle(int x, int y, bool randomize);
bool DropParticle(int x, int y);
bool AscendParticle(int x, int y, bool randomize);
void EvenOutTemperatures(int x1, int y1, int x2, int y2);

// Wall particles.
// Does not move.
class Wall : public SolidImmobile
{
public:
	Wall(int newX, int newY, float newTemperature);
};

// Plant particles.
// Does not move but can convert neighbouring water particles into plant particles.
class Plant : public SolidImmobile
{
public:
	Plant(int newX, int newY, float newTemperature);
	bool HandleEvents();
};

// Glitch particle.
// Slowly consumes all space, will delete and remove other blocks in its way.
class Glitch : public SolidImmobile
{
public:
	Glitch(int newX, int newY, float newTemperature);
	void Draw();
	bool HandleEvents();
};

// Sand particles.
class Sand : public SolidMobile
{
public:
	Sand(int newX, int newY, float newTemperature);
};

// Water particles.
// Always attempts to move downwards if nothing is in way, will always try to move either left or right if cant move down.
// Can freeze into water.
class Water : public Liquid
{
public:
	Water(int newX, int newY, float newTemperature);
	bool HandleEvents();
};

// Basically just water but has a very high thermal conductivity.
class ThermalFluid : public Liquid
{
public:
	ThermalFluid(int newX, int newY, float newTemperature);
};

// Basically jsut water but it damaged blocks around it.
class Acid : public Liquid
{
public:
	Acid(int newX, int newY, float newTemperature);
	bool HandleEvents();
};

// Salt water particles.
// Basically just standard water but a bit more heavier and will damage any plants it touches.
class SaltWater : public Liquid
{
public:
	SaltWater(int newX, int newY, float newTemperature);
	bool HandleEvents();
};

// Salt particle.
// Basically sand but can combine with water to become salt water.
class Salt : public SolidMobile
{
public:
	Salt(int newX, int newY, float newTemperature);
	bool HandleEvents();
};

// Ice particles.
// Does not move.
// Can melt into water.
class Ice : public SolidImmobile
{
public:
	Ice(int newX, int newY, float newTemperature);
	bool HandleEvents();
};

// Salt ice particles.
// Does not move.
// Can melt into salt water.
class SaltIce : public SolidImmobile
{
public:
	SaltIce(int newX, int newY, float newTemperature);
	bool HandleEvents();
};

// Steam particles.
// Move upwards in a randomised direction, somewhat like a reverese water particle.
// Turns back into water when cools down.
class Steam : public Airborn
{
public:
	Steam(int newX, int newY, float newTemperature);
	bool HandleEvents();
};

// Source particle.
// Creates other particles of the given source type around it.
// Can be a source for any other type of block, useful for self repairing walls or for self replenishing water for example.
class Source : public SolidImmobile
{
public:
	Source(ParticleType type, int newX, int newY, float newTemperature);
	void Draw();
	bool HandleEvents();

	ParticleType sourceType;
};

// Stone particles.
// Can melt into lava at high heat.
class Stone : public SolidMobile
{
public:
	Stone(int newX, int newY, float newTemperature);
	bool HandleEvents();
};

// Lava particles.
// Can turn into stone at low temperatures.
class Lava : public Liquid
{
public:
	Lava(int newX, int newY, float newTemperature);
	bool HandleEvents();
};

// Fire particles.
// Airborn type that will lose health quickly.
// Sets flammable particles on fire.
class Fire : public Airborn
{
public:
	Fire(int newX, int newY, float newTemperature);
	bool HandleEvents();
};

// Gas particles.
// Airborn type.
// Standard airborn that will spread fire fast.
class Gas : public Airborn
{
public:
	Gas(int newX, int newY, float newTemperature);
};

class LightGas : public Airborn
{
public:
	LightGas(int newX, int newY, float newTemperature);
};

class HeavyGas : public Airborn
{
public:
	HeavyGas(int newX, int newY, float newTemperature);
};

class HeatPad : public SolidImmobile
{
public:
	HeatPad(int newX, int newY, float newTemperature);
	void Draw();
};

class Seed : public Airborn
{
public:
	Seed(int newX, int newY, float newTemperature);
	bool HandleEvents();
};