#include <iostream>
#include <string>

#include "Game.h"
#include "SDL_ttf.h"
#include "Bitmap.h"
#include "Input.h"
#include "Hero.h"
#include "Monster.h"
#include "BoxCollider.h"
#include "CircleCollider.h"
#include "CanSee.h"

using namespace std;

const int FRAMES_PER_SECOND = 60;
const int MILLISECS_PER_FRAME = 1000 / FRAMES_PER_SECOND;

Game::Game()
	:m_pInput{ new Input }, m_windowWidth{ 1280 }, m_windowHeight{ 720 }
{
	m_pWindow = nullptr;
	m_pRenderer = nullptr;

	// Start up
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
	TTF_Init();

	// Create the window
	m_pWindow = SDL_CreateWindow(
		"SDL Platformer",  // Title
		250,				// Initial x
		50,					// Initial y
		m_windowWidth,				// Width
		m_windowHeight,				// Height
		0					// Window behaviour flags
	);

	if (!m_pWindow)
	{
		cout << "WINDOW initialisation failed: " << SDL_GetError() << endl;
		cout << "Press any key to continue." << endl;
		(void)getchar();
		m_isProgramRunning = false;
		return;
	}

	// Now create the renderer
	m_pRenderer = SDL_CreateRenderer(
		m_pWindow,	// Link the renderer to our newly created win
		-1,			// Index rendering driver
		0			// Renderer behaviour flags
	);

	if (!m_pRenderer)
	{
		cout << "RENDERER initialisation failed: " << SDL_GetError() << endl;
		cout << "Press any key to continue." << endl;
		(void)getchar();
		m_isProgramRunning = false;
		return;
	}

	// Read in the font
	m_pSmallFont = TTF_OpenFont("assets/DejaVuSans.ttf", 15);
	if (!m_pSmallFont)
	{
		cout << "SMALL_FONT initialisation failed: " << TTF_GetError() << endl;
		cout << "Press any key to continue." << endl;
		(void)getchar();
		m_isProgramRunning = false;
		return;
	}

	m_pBigFont = TTF_OpenFont("assets/DejaVuSans.ttf", 50);
	if (!m_pBigFont)
	{
		cout << "BIG_FONT initialisation failed: " << TTF_GetError() << endl;
		cout << "Press any key to continue." << endl;
		(void)getchar();
		m_isProgramRunning = false;
		return;
	}

	m_pBackground = new Bitmap(m_pRenderer, "Assets/Background.bmp");

	// Timer
	m_updateLogic = true;
	m_renderFrame = true;
	m_consecutiveLogicUpdates = 0;
	gameTimer = SDL_AddTimer(MILLISECS_PER_FRAME, &Game::TimerCallback, this);

	SetDisplayColour(255, 255, 255, 255);
}

Game::~Game()
{
	//// Clean up
	//// Destroy in the reverse of created order
	//Timer
	SDL_RemoveTimer(gameTimer);

	UnLoadLevel();

	// Fonts
	TTF_CloseFont(m_pBigFont);
	TTF_CloseFont(m_pSmallFont);

	// Arena
	delete m_pLevel;
	m_pLevel = nullptr;

	// Renderer
	if (m_pRenderer)
	{
		SDL_DestroyRenderer(m_pRenderer);
	}

	// Window
	if (m_pWindow)
	{
		SDL_DestroyWindow(m_pWindow);
	}

	delete m_pInput;
	m_pInput = nullptr;
}

