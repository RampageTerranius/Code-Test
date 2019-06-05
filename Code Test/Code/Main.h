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

const int MAX_BRUSH_SIZE = 16;

bool loopScreen = true;

float avgFPS = 0;

int currentBrushTemperature = 20;

//sdl window and renderer
SDL_Window* mainWindow = nullptr;
SDL_Renderer* mainRenderer = nullptr;

int countedFrames = 0;
bool running = false;

//extra settings

//weight (-1 donates it does not calculate weight)
int wallWeight = -1;
int sandWeight = 10;
int waterWeight = 3;
int iceWeight = -1;
int thermalFluidWeight = 10;

//thermal conductivity
float wallThermalConductivity = 0.001;
float sandThermalConductivity = 0.005;
float waterThermalConductivity = 0.025;
float iceThermalConductivity = 0.03;
float thermalFluidThermalConductivity = 2.5;

//other settings
//freeze and melt points
float waterFreezePoint = 0;
float iceMeltPoint = 0;

//project
#include "TimerHandle.h"
#include "Particles.h"
#include "EntityHandle.h"
#include "TTF.h"
#include "EventHandle.h"
#include "Render.h"
#include "Setup.h"
#include "Shutdown.h"