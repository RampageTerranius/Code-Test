#include "globals.h"

//global variables
const std::string PROGRAM_NAME = "Falling Sand";

int frameRateLimit = 60;
bool speedTest = true;// if true will spawn half the screen full of sand to test frame rate.

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

int countedFrames = 0;
bool running = false;

//ingame settings, these are the default settings for the given options

bool drawHeat = false;//when true will render particles color via heat instead of handing off draw fucntions to the particle its self
bool loopScreen = false;//when true particles that attempt to drop downwards while at the bottom of the screen will instead loop to the top of the screen, does the same for particles attempting to move upwards as well
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

	//the following must ALWAYS be at the end
	//source
	300
};

float settingThermalConductivity[TYPE_TOTALTYPES]
{
	//Wall
	0.00006f,
	//Sand
	0.00025f,
	//Water
	0.00125f,
	//Ice
	0.0015f,
	//Thermal
	0.25f,
	//Acid
	0.02f,
	//Steam
	0.003f,
	//plant
	0.0026f,
	//salt
	0.00185f,
	//salt water
	0.0015f,
	//salt ice
	0.00175f,
	//glitch
	0,
	//stone
	0.00018f,
	//lava
	0.0005f,
	//fire
	0.01f,
	//gas
	0.001f,
	//light gas
	0.001f,
	//heavy gas
	0.001f,

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
	{170, 60, 170, 0}
};

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
	50,
	//Acid
	0,
	//Steam
	0,
	//plant
	8,
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
	2,
	//light gas
	2,
	//heavy gas
	2,

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
int acidDamageChance = 10;//higher number means lower chance (calculated as (1 / acidDamageChance) meaning if chance was 10 there is a 1 in 10 chance per tick to damage a block)
int plantSpreadChance = 30;//higher number means lower chance (calculated as (1 / plantSpreadChance) meaning if chance was 10 there is a 1 in 10 chance per tick to spread plant through a water block)
int glitchSpreadChance = 120;

//airborn particle movement rates
//steam
int steamAscendRate = 6;
int steamDescendRate = 0;
int steamSidewardsRate = 2;
int steamNoMovementRate = 1;
//fire
int fireAscendRate = 8;
int fireDescendRate = 0;
int fireSidewardsRate = 1;
int fireNoMovementRate = 0;
//gas
int gasAscendRate = 4;
int gasDescendRate = 1;
int gasSidewardsRate = 2;
int gasNoMovementRate = 1;
//heavy gas
int heavyGasAscendRate = 1;
int heavyGasDescendRate = 6;
int heavyGasSidewardsRate = 2;
int heavyGasNoMovementRate = 1;
//light gas
int lightGasAscendRate = 10;
int lightGasDescendRate = 1;
int lightGasSidewardsRate = 4;
int lightGasNoMovementRate = 1;


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