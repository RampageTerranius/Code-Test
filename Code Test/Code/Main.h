#pragma once

//sdl
#include <SDL.h>

//other
#include <string>
#include <vector>

//global variables
const std::string PROGRAM_NAME = "Testola";

//used to determine default screen size and frame rate
const int WINDOW_WIDTH = 1024;
const int WINDOW_HEIGHT = 768;
const int FPS = 60;

//sdl window and renderer
SDL_Window* mainWindow = NULL;
SDL_Renderer* mainRenderer = NULL;

bool EntityExists[WINDOW_WIDTH][WINDOW_HEIGHT];



bool running = false;

//project
#include "TimerHandle.h"
#include "Player.h"
#include "EntityHandle.h"
#include "Setup.h"
#include "Render.h"
#include "EventHandle.h"
#include "Shutdown.h"