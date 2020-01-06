#pragma once

#include "ParticleTypes.h"
#include "ViewTypes.h"
#include "TTF.h"

#include <SDL.h>

#include <string>
#include <vector>

// Used to determine default screen size.
#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

// Global variables.
extern const std::string PROGRAM_NAME;

extern int frameRateLimit;

// The total largest size the brush may be.
extern const int MAX_BRUSH_SIZE;

// Sdl window, renderer and pixel surface.
extern SDL_Window* mainWindow;
extern SDL_Renderer* mainRenderer;
extern SDL_Surface* mainSurface;

extern float avgFPS;

extern int currentBrushTemperature;// Changing this will change the default brush temperature.
extern ParticleType currentBrushType;
extern int currentBrushSize;

extern int countedFrames;
extern bool running;

extern SDL_Color backgroundColor;

// Ingame settings, these are the default settings for the given options.

extern ViewType viewMode;// Determines what view mode we are currently in.
extern bool loopScreen;// When true particles that attempt to drop downwards while at the bottom of the screen will instead loop to the top of the screen, does the same for particles attempting to move upwards as well.
extern bool pauseParticles;// When true particles physics and events will be paused, does not pause the entire program.
extern bool renderBrush;// When true will show an outline of where particles will be brushed to.
extern bool createAsSource;// If the blocks created should be source particles or standard particles.

// Affects how big of a multiplier is used depending on the percentage difference in temperature between two different particles.
// Is calculated like so: abs( (allParticles[x1][y1]->temperature - allParticles[x2][y2]->temperature) / allParticles[x1][y1]->temperature) * 100 ) / temperatureDifferenceDivisor
// Lower number = faster spread.
extern int temperatureDifferenceDivisor;

extern TTF brushName;
extern TTF brushSize;
extern TTF brushTemperature;
extern TTF selectedParticleTemperature;
extern TTF selectedParticleName;
extern TTF selectedParticleHealth;
extern TTF currentFrameRate;