void Game::Update()
{	
	if (m_isGameRunning)
	{
		m_updateLogic = false;
		m_renderFrame = false;
		CheckEvents();

		if (m_updateLogic)
		{
			if (GameInput())
			{
				m_pHero->Update(m_pTrees);

				// Move viewport
				Vector2 heroPos = m_pHero->GetPosition();
				int newViewportX = static_cast<int>(heroPos.x) + m_pHero->GetWidth() / 2 - m_pLevel->GetViewportWidth() / 2;
				int newViewportY = static_cast<int>(heroPos.y) + m_pHero->GetHeight() / 2 - m_pLevel->GetViewportHeight() / 2;

				m_pLevel->SetViewportX(newViewportX);
				m_pLevel->SetViewportY(newViewportY);

				for (unsigned i = 0; i < m_pMonsters.size(); i++)
				{
					if (m_pMonsters[i]->GetHealth() <= 0)
					{
						for (unsigned j = 0; j < m_pGameObjects.size(); j++)
						{
							if (m_pGameObjects[j] == m_pMonsters[i])
								m_pGameObjects.erase(m_pGameObjects.begin() + j);
						}

						delete m_pMonsters[i];
						m_pMonsters.erase(m_pMonsters.begin() + i);
						++m_score;

						break;
					}

					m_pMonsters[i]->Update(m_pTrees, m_pHero);

					for (Monster* collisionMonster : m_pMonsters)
					{
						if (m_pMonsters[i]->GetCollider()->CollidesWith(collisionMonster->GetCollider()))
						{
							m_pMonsters[i]->RevertPosition();
						}
					}

					if (m_pMonsters[i]->GetCollider()->CollidesWith(m_pHero->GetCollider()))
					{
						HurtPlayerWith(m_pMonsters[i]);
						return;
					}
				}

				for (Mushroom* p_mushroom : m_pMushrooms)
				{
					p_mushroom->Update(m_pTrees);

					if (p_mushroom->GetCollider()->CollidesWith(m_pHero->GetCollider()))
					{
						if (!p_mushroom->isCollingWithPlayer())
						{
							m_pHero->SetYVelocity(0);
							m_pHero->Push({ 0, -12 });
							p_mushroom->SetCollidingWithPlayer(true);
						}
					}
					else
					{
						p_mushroom->SetCollidingWithPlayer(false);
					}
				}

				for (unsigned i = 0; i < m_pFires.size(); i++)
				{
					if (!m_pFires[i]->GetIsAlive())
					{
						delete m_pFires[i];
						for (unsigned j = 0; j < m_pGameObjects.size(); j++)
						{
							if (m_pGameObjects[j] == m_pFires[i])
								m_pGameObjects.erase(m_pGameObjects.begin() + j);
						}
						m_pFires.erase(m_pFires.begin() + i);
					}
					else if (m_pFires[i]->GetCollider()->CollidesWith(m_pHero->GetCollider()))
					{
						HurtPlayerWith(m_pFires[i]);
						return;
					}
				}

				for (unsigned i = 0; i < m_pTrees.size(); i++)
				{
					if (!m_pTrees[i]->GetIsAlive())
					{
						delete m_pTrees[i];
						for (unsigned j = 0; j < m_pGameObjects.size(); j++)
						{
							if (m_pGameObjects[j] == m_pTrees[i])
								m_pGameObjects.erase(m_pGameObjects.begin() + j);
						}
						m_pTrees.erase(m_pTrees.begin() + i);
					}
				}

				for (unsigned i = 0; i < m_pGoldBars.size(); i++)
				{
					m_pGoldBars[i]->Update();

					if (m_pGoldBars[i]->GetCollider()->CollidesWith(m_pHero->GetCollider()))
					{
						for (unsigned j = 0; j < m_pGameObjects.size(); j++)
						{
							if (m_pGameObjects[j] == m_pGoldBars[i])
								m_pGameObjects.erase(m_pGameObjects.begin() + j);
						}

						++m_score;
						delete m_pGoldBars[i];
						m_pGoldBars.erase(m_pGoldBars.begin() + i);
					}
				}

				if (m_pLevel->GetLevelFileName() == "Assets/LevelOneData.txt" && m_score >= 50)
				{
					UnLoadLevel();
					LoadLevel("Assets/LevelTwoData.txt");
				}

			}
			else
			{
				UnLoadLevel();
				m_renderFrame = false;
			}
		}

		if (m_renderFrame)
		{
			Render();
		}
	}
	else
	{
		ClearDisplay();
		DrawText("E = Play", 25, 25, m_pBigFont, {0, 0, 0, 255});
		DrawText("Esc = Quit", 25, 75, m_pBigFont, { 0, 0, 0, 255 });
		DrawText("Controls:", m_windowWidth - 550, 25, m_pBigFont, { 0, 0, 0, 255 });
		DrawText("Arrow Keys = Move", m_windowWidth - 550, 75, m_pBigFont, { 0, 0, 0, 255 });
		DrawText("SPACE = Jump", m_windowWidth - 550, 125, m_pBigFont, { 0, 0, 0, 255 });
		DrawText("E = Interact", m_windowWidth - 550, 175, m_pBigFont, { 0, 0, 0, 255 });
		DrawText("F = Swap element", m_windowWidth - 550, 225, m_pBigFont, { 0, 0, 0, 255 });
		DrawText("ESC = Exit to menu", m_windowWidth - 550, 275, m_pBigFont, { 0, 0, 0, 255 });
		DrawText("Accumulate 50 score to complete a level", 25, 600, m_pBigFont, { 0, 0, 0, 255 });
		UpdateDisplay();

		CheckEvents();
		m_pInput->Update();

		if (m_pInput->GetKeyDown(KEYS_PRESSED_LIST::KEY_ESCAPE))
		{
			m_isProgramRunning = false;
			return;
		}
		else if (m_pInput->GetKeyDown(KEYS_PRESSED_LIST::KEY_E))
		{
			m_isGameRunning = true;
			LoadLevel("Assets/LevelOneData.txt");
		}
	}
}

