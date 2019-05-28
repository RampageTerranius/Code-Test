#pragma once

//sdl
#include <SDL.h>

//other
#include <string>
#include <vector>
#include <iostream>

//global variables
const std::string PROGRAM_NAME = "Testola";

//used to determine default screen size and frame rate
const int WINDOW_WIDTH = 1024;
const int WINDOW_HEIGHT = 768;

const int MAX_BRUSH_SIZE = 6;

//sdl window and renderer
SDL_Window* mainWindow = nullptr;
SDL_Renderer* mainRenderer = nullptr;

int countedFrames = 0;

bool running = false;

//project
#include "TimerHandle.h"
#include "Particles.h"
#include "EntityHandle.h"
#include "Setup.h"
#include "Render.h"
#include "EventHandle.h"
#include "Shutdown.h"