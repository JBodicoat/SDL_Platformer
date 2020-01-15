#pragma once
#include <SDL.h>
#include <vector>
#include <string>
#include "Bitmap.h"
#include "GameObject.h"

using namespace std;
struct SDL_Texture;
struct SDL_Renderer;
class GameObject;

class Level
{
private:
	SDL_Texture* m_pBlockTexture{ nullptr };
	SDL_Texture* m_pPlatformTexture{ nullptr };
	SDL_Renderer* m_pRenderer{ nullptr };

	const int m_blockWidth{ 0 };
	const int m_blockHeight{ 0 };
	const float m_halfBlockWidth = static_cast<float>(m_blockWidth) / 2.0f;
	const float m_halfBlockHeight = static_cast<float>(m_blockHeight) / 2.0f;

	float m_gravityScale{ 0.2f };

	vector<string> arenaData;

	Bitmap* m_pWallBitmap{ nullptr };
	Bitmap* m_pPlatformBitmap{ nullptr };
	const bool m_usingBitmap{ false };
	vector<GameObject*> m_pGameObjects;
	void ReadInLevelData(string fileName);

	int m_viewportX{ 0 };
	int m_viewportY{ 0 };
	int m_windowWidth{ 0 };
	int m_windowHeight{ 0 };

	string m_levelFileName{ "" };

public:
	Level(
		string levelFileName, 
		string bitmapFileName, 
		string platformFileName, 
		int blockWidth, 
		int blockHeight, 
		SDL_Renderer* renderer);
	~Level();

	int GetWorldHeightInBlocks();
	int GetWorldWidthInBlocks(int row);

	int GetBlockHeight();
	int GetBlockWidth();
	float GetHalfBlockHeight();
	float GetHalfBlockWidth();

	void Render();

	bool isWall(int x, int y);
	bool isPlatform(int x, int y);

	float GetGravityScale();
	void SetGravityScale(float newGravityScale);

	int GetViewportX();
	int GetViewportY();
	int GetViewportWidth();
	int GetViewportHeight();
	void SetViewportX(int newX);
	void SetViewportY(int newY);

	vector<GameObject*> GetGameObjects();

	string GetLevelFileName();
};

