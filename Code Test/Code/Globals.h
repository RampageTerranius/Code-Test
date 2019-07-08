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


	//global variables
	extern const std::string PROGRAM_NAME;

	//used to determine default screen size

	extern const int MAX_BRUSH_SIZE;//the total largest size the brush may be

	//sdl window and renderer
	extern SDL_Window* mainWindow;
	extern SDL_Renderer* mainRenderer;

	extern float avgFPS;

	extern int currentBrushTemperature;//changing this will change the default brush temperature
	extern ParticleType currentBrushType;
	extern int currentBrushSize;

	//array and vector list handling all data to do with our entities
	extern Particle* allParticles[WINDOW_WIDTH][WINDOW_HEIGHT];
	extern std::vector<Particle*> particleList;

	extern int countedFrames;
	extern bool running;

	//ingame settings, these are the default settings for the given options

	extern bool drawHeat;//when true will render particles color via heat instead of handing off draw fucntions to the particle its self
	extern bool loopScreen;//when true particles that attempt to drop downwards while at the bottom of the screen will instead loop to the top of the screen, does the same for particles attempting to move upwards as well
	extern bool pauseParticles;//when true particles physics and events will be paused, does nto pause the entire program
	extern bool renderBrush;//when true will show an outline of where particles will be brushed to
	extern bool createAsSource;//if the blocks created should be source particles or standard particles


	//weight (-1 donates it does not calculate weight)
	extern int settingWeight[TYPE_TOTALTYPES];

	//health (-1 donates can not be broken)
	extern int settingHealth[TYPE_TOTALTYPES];
	extern float settingThermalConductivity[TYPE_TOTALTYPES];

	//color settings
	//format is R, G, B, A
	extern int settingColor[TYPE_TOTALTYPES][4];

//other settings
//freeze/melt/boil points
extern float waterFreezePoint;
extern float iceMeltPoint;
extern float steamCondensationPoint;
extern float waterBoilIntoSteamPoint;
extern float saltWaterEventTempMultiplier;//a value of 1.1 would give a 10% increase over water boil/freeze points

//randomised event chances
extern int acidDamageChance;;//higher number means lower chance (calculated as (1 / acidDamageChance) meaning if chance was 10 there is a 1 in 10 chance per tick to damage a block)
extern int plantSpreadChance;//higher number means lower chance (calculated as (1 / plantSpreadChance) meaning if chance was 10 there is a 1 in 10 chance per tick to spread plant through a water block)
extern int glitchSpreadChance;


//affects how big of a multiplier is used depending on the percentage difference in temperature between two different particles
// is calculated like so
// abs( (allParticles[x1][y1]->temperature - allParticles[x2][y2]->temperature) / allParticles[x1][y1]->temperature) * 100 ) / temperatureDifferenceDivisor
//lower number = faster spread
extern int temperatureDifferenceDivisor;

extern TTF brushName;
extern TTF brushSize;
extern TTF brushTemperature;
extern TTF selectedParticleTemperature;
extern TTF selectedParticleName;
extern TTF currentFrameRate;

extern Mouse mouse;
extern Keyboard keyboard;