#pragma once

#include <string>
#include <vector>

//sdl
#include <SDL.h>

#include "ParticleTypes.h"
#include "TTF.h"
#include "mouse.h"
#include "keyboard.h"

//used to determine default screen size
#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

class Particle;

static class Globals
{
public:
	//global variables
	const std::string PROGRAM_NAME = "Falling Sand";

	//used to determine default screen size

	const int MAX_BRUSH_SIZE = 32;//the total largest size the brush may be

	//sdl window and renderer
	SDL_Window* mainWindow = nullptr;
	SDL_Renderer* mainRenderer = nullptr;

	float avgFPS = 0;

	int currentBrushTemperature = 20;//changing this will change the default brush temperature
	ParticleType currentBrushType = TYPE_SAND;
	int currentBrushSize = 4;

	//array and vector list handling all data to do with our entities
	Particle* allParticles[WINDOW_WIDTH][WINDOW_HEIGHT];
	std::vector<Particle*> particleList;

	int countedFrames = 0;
	bool running = false;

	//ingame settings, these are the default settings for the given options

	bool drawHeat = false;//when true will render particles color via heat instead of handing off draw fucntions to the particle its self
	bool loopScreen = true;//when true particles that attempt to drop downwards while at the bottom of the screen will instead loop to the top of the screen, does the same for particles attempting to move upwards as well
	bool pauseParticles = false;//when true particles physics and events will be paused, does nto pause the entire program
	bool renderBrush = true;//when true will show an outline of where particles will be brushed to
	bool createAsSource = false;//if the blocks created should be source particles or standard particles


	//weight (-1 donates it does not calculate weight)
	int settingWeight[TYPE_TOTALTYPES]
	{
		//Wall
		-1,
		//Sand
		10,
		//Water
		3,
		//Ice
		-1,
		//Thermal
		4,
		//Acid
		1,
		//Steam
		2,
		//plant
		-1,
		//salt
		6,
		//salt water
		5,
		//salt ice,
		-1,
		//glitch
		-1,

		//the following must ALWAYS be at the end
		//source
		-1
	};

	//health (-1 donates can not be broken)
	int settingHealth[TYPE_TOTALTYPES]
	{
		//Wall
		500,
		//Sand
		160,
		//Water
		80,
		//Ice
		100,
		//Thermal
		20,
		//Acid
		200,//loses health each time it damages another block
		//Steam
		20,
		//Plant
		120,
		//salt
		140,
		//salt water
		100,
		//salt ice
		120,
		//glitch
		50,

		//the following must ALWAYS be at the end
		//source
		300
	};

	float settingThermalConductivity[TYPE_TOTALTYPES]
	{
		//Wall
		0.0006f,
		//Sand
		0.0025f,
		//Water
		0.0125f,
		//Ice
		0.015f,
		//Thermal
		2.5f,
		//Acid
		0.2f,
		//Steam
		0.03f,
		//plant
		0.026f,
		//salt
		0.0185f,
		//salt water
		0.015f,
		//salt ice
		0.0175f,
		//glitch
		0,

		//the following must ALWAYS be at the end
		//source
		0
	};

	//color settings
	//format is R, G, B, A
	int settingColor[TYPE_TOTALTYPES][4]
	{
		//Wall
		{87, 87, 87, 0},
		//Sand
		{207, 226, 34, 0},
		//Water
		{0, 0, 255, 0},
		//Ice
		{100, 255, 255, 0},
		//Thermal
		{150, 50, 255, 0},
		//Acid
		{102, 0, 102, 0},
		//Steam
		{200, 255, 255, 0},
		//plant
		{20, 150, 20, 0},
		//salt
		{230, 230, 230, 0},
		//salt water
		{0, 0, 150, 0},
		//salt ice
		{100, 255, 150, 0},
		//gltich(this color is not used here, glitch particles color is randomized)
		{0, 0, 0, 0}
	};

//other settings
//freeze/melt/boil points
float waterFreezePoint = 0;
float iceMeltPoint = 2;
float steamCondensationPoint = 98;
float waterBoilIntoSteamPoint = 100;
float saltWaterEventTempMultiplier = 1.2f;//a value of 1.1 would give a 10% increase over water boil/freeze points

//randomised event chances
int acidDamageChance = 10;//higher number means lower chance (calculated as (1 / acidDamageChance) meaning if chance was 10 there is a 1 in 10 chance per tick to damage a block)
int plantSpreadChance = 30;//higher number means lower chance (calculated as (1 / plantSpreadChance) meaning if chance was 10 there is a 1 in 10 chance per tick to spread plant through a water block)
int glitchSpreadChance = 120;


//affects how big of a multiplier is used depending on the percentage difference in temperature between two different particles
// is calculated like so
// abs( (allParticles[x1][y1]->temperature - allParticles[x2][y2]->temperature) / allParticles[x1][y1]->temperature) * 100 ) / temperatureDifferenceDivisor
//lower number = faster spread
int temperatureDifferenceDivisor = 15;

TTF brushName;
TTF brushSize;
TTF brushTemperature;
TTF selectedParticleTemperature;
TTF selectedParticleName;
TTF currentFrameRate;

Mouse mouse;
Keyboard keyboard;
}globals;