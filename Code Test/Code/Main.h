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
//water
float waterFreezePoint = 0;

//ice
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