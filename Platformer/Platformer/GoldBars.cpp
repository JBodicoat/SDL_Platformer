#include "GoldBar.h"
#include "SDL.h"

const int MILLISECS_PER_GOLD_FRAME = 250;

GoldBar::GoldBar(
	string name, 
	Vector2 position, 
	ElementType elementType, 
	Level* p_arena,
	Collider* p_collider)
	:GameObject::GameObject(name, position, elementType, p_arena, p_collider)
{
	m_animationTimer = SDL_AddTimer(MILLISECS_PER_GOLD_FRAME, &GameObject::AnimationTimerCallback, this);
}

GoldBar::~GoldBar()
{
	while (!m_pBitmaps.empty())
	{
		delete m_pBitmaps.back();
		m_pBitmaps.pop_back();
	}
}

int GoldBar::GetWidth()
{
	return m_pBitmaps[m_current_frame]->GetWidth();
}

int GoldBar::GetHeight()
{
	return m_pBitmaps[m_current_frame]->GetHeight();
}

void GoldBar::Update()
{
	CheckEvents();
}

void GoldBar::Draw()
{
	int viewportX = m_pLevel->GetViewportX();
	int viewportY = m_pLevel->GetViewportY();

	if (m_position.x + m_pBitmaps[m_current_frame]->GetWidth() - 1 >= viewportX && m_position.x <= viewportX + m_pLevel->GetViewportWidth() - 1)
	{
		if (m_position.y + m_pBitmaps[m_current_frame]->GetHeight() - 1 >= viewportY && m_position.y <= viewportY + m_pLevel->GetViewportHeight() - 1)
		{
			Vector2 screenPos = { m_position.x - viewportX, m_position.y - viewportY };
			if (static_cast<int>(m_pBitmaps.size()) > m_current_frame)
			{
				if (m_flipH)
				{
					m_pBitmaps[m_current_frame]->hFlip = true;
				}
				else
				{
					m_pBitmaps[m_current_frame]->hFlip = false;
				}
				m_pBitmaps[m_current_frame]->Draw(static_cast<int>(screenPos.x), static_cast<int>(screenPos.y));
			}
		}
	}
}

void GoldBar::AddFrame(string fileName, SDL_Renderer* renderer)
{
	m_pBitmaps.push_back(new Bitmap{ renderer, fileName, true });
}

void GoldBar::IterateFrame()
{
	if (++m_current_frame >= static_cast<int>(m_pBitmaps.size()))
	{
		m_current_frame = 0;
	}
}

void GoldBar::Interact(ElementType playerElement)
{
	// Do nothing
}
