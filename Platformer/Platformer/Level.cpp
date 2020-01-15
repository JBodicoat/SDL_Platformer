#include "Level.h"
#include "GoldBar.h"
#include "Monster.h"
#include "Hero.h"
#include "BoxCollider.h"
#include "GameObject.h"
#include "Tree.h"
#include "Fire.h"
#include "Mushroom.h"
#include <iostream>
#include <fstream>

Level::Level(string levelFileName, string bitmapFileName, string platformFileName, int blockWidth, int blockHeight, SDL_Renderer* renderer)
	:m_levelFileName{ levelFileName }, m_blockWidth{ blockWidth }, m_blockHeight{ blockHeight }, m_pRenderer{ renderer }, m_usingBitmap{ true }
{
	ReadInLevelData(m_levelFileName);

	m_pWallBitmap = new Bitmap{ renderer, bitmapFileName, true };
	m_pPlatformBitmap = new Bitmap{ renderer, platformFileName, true };

	SDL_Surface* platformSurface = SDL_CreateRGBSurface(0, m_blockWidth, m_blockHeight, 32, 0, 0, 0, 0);
	Uint32 platformColour = SDL_MapRGB(platformSurface->format, 0, 0, 255);
	SDL_FillRect(platformSurface, NULL, platformColour);
	m_pPlatformTexture = SDL_CreateTextureFromSurface(m_pRenderer, platformSurface);
	SDL_FreeSurface(platformSurface);

	SDL_GetRendererOutputSize(renderer, &m_windowWidth, &m_windowHeight);
}

Level::~Level()
{
	if (m_pBlockTexture)
	{
		SDL_DestroyTexture(m_pBlockTexture);
	}
}

