#pragma once
#include <string>
#include <iostream>
#include <vector>
#include "SDL_ttf.h"
#include "Bitmap.h"
#include "Input.h"
#include "Monster.h"
#include "Hero.h"
#include "Level.h"
#include "GoldBar.h"
#include "Enums.h"
#include "Fire.h"
#include "Tree.h"
#include "Mushroom.h"

using namespace std;

//forward declare any SDL stuff we need
struct SDL_Window;
struct SDL_Renderer;

//forward declare any classes we're using
class Input;
class Bitmap;
class Monster;
class Hero;

class Game
{
private:
	// SDL stuff
	SDL_Window* m_pWindow{ nullptr };
	SDL_Renderer* m_pRenderer{ nullptr };

	const int m_windowWidth;
	const int m_windowHeight;

	Input* m_pInput{ new Input };

	// Fonts
	TTF_Font* m_pSmallFont{ nullptr };
	TTF_Font* m_pBigFont{ nullptr };

	vector<GameObject*> m_pGameObjects;
	vector<Monster*> m_pMonsters;
	vector<Mushroom*> m_pMushrooms;
	vector<Fire*> m_pFires;
	vector<Tree*> m_pTrees;
	vector<GoldBar*> m_pGoldBars;

	Hero* m_pHero{ nullptr };
	int m_score{ 0 };
	const float m_interactDist{ 96.0f };

	Level* m_pLevel{ nullptr };
	const int m_blockWidth{ 64 };
	const int m_blockHeight{ 64 };
	const float m_halfBlockWidth = static_cast<float>(m_blockWidth) / 2.0f;
	const float m_halfBlockHeight = static_cast<float>(m_blockHeight) / 2.0f;

	bool m_isProgramRunning{ true };
	bool m_isGameRunning{ false };

	void DrawText(
		string message, 
		int x, 
		int y, 
		TTF_Font* font, 
		SDL_Color colour);
	void DrawHUD();

	// Consistent game time
	SDL_TimerID gameTimer{ NULL };

	bool m_updateLogic{ true };
	bool m_renderFrame{ true };
	int m_consecutiveLogicUpdates{ 0 };
	const int m_maxConsecutiveLogicUpdates = 10;

	Bitmap* m_pBackground{ nullptr };

	void HurtPlayerWith(GameObject* damageSource);
	void SetDisplayColour(int r, int g, int b, int a = 255);
	void ClearDisplay(void);
	void UpdateDisplay(void);
	void WaitForMillisecs(int millisecs);
	static Uint32 TimerCallback(Uint32 interval, void* gameObjectIn);
	void CreateTimerEvent();
	void CheckEvents();
	bool GameInput();
	void Render();
	void LoadLevel(string levelFile);
	void UnLoadLevel();

public:
	Game();
	~Game();

	void Update();
	bool GetProgramRunning();

};
