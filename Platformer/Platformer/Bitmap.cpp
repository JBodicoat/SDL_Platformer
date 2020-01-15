#include <string>
#include <iostream>

#include "Bitmap.h"
#include "SDL.h"
#include "SDL_render.h"

using namespace std;

Bitmap::Bitmap(SDL_Renderer* renderer, string fileName, bool useTransparency)
	: m_pRenderer{ renderer }
{
	// crete the bitmap surface
	m_pbitmapSurface = SDL_LoadBMP(fileName.c_str());

	if (!m_pbitmapSurface)
	{
		//bitmap not loaded? Output the error
		cout << "SURFACE for bitmap " << fileName << " not loaded!" << endl;
		cout << SDL_GetError() << endl;
	}
	else
	{
		//if we are to use the ransparency, going to assume
		//the colour key is magenta (r = 255, g = 0, b = 255)
		if (useTransparency)
		{
			Uint32 colourKey = SDL_MapRGB(m_pbitmapSurface->format, 255, 0, 255);
			SDL_SetColorKey(m_pbitmapSurface, static_cast<int>(SDL_bool::SDL_TRUE), colourKey);
		}

		m_width = m_pbitmapSurface->w;
		m_height = m_pbitmapSurface->h;

		//create the texture
		m_pbitmapTexture = SDL_CreateTextureFromSurface(m_pRenderer, m_pbitmapSurface);
		if (!m_pbitmapTexture)
		{
			//texture not loaded? Output the error
			cout << "TEXTURE for bitmap " << fileName << " not loaded!" << endl;
			cout << SDL_GetError() << endl;
		}
	}
}

Bitmap::~Bitmap()
{
	if (m_pbitmapTexture)
		SDL_DestroyTexture(m_pbitmapTexture);

	if (m_pbitmapSurface)
		SDL_FreeSurface(m_pbitmapSurface);
}

void Bitmap::Draw(int x, int y)
{
	//render the bitmap at the x/y coords
	if (m_pbitmapTexture)
	{
		int flip = SDL_FLIP_NONE;

		if (hFlip && vFlip)
		{
			// Flip horizontally & vertically
			flip = SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL;
		}
		else if (hFlip)
		{
			// Flip horizontally
			flip = SDL_FLIP_HORIZONTAL;
		}
		else if (vFlip)
		{
			// Flip vertically
			flip = SDL_FLIP_VERTICAL;
		}

		SDL_Rect destRect = { x, y, m_pbitmapSurface->w, m_pbitmapSurface->h };
		//SDL_RenderCopy(m_pRenderer, m_pbitmapTexture, NULL, &destRect);

		SDL_RenderCopyEx(m_pRenderer, m_pbitmapTexture, NULL, &destRect, 0, NULL, static_cast<SDL_RendererFlip>(flip));
	}
}

int Bitmap::GetWidth()
{
	return m_width;
}

int Bitmap::GetHeight()
{
	return m_height;
}