void Level::ReadInLevelData(string fileName)
{
	ifstream levelFile(fileName);
	if (levelFile.good())
	{
		string currentLine;
		while (getline(levelFile, currentLine))
			arenaData.push_back(currentLine);

		for (unsigned int row = 0; row < arenaData.size(); row++)
		{
			for (unsigned int column = 0; column < arenaData[row].size(); column++)
			{
				switch (arenaData[row][column])
				{
					case 'G':
					{
						// Spawn gold
						float x = static_cast<float>(column * m_blockWidth);
						float y = static_cast<float>(row * m_blockHeight);

						GoldBar* p_goldBar = new GoldBar{
							"GoldBar",
							{x, y},
							ElementType::NONE,
							this
							};

						p_goldBar->AddFrame("Assets/Goldbar_0.bmp", m_pRenderer);
						p_goldBar->AddFrame("Assets/Goldbar_1.bmp", m_pRenderer);

						p_goldBar->SetCollider(new BoxCollider(
							{ x, y },
							static_cast<float>(p_goldBar->GetWidth()),
							static_cast<float>(p_goldBar->GetHeight())));

						m_pGameObjects.push_back(p_goldBar);
						break;
					}

					case 'M':
					{
						// Spawn monster
						float x = static_cast<float>(column * m_blockWidth);
						float y = static_cast<float>(row * m_blockHeight);

						Monster* p_monster = new Monster{
							"Monster",
							{x, y},
							ElementType::FIRE,
							2.0f,
							20.0f,
							this
							};

						p_monster->AddFrame("Assets/Monster_Run_0.bmp", m_pRenderer, CreatureAnimations::RUN);
						p_monster->AddFrame("Assets/Monster_Run_1.bmp", m_pRenderer, CreatureAnimations::RUN);
						p_monster->AddFrame("Assets/Monster_Run_2.bmp", m_pRenderer, CreatureAnimations::RUN);
						p_monster->AddFrame("Assets/Monster_Run_3.bmp", m_pRenderer, CreatureAnimations::RUN);

						p_monster->AddFrame("Assets/Monster_Idle_0.bmp", m_pRenderer, CreatureAnimations::IDLE);
						p_monster->AddFrame("Assets/Monster_Idle_1.bmp", m_pRenderer, CreatureAnimations::IDLE);
						p_monster->AddFrame("Assets/Monster_Idle_2.bmp", m_pRenderer, CreatureAnimations::IDLE);

						p_monster->SetCollider(new BoxCollider(
							{ x, y },
							static_cast<float>(p_monster->GetWidth()),
							static_cast<float>(p_monster->GetHeight())));

						m_pGameObjects.push_back(p_monster);
						break;
					}

					case 'S':
					{
						// Spawn Mushroom
						float x = static_cast<float>(column * m_blockWidth);
						float y = static_cast<float>(row * m_blockHeight);

						Mushroom* p_mushroom = new Mushroom{
							"Mushroom",
							{x, y},
							ElementType::NONE,
							3.0f,
							100.0f,
							this
							};

						p_mushroom->AddFrame("Assets/Mushroom_Run_0.bmp", m_pRenderer, CreatureAnimations::RUN);
						p_mushroom->AddFrame("Assets/Mushroom_Run_1.bmp", m_pRenderer, CreatureAnimations::RUN);
						p_mushroom->AddFrame("Assets/Mushroom_Run_2.bmp", m_pRenderer, CreatureAnimations::RUN);
						p_mushroom->AddFrame("Assets/Mushroom_Run_3.bmp", m_pRenderer, CreatureAnimations::RUN);

						p_mushroom->AddFrame("Assets/Mushroom_Idle_0.bmp", m_pRenderer, CreatureAnimations::IDLE);
						p_mushroom->AddFrame("Assets/Mushroom_Idle_1.bmp", m_pRenderer, CreatureAnimations::IDLE);
						p_mushroom->AddFrame("Assets/Mushroom_Idle_2.bmp", m_pRenderer, CreatureAnimations::IDLE);
						p_mushroom->AddFrame("Assets/Mushroom_Idle_3.bmp", m_pRenderer, CreatureAnimations::IDLE);

						p_mushroom->SetCollider(new BoxCollider(
							{ x, y },
							static_cast<float>(p_mushroom->GetWidth()),
							static_cast<float>(p_mushroom->GetHeight())));

						m_pGameObjects.push_back(p_mushroom);

						break;
					}

					case 'H':
					{
						// Spawn hero
						float x = static_cast<float>(column * m_blockWidth);
						float y = static_cast<float>(row * m_blockHeight);

						Hero* p_hero = new Hero{
							"Player",
							{x, y},
							ElementType::WATER,
							5.0f,
							100.0f,
							this
							};

						p_hero->AddFrame("Assets/Player_Walk_1.bmp", m_pRenderer, CreatureAnimations::RUN);
						p_hero->AddFrame("Assets/Player_Walk_2.bmp", m_pRenderer, CreatureAnimations::RUN);
						p_hero->AddFrame("Assets/Player_Walk_3.bmp", m_pRenderer, CreatureAnimations::RUN);
						p_hero->AddFrame("Assets/Player_Walk_4.bmp", m_pRenderer, CreatureAnimations::RUN);
						p_hero->AddFrame("Assets/Player_Walk_5.bmp", m_pRenderer, CreatureAnimations::RUN);

						p_hero->AddFrame("Assets/Player_Idle_0.bmp", m_pRenderer, CreatureAnimations::IDLE);
						p_hero->AddFrame("Assets/Player_Idle_1.bmp", m_pRenderer, CreatureAnimations::IDLE);
						p_hero->AddFrame("Assets/Player_Idle_2.bmp", m_pRenderer, CreatureAnimations::IDLE);
						p_hero->AddFrame("Assets/Player_Idle_3.bmp", m_pRenderer, CreatureAnimations::IDLE);
						p_hero->AddFrame("Assets/Player_Idle_4.bmp", m_pRenderer, CreatureAnimations::IDLE);

						p_hero->SetCollider(new BoxCollider(
							{ x, y },
							static_cast<float>(p_hero->GetWidth()),
							static_cast<float>(p_hero->GetHeight())));

						m_pGameObjects.push_back(p_hero);
						break;
					}

					case 'T':
					{
						float x = static_cast<float>(column * m_blockWidth);
						float y = static_cast<float>(row * m_blockHeight);

						m_pGameObjects.push_back(new Tree{
							"Tree",
							{x, y},
							"Assets/Tree_Base.bmp",
							"Assets/Tree_Mid.bmp",
							"Assets/Tree_Top.bmp",
							m_pRenderer,
							this
							});
						break;
					}

					case 'F':
					{
						float x = static_cast<float>(column* m_blockWidth);
						float y = static_cast<float>(row* m_blockHeight);

						m_pGameObjects.push_back(new Fire{
							"Fire",
							{x, y},
							"Assets/Fire.bmp",
							m_pRenderer,
							this
							});
						break;
					}
				}
			}
		}
	}
	else
	{
		cout << "Level file could not be opened" << endl;
	}

	if (levelFile.is_open())
	{
		levelFile.close();
	}
}

int Level::GetWorldHeightInBlocks()
{
	return arenaData.size();
}

int Level::GetWorldWidthInBlocks(int index)
{
	if (arenaData.size() > 0)
	{
		return arenaData[index].length();
	}

	return 0;
}

