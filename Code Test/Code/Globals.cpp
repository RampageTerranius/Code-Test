#include "globals.h"

// Global variables.
const std::string PROGRAM_NAME = "Falling Sand";

int frameRateLimit = 60;

// Used to determine default screen size.

const int MAX_BRUSH_SIZE = 128;// The total largest size the brush may be.

// Sdl window and renderer.
SDL_Window* mainWindow = nullptr;
SDL_Renderer* mainRenderer = nullptr;
SDL_Surface* mainSurface = nullptr;

// Main texture used for drawing pixels to screen.
SDL_Texture* pixelTexture = SDL_CreateTexture(mainRenderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, WINDOW_WIDTH, WINDOW_HEIGHT);

float avgFPS = 0;

int currentBrushTemperature = 20;// Changing this will change the default brush temperature.
int currentBrushType = 0;
int currentBrushSize = 4;

int countedFrames = 0;
bool running = false;
bool tempUnpause = false;
bool currentEventGate = false;
bool currentPhysicsGate = false;

SDL_Color backgroundColor = {255, 255, 255, 0};

// Ingame settings, these are the default settings for the given options.

ViewType viewMode = VIEW_DEFAULT;// Determines what view mode we are currently in.
bool loopScreen = false;// When true particles that attempt to drop downwards while at the bottom of the screen will instead loop to the top of the screen, does the same for particles attempting to move upwards as well.
bool pauseParticles = false;// When true particles physics and events will be paused, does not pause the entire program.
bool renderBrush = true;// When true will show an outline of where particles will be brushed to.
bool createAsSource = false;// If the blocks created should be source particles or standard particles.

// Affects how big of a multiplier is used depending on the percentage difference in temperature between two different particles.
// Is calculated like so.
// abs( (allParticles[x1][y1]->temperature - allParticles[x2][y2]->temperature) / allParticles[x1][y1]->temperature) * 100 ) / temperatureDifferenceDivisor
// Lower number = faster spread.
int temperatureDifferenceDivisor = 15;

// How far out liquid particles will check to attempt to flow out to in the situation they can only go oen direction.
int flowRate = 4;

TTF brushName = TTF();
TTF brushSize = TTF();
TTF brushTemperature = TTF();
TTF selectedParticleTemperature = TTF();
TTF selectedParticleName = TTF();
TTF selectedParticleHealth = TTF();
TTF currentFrameRate = TTF();
TTF currentViewType = TTF();