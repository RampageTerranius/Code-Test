#pragma once

#include <string>
#include <vector>
#include <iostream>

//sdl
#include <SDL.h>

#include "Globals.h"
#include "ParticleTypes.h"

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

//wall particles
//does not move
class Wall : public SolidImmobile
{
public:
	Wall(int newX, int newY, float newTemperature);
};

//plant particles
//does not move but can convert neighbouring water particles into plant particles
class Plant : public SolidImmobile
{
public:
	Plant(int newX, int newY, float newTemperature);
	bool HandleEvents();
};

//glitch particle
//slowly consumes all space, will delete and remove other blocks in its way
class Glitch : public SolidImmobile
{
public:
	Glitch(int newX, int newY, float newTemperature);
	void Draw();
	bool HandleEvents();
};

//sand particles
class Sand : public SolidMobile
{
public:
	Sand(int newX, int newY, float newTemperature);
};

//water particles
//always attempts to move downwards if nothing is in way, will always try to move either left or right if cant move down
//can freeze into water
class Water : public Liquid
{
public:
	Water(int newX, int newY, float newTemperature);
	bool HandleEvents();
};

//basically just water but has a very high thermal conductivity
class ThermalFluid : public Liquid
{
public:
	ThermalFluid(int newX, int newY, float newTemperature);
};

//basicalyl jsut water but it damaged blocks around it
class Acid : public Liquid
{
public:
	Acid(int newX, int newY, float newTemperature);
	bool HandleEvents();
};

//salt water particles
//basically just standard water but a bit more heavier and will damage any plants it touches
class SaltWater : public Liquid
{
public:
	SaltWater(int newX, int newY, float newTemperature);
	bool HandleEvents();
};

//salt particle
//basically sand but can combine with water to become salt water
class Salt : public SolidMobile
{
public:
	Salt(int newX, int newY, float newTemperature);
	bool HandleEvents();
};

//ice particles
//does not move
//can melt into water
class Ice : public SolidImmobile
{
public:
	Ice(int newX, int newY, float newTemperature);
	bool HandleEvents();
};

//salt ice particles
//does not move
//can melt into salt water
class SaltIce : public SolidImmobile
{
public:
	SaltIce(int newX, int newY, float newTemperature);
	bool HandleEvents();
};

//steam particles
//move upwards in a randomised direction, somewhat like a reverese water particle
//turns back into water when cools down
class Steam : public Airborn
{
public:
	Steam(int newX, int newY, float newTemperature);
	bool HandleEvents();
};

//source particle
//creates other particles of the given source type around it
//can be a source for any other type of block, useful for self repairing walls or for self replenishing water for example
class Source : public SolidImmobile
{
public:
	Source(ParticleType type, int newX, int newY, float newTemperature);
	void Draw();
	bool HandleEvents();

	ParticleType sourceType;
};

//stone particles
//can melt into lava at high heat
class Stone : public SolidMobile
{
public:
	Stone(int newX, int newY, float newTemperature);
	bool HandleEvents();
};

//lava particles
//can turn into stone at low temperatures
class Lava : public Liquid
{
public:
	Lava(int newX, int newY, float newTemperature);
	bool HandleEvents();
};

//fire particles
//airborn type that will lose health quickly
//sets flammable particles on fire
class Fire : public Airborn
{
public:
	Fire(int newX, int newY, float newTemperature);
	bool HandleEvents();
};

//gas particles
//airborn type
//standard airborn that will spread fire fast
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