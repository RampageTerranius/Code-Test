#pragma once

//sdl
#include <SDL.h>
#include <SDL_ttf.h>

//other
#include <string>
#include <vector>
#include <iostream>
#include <cmath>

//global variables
const std::string PROGRAM_NAME = "Testola";

//used to determine default screen size and frame rate
const int WINDOW_WIDTH = 1024;
const int WINDOW_HEIGHT = 768;

const int MAX_BRUSH_SIZE = 32;

float avgFPS = 0;

int currentBrushTemperature = 20;

//sdl window and renderer
SDL_Window* mainWindow = nullptr;
SDL_Renderer* mainRenderer = nullptr;

int countedFrames = 0;
bool running = false;

//extra settings

bool drawHeat = false;//when true will render particles color via heat isntead of handing off draw fucntiosn to the particle itsself
bool loopScreen = true;
bool pauseParticles = false;

//weight (-1 donates it does not calculate weight)
int settingWeight[]
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
	8,
	//Acid
	1,
	//Steam
	2
};

//health (-1 donates can not be broken)
int settingHealth[]
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
	20
};

float settingThermalConductivity[]
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
	0.03f
};

//other settings
//freeze and melt points
float waterFreezePoint = 0;
float iceMeltPoint = 2;
float steamCondensationPoint = 98;
float waterBoilIntoSteamPoint = 100;

int acidDamageChance = 10;//higher number means lower chance

//affects how bit of a multiplier is used depending on the percentage difference in temperature between two different particles
// is calculated like so
// abs( (allParticles[x1][y1]->temperature - allParticles[x2][y2]->temperature) / allParticles[x1][y1]->temperature) * 100 ) / temperatureDifferenceDivisor
//lower number = faster spread
int temperatureDifferenceDivisor = 15;

//project
#include "TimerHandle.h"
#include "Particles.h"
#include "EntityHandle.h"
#include "TTF.h"
#include "EventHandle.h"
#include "Render.h"
#include "Setup.h"
#include "Shutdown.h"