#pragma once

#include <string>

#include "SDL.h"
#include "SDL_render.h"

struct SDL_Surface;
struct SDL_Texture;
struct SDL_Renderer;

class Bitmap
{
private:
	SDL_Surface* m_pbitmapSurface{ nullptr };
	SDL_Texture* m_pbitmapTexture{ nullptr };
	SDL_Renderer* m_pRenderer{ nullptr };

	int m_width{ 0 };
	int m_height{ 0 };

public:
	Bitmap(SDL_Renderer* renderer, std::string fileName, bool useTransparency = false);
	~Bitmap();

	void Draw(int x, int y);
	int GetWidth();
	int GetHeight();
	bool hFlip{ false }, vFlip{ false };
};

