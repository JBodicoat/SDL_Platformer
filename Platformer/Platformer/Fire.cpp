#include "Fire.h"
#include "BoxCollider.h"

Fire::Fire(string name, Vector2 position, string bitmapFile, SDL_Renderer* p_renderer, Level* p_arena, Collider* p_collider)
	:GameObject::GameObject(name, position, ElementType::FIRE, p_arena, p_collider)
{
	m_pBitmap = new Bitmap(p_renderer, bitmapFile, true);

	m_pCollider = new BoxCollider{ m_position, static_cast<float>(m_pBitmap->GetWidth()), static_cast<float>(m_pBitmap->GetHeight()) };
}

Fire::~Fire()
{
	delete m_pBitmap;
}

void Fire::Interact(ElementType playerElement)
{
	switch (playerElement)
	{
	case ElementType::WATER:
		// Get put out
		m_isAlive = false;
		break;

	case ElementType::FIRE:
		// Nothing
		break;

	default:
		// Nothing
		break;
	}
}

void Fire::IterateFrame()
{
	// Do nothing
}

void Fire::Update()
{
	//Nothing
}

void Fire::Draw()
{
	int viewportX = m_pLevel->GetViewportX();
	int viewportY = m_pLevel->GetViewportY();

	if (m_position.x + m_pBitmap->GetWidth() - 1 >= viewportX && m_position.x <= viewportX + m_pLevel->GetViewportWidth() - 1)
	{
		if (m_position.y + m_pBitmap->GetHeight() - 1 >= viewportY && m_position.y <= viewportY + m_pLevel->GetViewportHeight() - 1)
		{
			Vector2 screenPos = { m_position.x - viewportX, m_position.y - viewportY };
			if (m_flipH)
			{
				m_pBitmap->hFlip = true;
			}
			else
			{
				m_pBitmap->hFlip = false;
			}
			m_pBitmap->Draw(static_cast<int>(screenPos.x), static_cast<int>(screenPos.y));
		}
	}
}

int Fire::GetWidth()
{
	return m_pBitmap->GetWidth();
}

int Fire::GetHeight()
{
	return m_pBitmap->GetHeight();
}

bool Fire::GetIsAlive()
{
	return m_isAlive;
}
