#include "Shutdown + Startup.h"

#include "Particles.h"

#include <SDL.h>
#include <SDL_ttf.h>

#include <windows.h>
#include <string>
#include <iostream>
#include <filesystem>
#include <exception>
#include "SimpleINI/SimpleIni.h"

bool LoadParticle(CSimpleIni ini)
{
	try
	{
		ParticleType newType = ParticleType();
		
		// Load all basic particle data.
		newType.name = ini.GetValue("Particle", "Name");

		std::string str = ini.GetValue("Particle", "Type");
		if (str == "immobile")
			newType.movementType = MOVEMENTTYPE_IMMOBILE;
		else if (str == "pile")
			newType.movementType = MOVEMENTTYPE_PILE;
		else if (str == "liquid")
			newType.movementType = MOVEMENTTYPE_LIQUID;
		else if (str == "airborn")
			newType.movementType = MOVEMENTTYPE_AIRBORN;
		else
			newType.movementType = MOVEMENTTYPE_IMMOBILE;

		newType.weight = std::stof(ini.GetValue("Particle", "Weight"));
		newType.startingHealth = std::stoi(ini.GetValue("Particle", "Health"));
		newType.thermalConductivity = std::stof(ini.GetValue("Particle", "Thermal_Conductivity"));
		newType.Flammability = std::stoi(ini.GetValue("Particle", "Flammability"));

		// Load particle colors.
		newType.R = std::stoi(ini.GetValue("Color", "R"));
		newType.G = std::stoi(ini.GetValue("Color", "G"));
		newType.B = std::stoi(ini.GetValue("Color", "B"));		

		//Load particle effects.
		CSimpleIniA::TNamesDepend effects;
		ini.GetAllKeys("Effects", effects);

		for (CSimpleIniA::TNamesDepend::iterator iterator = effects.begin(); iterator != effects.end(); iterator++)
		{
			std::string a = iterator->pComment;
			std::string a = iterator->pItem;

			ParticleEffect newEffect;
			if (iterator->pComment == "ChangeTypeAtHighTemperature")
			{
				newEffect.effectType = EFFECT_CHANGE_TYPE_AT_HIGH_TEMPERATURE;
				newEffect.effectData = iterator->pItem;
			} 
			else if (iterator->pComment == "ChangeTypeAtLowTemperature")
			{
				newEffect.effectType = EFFECT_CHANGE_TYPE_AT_LOW_TEMPERATURE;
				newEffect.effectData = iterator->pItem;
			}

			if (newEffect.effectType != EFFECT_NULL)
				newType.particleEffects.push_back(newEffect);
		}

		// Push the new particle type into the list now that its created.
		ParticleTypes.push_back(newType);

		std::cout << "Sucessfully loaded particle type " + newType.name;

	}
	catch (std::exception e)
	{
		return false;
	}

	return true;
}

