#pragma once

//sdl
#include <SDL.h>

//other
#include <string>
#include <vector>

//global variables
const std::string PROGRAM_NAME = "Testola";

const int WINDOW_WIDTH = 1024;
const int WINDOW_HEIGHT = 768;
const int FPS = 60;

SDL_Window* mainWindow = NULL;
SDL_Renderer* mainRenderer = NULL;

int playerEntity;



bool running = false;

//project
#include "TimerHandle.h"
#include "Player.h"
#include "EntityHandle.h"
#include "Setup.h"
#include "Render.h"
#include "EventHandle.h"
#include "Shutdown.h"