bool Game::GetProgramRunning()
{
	return m_isProgramRunning;
}

void Game::HurtPlayerWith(GameObject* damageSource)
{
	Vector2 direction = {
	m_pHero->GetPosition().x + static_cast<float>(m_pHero->GetWidth() - 1),
	m_pHero->GetPosition().y + static_cast<float>(m_pHero->GetHeight() - 1)
	};
	direction.x -= damageSource->GetPosition().x + static_cast<float>(damageSource->GetWidth() - 1);
	direction.y -= damageSource->GetPosition().y, static_cast<float>(damageSource->GetHeight() - 1);
	direction /= sqrt(direction.x * direction.x + direction.y * direction.y);

	m_pHero->SetXVelocity(0);
	m_pHero->SetYVelocity(0);
	m_pHero->Push(direction * 20.0f);
	m_pHero->DecrementHealth(15.0f);

	if (m_pHero->GetHealth() <= 0)
	{
		m_isGameRunning = false;
		UnLoadLevel();
	}
}

void Game::SetDisplayColour(int r, int g, int b, int a)
{
	if (m_pRenderer)
	{
		// Render in a pretty red colour
		int result = SDL_SetRenderDrawColor(
			m_pRenderer,
			r,
			g,
			b,
			a
		);
	}
}

void Game::ClearDisplay(void)
{
	// Wipe the display to the colour we just set.
	SDL_RenderClear(m_pRenderer);
}

void Game::UpdateDisplay(void)
{
	// Show what we've drawn
	SDL_RenderPresent(m_pRenderer);
}

void Game::WaitForMillisecs(int millisecs)
{
	SDL_Delay(millisecs); // SDL_Delay takes millisecs
}

void Game::DrawText(string message, int x, int y, TTF_Font* font, SDL_Color colour)
{
	SDL_Surface* surface = nullptr;
	SDL_Texture* texture = nullptr;

	int textW = 0;
	int textH = 0;

	surface = TTF_RenderText_Solid(font, message.c_str(), colour);
	if (!surface)
	{
		// Surface not loaded? Output the error
		cout << "SURFACE for font not loaded!" << endl;
		cout << SDL_GetError() << endl;
	}
	else
	{
		texture = SDL_CreateTextureFromSurface(m_pRenderer, surface);
		if (!texture)
		{
			// Texture not loaded? Output the error
			cout << "TEXTURE for font not loaded!" << endl;
			cout << SDL_GetError() << endl;
		}
		else
		{
			SDL_QueryTexture(texture, NULL, NULL, &textW, &textH);
			SDL_Rect textRect = { x, y, textW, textH };

			SDL_RenderCopy(m_pRenderer, texture, NULL, &textRect);
		}
	}

	if (texture)
		SDL_DestroyTexture(texture);

	if (surface)
		SDL_FreeSurface(surface);
}

