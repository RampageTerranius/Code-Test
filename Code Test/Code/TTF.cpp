#include "TTF.h"

#include <SDL.h>
#include <SDL_ttf.h>

#include <string>

TTF::TTF()
{
	text = "";
	font = nullptr;
	color = { 255, 255 ,255, 0 };
	texture = nullptr;
}

// Cleans up any in use fonts and surfaces, should be called on closing program.
void TTF::Clear()
{
	if (texture != nullptr)
	{
		SDL_DestroyTexture(texture);
		if (texture != nullptr)
			texture = nullptr;
	}

	if (font != nullptr)
		TTF_CloseFont(font);
}

// Set the currently in use font.
bool TTF::SetFont(std::string fontLocation, int size)
{
	if (font == nullptr)
	{
		font = TTF_OpenFont(fontLocation.c_str(), size);
		if (font != nullptr)
			return true;
		else
			return false;
	}
	else
	{
		// Close the current font first THEN open the new one.
		TTF_CloseFont(font);
		font = TTF_OpenFont(fontLocation.c_str(), size);
		if (font != nullptr)
			return true;
		else
			return false;
	}
}

void TTF::SetColor(int r, int g, int b)
{
	color.r = r;
	color.g = g;
	color.b = b;
};

// Set the text to show, creates a surface with the given text.
void TTF::SetText(SDL_Renderer* renderer, std::string newText)
{
	if (font != nullptr)
	{
		// If we already have the same message then stop here.
		if (text == newText)
			return;
		else
			text = newText;

		// Delete the old texture if it exists.
		if (texture != nullptr)
			SDL_DestroyTexture(texture);

		if (texture != nullptr)
			texture = nullptr;

		// Create the new texture.
		SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
		texture = SDL_CreateTextureFromSurface(renderer, surface);

		SDL_FreeSurface(surface);
	}
}

// Draws the surface to the given renderer.
void TTF::Draw(SDL_Renderer* renderer, int x, int y)
{
	// Make sure we have data to work with.
	if (renderer != nullptr && texture != nullptr && font != nullptr)
	{
		// Prepare the render zones ahead of time.
		SDL_Rect rect;
		rect.x = x;
		rect.y = y;
		SDL_QueryTexture(texture, nullptr, nullptr, &rect.w, &rect.h);

		// Render the texture of the words to the given renderer.
		SDL_RenderCopy(renderer, texture, nullptr, &rect);
	};
}