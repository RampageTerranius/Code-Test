#pragma once

#include <string>

//used to help objects keep track of what they are for rendering purposes as the lists will be using base particles that are using polymorphism to determine how to handle physics
enum ParticleType
{
	TYPE_WALL,
	TYPE_SAND,
	TYPE_WATER,
	TYPE_ICE,
	TYPE_THERMALFLUID,
	TYPE_ACID,
	TYPE_STEAM,
	TYPE_PLANT,
	TYPE_SALT,
	TYPE_SALTWATER,
	TYPE_SALTICE,
	TYPE_GLITCH,
	TYPE_STONE,
	TYPE_LAVA,

	//the following must ALWAYS be at the end
	TYPE_SOURCE,
	TYPE_TOTALTYPES
};

//used to give a name to each particle type
static std::string typeNames[] = {  "Wall",
									"Sand",
									"Water",
									"Ice",
									"Thermal Fluid",
									"Acid",
									"Steam",
									"Plant",
									"Salt",
									"Salt Water",
									"Salt Ice",
									"GLITCH",
									"Stone",
									"Lava",

									//the following must ALWAYS be at the end
									"Source - ",
									"TotalTypes" };//you should never be using this final string, if you are your working with a non existant particale type