void Game::DrawHUD()
{
	string scoreString = "Score: ";
	scoreString += to_string(m_score);
	DrawText(scoreString, 4, 4, m_pBigFont, { 0, 0, 0, 255 });

	string healthString = "Health: ";
	healthString += to_string(static_cast<int>(m_pHero->GetHealth()));
	DrawText(healthString, m_windowWidth - 300, 4, m_pBigFont, { 0, 0, 0, 255 });

	string elementText;
	if (m_pHero->GetElementType() == ElementType::WATER)
	{
		elementText = "WATER";
	}
	else
	{
		elementText = "FIRE";
	}
	DrawText(
		elementText, 
		static_cast<int>(m_pHero->GetPosition().x) - m_pLevel->GetViewportX(), 
		static_cast<int>(m_pHero->GetPosition().y) - 15 - m_pLevel->GetViewportY(), m_pSmallFont, 
		{0, 0, 0, 255}
	);

	for (Monster* p_monster : m_pMonsters)
	{
		string monsterText = to_string(static_cast<int>(p_monster->GetHealth()));
		if (p_monster->GetElementType() == ElementType::FIRE)
		{
			monsterText += " FIRE";
		}
		else
		{
			monsterText += " WATER";
		}

		DrawText(
			monsterText,
			static_cast<int>(p_monster->GetPosition().x) - m_pLevel->GetViewportX(),
			static_cast<int>(p_monster->GetPosition().y) - 15 - m_pLevel->GetViewportY(), m_pSmallFont,
			{ 0, 0, 0, 255 }
		);
	}
}

Uint32 Game::TimerCallback(Uint32 interval, void* gameObjectIn)
{
	if (gameObjectIn)
	{
		Game* gameObject = static_cast<Game*>(gameObjectIn);
		gameObject->CreateTimerEvent();
	}

	return interval;
}

void Game::CreateTimerEvent()
{
	SDL_Event event;
	SDL_UserEvent userEvent;

	//set up userEvent with default data
	userEvent.type = SDL_USEREVENT;
	userEvent.data1 = NULL;
	userEvent.data2 = NULL;

	//this is a TIMER event
	userEvent.code = GE_TIMER;

	//create the event
	event.type = SDL_USEREVENT;
	event.user = userEvent;

	//push the event
	SDL_PushEvent(&event);
}

void Game::CheckEvents()
{
	SDL_Event event;

	//poll the event queue
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_USEREVENT:
			{
				if (event.user.code == GE_TIMER)
				{
					//we have a ping

					//if logic taking too long ensures frame is rendered occasionally and input is responsive
					//this essentailly ignores logic updates if there are a lot of consecutive ones
					if (++m_consecutiveLogicUpdates < m_maxConsecutiveLogicUpdates)
					{
						m_updateLogic = true;
					}

					//only render frame if no events left to process
					if (SDL_PeepEvents(NULL, 1, SDL_PEEKEVENT, SDL_USEREVENT, SDL_USEREVENT) == 0)
					{
						m_renderFrame = true;
						m_consecutiveLogicUpdates = 0;
					}
					break;
				}
				else if (event.user.code == GE_ANIMATION_TIMER)
				{
					for (unsigned i = 0; i < m_pGameObjects.size(); i++)
					{
						if (event.user.data1 == m_pGameObjects[i])
						{
							m_pGameObjects[i]->PushEvent(event);
						}
					}
					break;
				}
				else if(event.user.code == GE_INTERACT)
				{ 
					if (m_pHero)
					{
						Vector2 playerCenter = m_pHero->GetPosition();
						playerCenter.x += m_pHero->GetWidth() / 2 - 0.5f;
						playerCenter.y += m_pHero->GetHeight() / 2 - 0.5f;

						CircleCollider* p_collider = new CircleCollider(playerCenter, m_interactDist);
						for (GameObject* p_gameObject : m_pGameObjects)
						{
							if (p_gameObject != m_pHero && p_collider->CollidesWith(p_gameObject->GetCollider()))
							{
								// Test line of sight
								Vector2 originPos = m_pHero->GetPosition();
								originPos.x += static_cast<float>(m_pHero->GetWidth() / 2 - 0.5f);
								originPos.y += static_cast<float>(m_pHero->GetHeight() / 4 - 0.25f);

								Vector2 targetPos = p_gameObject->GetPosition();
								targetPos.x += static_cast<float>(p_gameObject->GetWidth() / 2 - 0.5f);
								targetPos.y += static_cast<float>(p_gameObject->GetHeight() / 2 - 0.5f);

								if (CanSee(originPos, targetPos, m_halfBlockWidth, m_halfBlockHeight, m_pLevel))
								{
									p_gameObject->Interact(m_pHero->GetElementType());
								}
								else
								{
									// Can see top?
									targetPos = p_gameObject->GetPosition();
									targetPos.x += static_cast<float>(p_gameObject->GetWidth() / 2 - 0.5f);

									if (CanSee(originPos, targetPos, m_halfBlockWidth, m_halfBlockHeight, m_pLevel))
									{
										p_gameObject->Interact(m_pHero->GetElementType());
									}
									else
									{
										// Can see bottom?
										targetPos = p_gameObject->GetPosition();
										targetPos.x += static_cast<float>(p_gameObject->GetWidth() / 2 - 0.5f);
										targetPos.y += static_cast<float>(p_gameObject->GetHeight() - 1);

										if (CanSee(originPos, targetPos, m_halfBlockWidth, m_halfBlockHeight, m_pLevel))
										{
											p_gameObject->Interact(m_pHero->GetElementType());
										}
									}
								}
							}
						}
					}
				}

				break;
			}

			default:
			{
				m_pInput->PushEvent(event);
				break;
			}
		}
	}
}