int Level::GetBlockHeight()
{
	return m_blockHeight;
}

int Level::GetBlockWidth()
{
	return m_blockWidth;
}

float Level::GetHalfBlockHeight()
{
	return m_halfBlockHeight;
}

float Level::GetHalfBlockWidth()
{
	return m_halfBlockWidth;
}

void Level::Render()
{
	int topLeftBlockX = m_viewportX / m_blockWidth * m_blockWidth;
	int topLeftBlockY = m_viewportY / m_blockHeight * m_blockHeight;

	int bottomRightBlockX = (m_viewportX + m_windowWidth) / m_blockWidth * m_blockWidth;
	int bottomRightBlockY = (m_viewportY + m_windowHeight) / m_blockHeight * m_blockHeight;

	for (int row = topLeftBlockY; row <= bottomRightBlockY; row += m_blockHeight)
	{
		for (int column = topLeftBlockX; column <= bottomRightBlockX; column += m_blockWidth)
		{
			int rowIndex = row / m_blockHeight;
			int columnIndex = column / m_blockWidth;

			if (rowIndex < GetWorldHeightInBlocks() && columnIndex < GetWorldWidthInBlocks(rowIndex))
			{
				int xDrawPos = column - m_viewportX;
				int yDrawPos = row - m_viewportY;

				switch (arenaData[rowIndex][columnIndex])
				{
				case 'W':
				{
					if (m_usingBitmap)
					{
						m_pWallBitmap->Draw(xDrawPos, yDrawPos);
					}
					else
					{
						SDL_Rect destRect = { xDrawPos, yDrawPos, m_blockWidth, m_blockHeight };
						SDL_RenderCopy(m_pRenderer, m_pBlockTexture, NULL, &destRect);
					}
					break;
				}

				case 'P':
				{
					if (m_usingBitmap)
					{
						m_pPlatformBitmap->Draw(xDrawPos, yDrawPos);
					}
					else
					{
						SDL_Rect destRect = { xDrawPos, yDrawPos, m_blockWidth, m_blockHeight };
						SDL_RenderCopy(m_pRenderer, m_pPlatformTexture, NULL, &destRect);
					}
					break;
				}

				default:
					break;
				}
			}
		}
	}
}

bool Level::isWall(int xpos, int ypos)
{
	int x = xpos / m_blockWidth;
	int y = ypos / m_blockHeight;

	if (y < static_cast<int>(arenaData.size()))
	{
		if (x < static_cast<int>(arenaData[y].size()))
		{
			if (arenaData[y][x] == 'W')
			{
				return true;
			}
		}
	}

	return false;
}

bool Level::isPlatform(int xpos, int ypos)
{
	int x = xpos / m_blockWidth;
	int y = ypos / m_blockHeight;

	if (y < static_cast<int>(arenaData.size()))
	{
		if (x < static_cast<int>(arenaData[y].size()))
		{
			if (arenaData[y][x] == 'P')
			{
				return true;
			}
		}
	}

	return false;
}

float Level::GetGravityScale()
{
	return m_gravityScale;
}

void Level::SetGravityScale(float newGravityScale)
{
	m_gravityScale = newGravityScale;
}

int Level::GetViewportX()
{
	return m_viewportX;;
}

int Level::GetViewportY()
{
	return m_viewportY;
}

int Level::GetViewportWidth()
{
	return m_windowWidth;
}

int Level::GetViewportHeight()
{
	return m_windowHeight;
}

void Level::SetViewportX(int newX)
{
	int difference = newX - m_viewportX;
	int increment = -1;
	if (difference < 0)
	{
		increment = 1;
	}

	int i = newX;
	while (m_viewportX != i)
	{
		if (i >= 0 && i + m_windowWidth <= GetWorldWidthInBlocks(0) * m_blockWidth)
		{
			m_viewportX = i;
		}
		else
		{
			i += increment;
		}
	}
}

void Level::SetViewportY(int newY)
{
	int difference = newY - m_viewportY;
	int increment = -1;
	if (difference < 0)
	{
		increment = 1;
	}

	int i = newY;
	while (m_viewportY != i)
	{
		if (i >= 0 && i + m_windowHeight <= GetWorldHeightInBlocks() * m_blockHeight)
		{
			m_viewportY = i;
		}
		else
		{
			i += increment;
		}
	}
}

vector<GameObject*> Level::GetGameObjects()
{
	return m_pGameObjects;
}

string Level::GetLevelFileName()
{
	return m_levelFileName;
}
