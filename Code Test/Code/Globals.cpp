#include "globals.h"

// Global variables.
const std::string PROGRAM_NAME = "Falling Sand";

int frameRateLimit = 60;

// Used to determine default screen size.

const int MAX_BRUSH_SIZE = 32;// The total largest size the brush may be.

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

SDL_Color backgroundColor = {255, 255, 255, 0};

// Ingame settings, these are the default settings for the given options.

ViewType viewMode = VIEW_TYPE;// Determines what view mode we are currently in.
bool loopScreen = false;// When true particles that attempt to drop downwards while at the bottom of the screen will instead loop to the top of the screen, does the same for particles attempting to move upwards as well.
bool pauseParticles = false;// When true particles physics and events will be paused, does not pause the entire program.
bool renderBrush = true;// When true will show an outline of where particles will be brushed to.
bool createAsSource = false;// If the blocks created should be source particles or standard particles.


// Weight (-1 donates it does not calculate weight)
int settingWeight[TYPE_TOTALTYPES]
{
	// Sand.
	10,
	// Ice.
	-1,
	// Thermal.
	4,
	// Acid.
	1,
	// Steam.
	2,
	// Plant.
	-1,
	// Salt.
	6,
	// Salt water.
	5,
	// Salt ice.
	-1,
	// Glitch.
	-1,
	// Stone.
	20,
	// Lava.
	7,
	// Fire.
	-1,
	// Gas.
	5,
	// Light gas.
	3,
	// Heavy gas.
	7,
	// Heat Pad.
	-1,
	// Seed.
	-1,

	// The following must ALWAYS be at the end.
	// Source.
	-1
};

// Health (-1 donates can not be broken)
int settingHealth[TYPE_TOTALTYPES]
{
	// Sand.
	160,
	// Ice.
	100,
	// Thermal.
	20,
	// Acid.
	200,// Loses health each time it damages another block.
	// Steam.
	20,
	// Plant.
	120,
	// Salt.
	140,
	// Salt water.
	100,
	// Salt ice.
	120,
	// Glitch.
	50,
	// Stone.
	260,
	// Lava.
	100,
	// Fire(loses health per tick)
	300,
	// Gas.
	50,
	// Light gas.
	50,
	// Heavy gas.
	50,
	// Heat Pad.
	60,
	// Seed.
	20,

	// The following must ALWAYS be at the end.
	// Source.
	300
};

float settingThermalConductivity[TYPE_TOTALTYPES]
{
	// Sand.
	0.0025f,
	// Ice.
	0.015f,
	// Thermal.
	2.5f,
	// Acid.
	0.2f,
	// Steam.
	0.03f,
	// Plant.
	0.026f,
	// Salt.
	0.0185f,
	// Salt water.
	0.015f,
	// Salt ice.
	0.0175f,
	// Glitch.
	0,
	// Stone.
	0.0018f,
	// Lava.
	0.005f,
	// Fire.
	0.1f,
	// Gas.
	0.01f,
	// Light gas.
	0.01f,
	// Heavy gas.
	0.01f,
	// Heat Pad.
	0.05f,
	// Seed.
	0.02f,

	// The following must ALWAYS be at the end.
	// Source.
	0
};

// Color settings.
// Format is R, G, B, A.
int settingColor[TYPE_TOTALTYPES][4]
{
	// Sand.
	{207, 226, 34, 0},
	// Ice.
	{100, 255, 255, 0},
	// Thermal.
	{150, 50, 255, 0},
	// Acid.
	{102, 0, 102, 0},
	// Steam.
	{200, 255, 255, 0},
	// Plant.
	{20, 150, 20, 0},
	// Salt.
	{230, 230, 230, 0},
	// Salt water.
	{0, 0, 150, 0},
	// Salt ice.
	{100, 255, 150, 0},
	// Glitch (this color is not used here, glitch particles color is randomized)
	{0, 0, 0, 0},
	// Stone.
	{127, 127, 90, 0},
	// Lava.
	{255, 153, 51, 0},
	// Fire.
	{255, 20, 20, 0},
	// Gas.
	{150, 20, 150, 0},
	// Light gas.
	{160, 40, 160, 0},
	// Heavy gas.
	{170, 60, 170, 0},
	// Heat pad (this particle uses heat color to self print).
	{0, 0, 0, 0},
	// Seed.
	{0, 100, 0, 0}

};

// Flammability is in a percentage basis (0 to 100)
int settingFlammability[TYPE_TOTALTYPES]
{
	// Sand.
	0,
	// Thermal.
	2,
	// Acid.
	0,
	// Plant.
	25,
	// Salt.
	0,
	// Salt water.
	0,
	// Salt ice.
	0,
	// Glitch.
	0,
	// Stone.
	0,
	// Lava.
	0,
	// Fire.
	0,
	// Gas.
	80,
	// light gas.
	80,
	// Heavy gas.
	80,
	// Heat pad.
	0,
	// Seed.
	40,

	// The following must ALWAYS be at the end.
	// Source.
	-1
};

// Other settings.
// Freeze/melt/boil points.
float steamCondensationPoint = 98;
float saltWaterEventTempMultiplier = 1.2f;// A value of 1.1 would give a 10% increase over water boil/freeze points.
float lavaSolidifyTemp = 200;

// Randomised event chances.
int acidDamageChance = 30;// Percentage chance.
int plantSpreadChance = 20;// Percentage chance.
int seedSpreadChance = 2;// Percentage chance.
int glitchSpreadChance = 5;// Percentage chance.

// Airborn particle movement rates (movement in percentage chance, MUST equal up to 100 in total)
// Fire.
int fireAscendRate = 85;
int fireDescendRate = 0;
int fireSidewardsRate = 15;
int fireNoMovementRate = 0;
// Gas.
int gasAscendRate = 50;
int gasDescendRate = 10;
int gasSidewardsRate = 30;
int gasNoMovementRate = 10;
// Heavy gas.
int heavyGasAscendRate = 10;
int heavyGasDescendRate = 60;
int heavyGasSidewardsRate = 20;
int heavyGasNoMovementRate = 10;
// Light gas.
int lightGasAscendRate = 75;
int lightGasDescendRate = 6;
int lightGasSidewardsRate = 10;
int lightGasNoMovementRate = 9;
// Seed.
int seedAscendRate = 15;
int seedDescendRate = 25;
int seedSidewardsRate = 20;
int seedNoMovementRate = 40;

// Affects how big of a multiplier is used depending on the percentage difference in temperature between two different particles.
// Is calculated like so.
// abs( (allParticles[x1][y1]->temperature - allParticles[x2][y2]->temperature) / allParticles[x1][y1]->temperature) * 100 ) / temperatureDifferenceDivisor
// Lower number = faster spread.
int temperatureDifferenceDivisor = 15;

TTF brushName = TTF();
TTF brushSize = TTF();
TTF brushTemperature = TTF();
TTF selectedParticleTemperature = TTF();
TTF selectedParticleName = TTF();
TTF selectedParticleHealth = TTF();
TTF currentFrameRate = TTF();