bool Game::GameInput()
{
	// Get input
	m_pInput->Update();

	// Run game until escape pressed
	if (m_pInput->KeyIsPressed(KEYS_PRESSED_LIST::KEY_ESCAPE))
	{
		m_isGameRunning = false;
		return false;
	}

	m_pHero->HandleInput(m_pInput, m_pTrees);
	return true;
}

void Game::Render()
{
	// Reset display
	float speedScale = 0.25;
	Vector2 screenPos;
	screenPos.x = -(m_pLevel->GetViewportX() * speedScale);
	screenPos.y = -(m_pLevel->GetViewportY() * speedScale);
	m_pBackground->Draw((int)screenPos.x, (int)screenPos.y);

	// Draw things
	m_pLevel->Render();
	
	for (GameObject* p_gameObject : m_pGameObjects)
	{
		p_gameObject->Draw();
	}

	DrawHUD();

	// Render window
	UpdateDisplay();
}

void Game::LoadLevel(string levelFile)
{
	m_pLevel = new Level(levelFile, "Assets/wall.bmp", "Assets/platform.bmp", m_blockWidth, m_blockHeight, m_pRenderer);
	m_pLevel->SetGravityScale(0.2f);
	m_pGameObjects = m_pLevel->GetGameObjects();

	for (GameObject* p_gameObject : m_pGameObjects)
	{
		if (p_gameObject->GetName() == "GoldBar")
		{
			m_pGoldBars.push_back(static_cast<GoldBar*>(p_gameObject));
		}
		else if (p_gameObject->GetName() == "Monster")
		{
			m_pMonsters.push_back(static_cast<Monster*>(p_gameObject));
		}
		else if (p_gameObject->GetName() == "Mushroom")
		{
			m_pMushrooms.push_back(static_cast<Mushroom*>(p_gameObject));
		}
		else if (p_gameObject->GetName() == "Tree")
		{
			m_pTrees.push_back(static_cast<Tree*>(p_gameObject));
		}
		else if (p_gameObject->GetName() == "Fire")
		{
			m_pFires.push_back(static_cast<Fire*>(p_gameObject));
		}
		else if (p_gameObject->GetName() == "Player")
		{
			m_pHero = static_cast<Hero*>(p_gameObject);
		}
	}
}

void Game::UnLoadLevel()
{
	for (GameObject* p_gameObject : m_pGameObjects)
	{
		delete p_gameObject;
	}

	m_pGameObjects.clear();
	m_pGoldBars.clear();
	m_pTrees.clear();
	m_pFires.clear();
	m_pMonsters.clear();
	m_pMushrooms.clear();
	m_pHero = nullptr;

	delete m_pLevel;
	m_pLevel = nullptr;

	m_score = 0;
}
