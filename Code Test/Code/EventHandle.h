#pragma once

#include "ParticleTypes.h"

#include <string>
#include <SDL.h>

void SwitchBrushType(bool gotoNext);
void UpdateEventStructs(SDL_Event event);
void CreateParticlesAtBrush(std::string type, int x, int y, float temperature);
void DestroyParticlesAtBrush(int x, int y);
void EventHandle(SDL_Event& event);