bool DirExists(std::string& folderName)
{
	// Get the attributes of the file/folder.
	DWORD fileAttributes = GetFileAttributesA(folderName.c_str());

	// Check if the attributes are invalid.
	if (fileAttributes == INVALID_FILE_ATTRIBUTES)
		return false;	

	// Check if the attributes say it is a directory.
	if (fileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		return true;
	
	// Otherwise it is not a directory but something else.
	return false;
}

bool Setup()
{
	// Initialize each SDL segment individually.
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		std::cout << ("Unable to initialize SDL_VIDEO: %s", SDL_GetError());
		return false;
	}

	if (SDL_Init(SDL_INIT_EVENTS) != 0)
	{
		std::cout << ("Unable to initialize SDL_EVENTS: %s", SDL_GetError());
		return false;
	}

	if (SDL_Init(SDL_INIT_TIMER) != 0)
	{
		std::cout << ("Unable to initialize SDL_TIMER: %s", SDL_GetError());
		return false;
	}

	if (TTF_Init() != 0)
	{
		std::cout << ("Unable to initialize TTF: %s", TTF_GetError());
		return false;
	}

	// Create main window.
	Uint32 windowFlags = SDL_WINDOW_OPENGL;
	mainWindow = SDL_CreateWindow(PROGRAM_NAME.c_str(), 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, windowFlags);

	mainSurface = SDL_GetWindowSurface(mainWindow);

	Uint32 renderFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
	mainRenderer = SDL_CreateRenderer(mainWindow, -1, renderFlags);

	// Setup ttf objects.
	if (!brushName.SetFont("pxl.ttf", 24))
	{
		std::cout << "Unable to load font pxl.ttf: " << TTF_GetError();
		return false;
	}

	if (!brushSize.SetFont("pxl.ttf", 24))
	{
		std::cout << "Unable to load font pxl.ttf: " << TTF_GetError();
		return false;
	}
	if (!brushTemperature.SetFont("pxl.ttf", 24))
	{
		std::cout << "Unable to load font pxl.ttf: " << TTF_GetError();
		return false;
	}
	if (!selectedParticleName.SetFont("pxl.ttf", 24))
	{
		std::cout << "Unable to load font pxl.ttf: " << TTF_GetError();
		return false;
	}
	if (!selectedParticleTemperature.SetFont("pxl.ttf", 24))
	{
		std::cout << "Unable to load font pxl.ttf: " << TTF_GetError();
		return false;
	}
	if (!selectedParticleHealth.SetFont("pxl.ttf", 24))
	{
		std::cout << "Unable to load font pxl.ttf: " << TTF_GetError();
		return false;
	}
	if (!currentFrameRate.SetFont("pxl.ttf", 24))
	{
		std::cout << "Unable to load font pxl.ttf: " << TTF_GetError();
		return false;
	}

	// Attempt to load all particle files in the Particles folder.
	char buf[256];
	GetCurrentDirectoryA(256, buf);
	std::string path(buf);
	path += "\\Particles";

	if (DirExists(path))
	{
		// Get all the files in the folder.
		WIN32_FIND_DATA fileFindData;
		HANDLE hFind = FindFirstFile((path + "\\*").c_str(), &fileFindData);

		std::vector<std::string> fileNames;

		// Make sure the handle is valid.
		if (hFind != INVALID_HANDLE_VALUE)
		{
			do
			{
				if (fileFindData.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)
					fileNames.push_back(fileFindData.cFileName);
			} while (FindNextFile(hFind, &fileFindData));

			FindClose(hFind);
		}

		// Check that there is files in this folder.
		if (fileNames.size() > 0)
		{	
			// Create an iterator of the fileNames vector and begin iterating through the list.
			for (std::vector<std::string>::const_iterator iterator = fileNames.begin(); iterator != fileNames.end(); iterator++)
			{
				CSimpleIniA ini;
				std::string str = path + "\\" + (*iterator);
				if (ini.LoadFile(str.c_str()) != SI_FAIL);				
					LoadParticle(&ini);				
			}
		}
		else
		{
			std::cout << "No particles found in Particle folder, program will now terminate.";
			return false;
		}

	}
	else
	{
		std::cout << "Particles folder does not exist, program will now terminate.";
		return false;
	}

	// SPEED TESTING LINES
	// DISABLE THESE IF NOT SPEED TESTING
	for (int i = 0; i < WINDOW_WIDTH; i++)
		for (int n = 0; n < (WINDOW_HEIGHT / 2); n++)
			CreateParticle("Sand", i, n, 30);

	SDL_MinimizeWindow(mainWindow);

	return true;
}


void Shutdown()
{
	// Destroy the main window before shutdown.
	SDL_DestroyWindow(mainWindow);

	// Destroy the main renderer before shutdown.
	SDL_DestroyRenderer(mainRenderer);

	// Clear up the pointer array used for the particles.
	for (int i = 0; i < WINDOW_WIDTH - 1; i++)
		for (int n = 0; n < WINDOW_HEIGHT - 1; n++)
		{
			delete allParticles[i][n];
			allParticles[i][n] = nullptr;
		}

	// Clean up all TTF files.
	brushName.Clear();
	brushSize.Clear();
	brushTemperature.Clear();
	selectedParticleTemperature.Clear();
	selectedParticleName.Clear();
	selectedParticleHealth.Clear();
	currentFrameRate.Clear();

	// Destroy the surface used for drawing particles.
	SDL_FreeSurface(mainSurface);
	if (mainSurface != nullptr)
		mainSurface = nullptr;

	SDL_Quit();
}
