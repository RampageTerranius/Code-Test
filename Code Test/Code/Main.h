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
int wallWeight = -1;
int sandWeight = 10;
int waterWeight = 3;
int iceWeight = -1;
int thermalFluidWeight = 8;
int acidWeight = 1;

//health (-1 donates can not be broken)
int wallHealth = 500;
int sandHealth = 160;
int waterHealth = 80;
int iceHealth = 100;
int thermalFluidHealth = 20;
int acidHealth = 200;//acid loses health each time it damages another block

//thermal conductivity
float wallThermalConductivity = 0.0006;
float sandThermalConductivity = 0.0025;
float waterThermalConductivity = 0.0125;
float iceThermalConductivity = 0.015;
float thermalFluidThermalConductivity = 2.5;
float acidThermalConductivity = 0.2;

//other settings
//freeze and melt points
float waterFreezePoint = 0;
float iceMeltPoint = 0;

int acidDamageChance = 10;//higher number means lower chance

//project
#include "TimerHandle.h"
#include "Particles.h"
#include "EntityHandle.h"
#include "TTF.h"
#include "EventHandle.h"
#include "Render.h"
#include "Setup.h"
#include "Shutdown.h"