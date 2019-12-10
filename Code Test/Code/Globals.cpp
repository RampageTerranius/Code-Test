#include "globals.h"

//global variables
const std::string PROGRAM_NAME = "Falling Sand";

int frameRateLimit = 60;

//used to determine default screen size

const int MAX_BRUSH_SIZE = 32;//the total largest size the brush may be

//sdl window and renderer
SDL_Window* mainWindow = nullptr;
SDL_Renderer* mainRenderer = nullptr;
SDL_Surface* mainSurface = nullptr;

//main texture used for drawing pixels to screen
SDL_Texture* pixelTexture = SDL_CreateTexture(mainRenderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, WINDOW_WIDTH, WINDOW_HEIGHT);

float avgFPS = 0;

int currentBrushTemperature = 20;//changing this will change the default brush temperature
ParticleType currentBrushType = TYPE_SAND;
int currentBrushSize = 4;

Randomize random;

int countedFrames = 0;
bool running = false;

//ingame settings, these are the default settings for the given options

bool drawHeat = false;//when true will render particles color via heat instead of handing off draw fucntions to the particle its self
bool loopScreen = true;//when true particles that attempt to drop downwards while at the bottom of the screen will instead loop to the top of the screen, does the same for particles attempting to move upwards as well
bool pauseParticles = false;//when true particles physics and events will be paused, does not pause the entire program
bool renderBrush = true;//when true will show an outline of where particles will be brushed to
bool createAsSource = false;//if the blocks created should be source particles or standard particles


//weight (-1 donates it does not calculate weight)
int settingWeight[TYPE_TOTALTYPES]
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
	4,
	//Acid
	1,
	//Steam
	2,
	//plant
	-1,
	//salt
	6,
	//salt water
	5,
	//salt ice,
	-1,
	//glitch
	-1,
	//stone
	20,
	//lava
	7,
	//fire
	-1,
	//gas
	5,
	//light gas
	3,
	//heavy gas
	7,
	// Heat Pad.
	-1,

	//the following must ALWAYS be at the end
	//source
	-1
};

//health (-1 donates can not be broken)
int settingHealth[TYPE_TOTALTYPES]
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
	20,
	//Plant
	120,
	//salt
	140,
	//salt water
	100,
	//salt ice
	120,
	//glitch
	50,
	//stone
	260,
	//lava
	100,
	//fire(loses health per tick)
	300,
	//gas
	50,
	//light gas
	50,
	//heavy gas
	50,
	// Heat Pad.
	60,

	//the following must ALWAYS be at the end
	//source
	300
};

float settingThermalConductivity[TYPE_TOTALTYPES]
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
	0.03f,
	//plant
	0.026f,
	//salt
	0.0185f,
	//salt water
	0.015f,
	//salt ice
	0.0175f,
	//glitch
	0,
	//stone
	0.0018f,
	//lava
	0.005f,
	//fire
	0.1f,
	//gas
	0.01f,
	//light gas
	0.01f,
	//heavy gas
	0.01f,
	// Heat Pad.
	0.005f,

	//the following must ALWAYS be at the end
	//source
	0
};

//color settings
//format is R, G, B, A
int settingColor[TYPE_TOTALTYPES][4]
{
	//Wall
	{87, 87, 87, 0},
	//Sand
	{207, 226, 34, 0},
	//Water
	{0, 0, 255, 0},
	//Ice
	{100, 255, 255, 0},
	//Thermal
	{150, 50, 255, 0},
	//Acid
	{102, 0, 102, 0},
	//Steam
	{200, 255, 255, 0},
	//plant
	{20, 150, 20, 0},
	//salt
	{230, 230, 230, 0},
	//salt water
	{0, 0, 150, 0},
	//salt ice
	{100, 255, 150, 0},
	//glitch(this color is not used here, glitch particles color is randomized)
	{0, 0, 0, 0},
	//stone
	{127, 127, 90, 0},
	//lava
	{255, 153, 51, 0},
	//fire
	{255, 20, 20, 0},
	//gas
	{150, 20, 150, 0},
	//light gas
	{160, 40, 160, 0},
	//heavy gas
	{170, 60, 170, 0},
	// Heat pad (this particle uses heat color to self print).
	{0, 0, 0, 0}

};

// Flammability is in a percentage basis (0 to 100)
int settingFlammability[TYPE_TOTALTYPES]
{
	//Wall
	0,
	//Sand
	0,
	//Water
	0,
	//Ice
	0,
	//Thermal
	2,
	//Acid
	0,
	//Steam
	0,
	//plant
	25,
	//salt
	0,
	//salt water
	0,
	//salt ice,
	0,
	//glitch
	0,
	//stone
	0,
	//lava
	0,
	//fire
	0,
	//gas
	80,
	//light gas
	80,
	//heavy gas
	80,
	// Heat pad.
	0,

	//the following must ALWAYS be at the end
	//source
	-1
};

//other settings
//freeze/melt/boil points
float waterFreezePoint = 0;
float iceMeltPoint = 2;
float steamCondensationPoint = 98;
float waterBoilIntoSteamPoint = 100;
float saltWaterEventTempMultiplier = 1.2f;//a value of 1.1 would give a 10% increase over water boil/freeze points
float lavaSolidifyTemp = 200;

//randomised event chances
int acidDamageChance = 30;// Percentage chance.
int plantSpreadChance = 20;// Percentage chance.
int glitchSpreadChance = 5;// Percentage chance.

//airborn particle movement rates (movement in percentage chance, MUST equal up to 100 in total)
//steam
int steamAscendRate = 65;
int steamDescendRate = 0;
int steamSidewardsRate = 25;
int steamNoMovementRate = 10;
//fire
int fireAscendRate = 85;
int fireDescendRate = 0;
int fireSidewardsRate = 15;
int fireNoMovementRate = 0;
//gas
int gasAscendRate = 50;
int gasDescendRate = 10;
int gasSidewardsRate = 30;
int gasNoMovementRate = 10;
//heavy gas
int heavyGasAscendRate = 10;
int heavyGasDescendRate = 60;
int heavyGasSidewardsRate = 20;
int heavyGasNoMovementRate = 10;
//light gas
int lightGasAscendRate = 75;
int lightGasDescendRate = 6;
int lightGasSidewardsRate = 10;
int lightGasNoMovementRate = 9;


//affects how big of a multiplier is used depending on the percentage difference in temperature between two different particles
// is calculated like so
// abs( (allParticles[x1][y1]->temperature - allParticles[x2][y2]->temperature) / allParticles[x1][y1]->temperature) * 100 ) / temperatureDifferenceDivisor
//lower number = faster spread
int temperatureDifferenceDivisor = 15;

TTF brushName = TTF();
TTF brushSize = TTF();
TTF brushTemperature = TTF();
TTF selectedParticleTemperature = TTF();
TTF selectedParticleName = TTF();
TTF currentFrameRate = TTF();