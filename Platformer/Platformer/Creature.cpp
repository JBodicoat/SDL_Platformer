#include "Creature.h"
#include "Bitmap.h"
#include "SDL.h"
#include "SDL_render.h"
#include "BoxCollider.h"
#include "Tree.h"
#include <string>
#include <iostream>

using namespace std;

const int MILLISECS_PER_RUN_FRAME = 50;
const int MILLISECS_PER_IDLE_FRAME = 300;

Creature::Creature(string name,
	Vector2 position,
	ElementType elementType, 
	float speed,
	float maxHealth,
	Level* p_arena,
	Collider* p_collider)
	: GameObject::GameObject(name, position, elementType, p_arena, p_collider), m_maxRunSpeed{ speed }, m_maxHealth{ maxHealth }
{
	m_animationTimer = SDL_AddTimer(MILLISECS_PER_IDLE_FRAME, &GameObject::AnimationTimerCallback, this);
}

Creature::~Creature()
{
	while (!m_pIdleBitmaps.empty())
	{
		delete m_pIdleBitmaps.back();
		m_pIdleBitmaps.pop_back();
	}

	while (!m_pRunBitmaps.empty())
	{
		delete m_pRunBitmaps.back();
		m_pRunBitmaps.pop_back();
	}
}

void Creature::Run(vector<Tree*> p_trees)
{
	bool goingRight = true;
	if (m_xVelocity < 0)
		goingRight = false;

	int speed = static_cast<int>(m_xVelocity);
	if (speed < 0)
		speed *= -1;

	int toMove = 0;
	bool canMove = true;

	BoxCollider* p_collider = static_cast<BoxCollider*>(m_pCollider);
	if (goingRight)
	{
		for (int i = 1; i <= speed && canMove; i++)
		{
			// Test right column of pixels
			for (int y = static_cast<int>(p_collider->GetPosition().y); y < p_collider->GetPosition().y + p_collider->GetHeight() && canMove; y++)
			{
				if (m_pLevel->isWall(static_cast<int>(p_collider->GetPosition().x + p_collider->GetWidth()) - 1 + i, y))
				{
					canMove = false;
					break;
				}

				if (canMove)
				{
					for (Tree* p_tree : p_trees)
					{
						if (p_tree->GetCollider()->CollidesWith({ p_collider->GetPosition().x + p_collider->GetWidth() - 1 + i, static_cast<float>(y) }))
						{
							canMove = false;
							break;
						}
					}
				}
			}

			if (canMove)
				toMove = i;
		}

		if (toMove > 0)
		{
			m_position.x += toMove;
			m_flipH = false;
		}
	}
	else
	{
		for (int i = 1; i <= speed && canMove; i++)
		{
			// Test left column of pixels
			for (int y = static_cast<int>(p_collider->GetPosition().y); y < p_collider->GetPosition().y + p_collider->GetHeight() && canMove; y++)
			{
				if (m_pLevel->isWall(static_cast<int>(p_collider->GetPosition().x) - i, y))
				{
					canMove = false;
					break;
				}

				if (canMove)
				{
					for (Tree* p_tree : p_trees)
					{
						if (p_tree->GetCollider()->CollidesWith({ p_collider->GetPosition().x - i, static_cast<float>(y) }))
						{
							canMove = false;
							break;
						}
					}
				}
			}

			if (canMove)
				toMove = i;
		}

		if (toMove > 0)
		{
			m_position.x -= toMove;
			m_flipH = true;
		}
	}
}

void Creature::Left()
{
	m_forcesToApply.push({ -m_runAcceleration, 0.0f });
}

void Creature::Right()
{
	m_forcesToApply.push({ m_runAcceleration, 0.0f });
}

void Creature::Draw()
{
	int viewportX = m_pLevel->GetViewportX();
	int viewportY = m_pLevel->GetViewportY();

	switch (m_currentAnimation)
	{
	case CreatureAnimations::RUN:
		if (m_position.x + m_pRunBitmaps[m_current_frame]->GetWidth() - 1 >= viewportX && m_position.x <= viewportX + m_pLevel->GetViewportWidth() - 1)
		{
			if (m_position.y + m_pRunBitmaps[m_current_frame]->GetHeight() - 1 >= viewportY && m_position.y <= viewportY + m_pLevel->GetViewportHeight() - 1)
			{
				Vector2 screenPos = { m_position.x - viewportX, m_position.y - viewportY };
				if (static_cast<int>(m_pRunBitmaps.size()) > m_current_frame)
				{
					if (m_flipH)
					{
						m_pRunBitmaps[m_current_frame]->hFlip = true;
					}
					else
					{
						m_pRunBitmaps[m_current_frame]->hFlip = false;
					}
					m_pRunBitmaps[m_current_frame]->Draw(static_cast<int>(screenPos.x), static_cast<int>(screenPos.y));
				}
			}
		}
		break;

	case CreatureAnimations::IDLE:
		if (m_position.x + m_pIdleBitmaps[m_current_frame]->GetWidth() - 1 >= viewportX && m_position.x <= viewportX + m_pLevel->GetViewportWidth() - 1)
		{
			if (m_position.y + m_pIdleBitmaps[m_current_frame]->GetHeight() - 1 >= viewportY && m_position.y <= viewportY + m_pLevel->GetViewportHeight() - 1)
			{
				Vector2 screenPos = { m_position.x - viewportX, m_position.y - viewportY };
				if (static_cast<int>(m_pIdleBitmaps.size()) > m_current_frame)
				{
					if (m_flipH)
					{
						m_pIdleBitmaps[m_current_frame]->hFlip = true;
					}
					else
					{
						m_pIdleBitmaps[m_current_frame]->hFlip = false;
					}
					m_pIdleBitmaps[m_current_frame]->Draw(static_cast<int>(screenPos.x), static_cast<int>(screenPos.y));
				}
			}
		}
		break;
	}
}

int Creature::GetWidth()
{
	switch (m_currentAnimation)
	{
	case CreatureAnimations::IDLE:
		return m_pIdleBitmaps[m_current_frame]->GetWidth();
		break;

	case CreatureAnimations::RUN:
		return m_pRunBitmaps[m_current_frame]->GetWidth();
		break;
	}

	return 0;
}

int Creature::GetHeight()
{
	switch (m_currentAnimation)
	{
	case CreatureAnimations::IDLE:
		return m_pIdleBitmaps[m_current_frame]->GetHeight();
		break;

	case CreatureAnimations::RUN:
		return m_pRunBitmaps[m_current_frame]->GetHeight();
		break;
	}

	return 0;
}

void Creature::Jump(vector<Tree*> p_trees)
{
	BoxCollider* p_collider = static_cast<BoxCollider*>(m_pCollider);

	bool canMove = true;
	int yToCheck = static_cast<int>(p_collider->GetPosition().y + p_collider->GetHeight());
	// Test bottom row of pixels
	for (int x = static_cast<int>(p_collider->GetPosition().x); x < p_collider->GetPosition().x + p_collider->GetWidth() && canMove; x++)
	{
		if (m_pLevel->isWall(x, yToCheck) || (m_pLevel->isPlatform(x, yToCheck) && yToCheck % m_pLevel->GetBlockHeight() == 0))
		{
			canMove = false;
			break;
		}

		if(canMove)
		{
			for (Tree* p_tree : p_trees)
			{
				if (p_tree->GetCollider()->CollidesWith({ static_cast<float>(x), static_cast<float>(yToCheck) }))
				{
					canMove = false;
					break;
				}
			}
		}
	}

	if (!canMove)
	{
		m_forcesToApply.push({ 0.0f, m_jumpForce });
	}
}

void Creature::MoveVertically(vector<Tree*> p_trees)
{
	BoxCollider* p_collider = static_cast<BoxCollider*>(m_pCollider);

	if (m_yVelocity < 0)
	{
		// Go up
		int toMove = 0;
		bool canMove = true;
		int speed = static_cast<int>(-m_yVelocity);

		for (int i = 1; i <= speed && canMove; i++)
		{
			// Test top edge of collider
			for (int x = static_cast<int>(p_collider->GetPosition().x); x < p_collider->GetPosition().x + p_collider->GetWidth() && canMove; x++)
			{
				if (m_pLevel->isWall(x, static_cast<int>(m_position.y) - i))
				{
					canMove = false;
					break;
				}

				if (canMove)
				{
					for (Tree* p_tree : p_trees)
					{
						if (p_tree->GetCollider()->CollidesWith({ static_cast<float>(x), m_position.y - i }))
						{
							canMove = false;
							break;
						}
					}
				}
			}

			if (canMove)
				toMove = i;
		}

		if (toMove > 0)
		{
			m_position.y -= toMove;
		}
		else if (speed < -m_minimumBounceVelocity)
		{
			m_yVelocity = speed * -1 * m_bounceScale;
		}
		else
		{
			m_yVelocity = 0;
		}
	}
	else
	{
		// Go down
		int toMove = 0;
		bool canMove = true;
		int speed = static_cast<int>(m_yVelocity);

		for (int i = 1; i <= speed && canMove; i++)
		{
			int yToCheck = static_cast<int>(p_collider->GetPosition().y + p_collider->GetHeight()) - 1 + i;
			// Test bottom edge of collider
			for (int x = static_cast<int>(p_collider->GetPosition().x); x < p_collider->GetPosition().x + p_collider->GetWidth() && canMove; x++)
			{
				if (m_pLevel->isWall(x, yToCheck) || (m_pLevel->isPlatform(x, yToCheck) && yToCheck % m_pLevel->GetBlockHeight() == 0))
				{
					canMove = false;
					break;
				}

				if (canMove)
				{
					for (Tree* p_tree : p_trees)
					{
						if (p_tree->GetCollider()->CollidesWith({ static_cast<float>(x), static_cast<float>(yToCheck) }))
						{
							canMove = false;
							break;
						}
					}
				}
			}

			if (canMove)
				toMove = i;
		}

		if (toMove > 0)
		{
			m_position.y += toMove;
		}
		else if (speed > m_minimumBounceVelocity)
		{
			m_yVelocity = speed * -1 * m_bounceScale;
		}
		else
		{
			int yToCheck = static_cast<int>(p_collider->GetPosition().y + p_collider->GetHeight());
			// Test bottom edge of collider
			for (int x = static_cast<int>(p_collider->GetPosition().x); x < p_collider->GetPosition().x + p_collider->GetWidth() && canMove; x++)
			{
				if (m_pLevel->isWall(x, yToCheck) || (m_pLevel->isPlatform(x, yToCheck) && yToCheck % m_pLevel->GetBlockHeight() == 0))
				{
					canMove = false;
					break;
				}

				if (canMove)
				{
					for (Tree* p_tree : p_trees)
					{
						if (p_tree->GetCollider()->CollidesWith({ static_cast<float>(x), static_cast<float>(yToCheck) }))
						{
							canMove = false;
							break;
						}
					}
				}
			}

			if (!canMove)
				m_yVelocity = 0;
		}
	}
}

void Creature::Update(vector<Tree*> p_trees)
{
	m_oldPosition.x = m_position.x;
	m_oldPosition.y = m_position.y;
	m_oldXVelocity = m_xVelocity;

	m_forcesToApply.push({ 0.0f, m_pLevel->GetGravityScale() }); // Apply gravity

	Vector2 resultantForce{ 0.0f, 0.0f};
	while (!m_forcesToApply.empty())
	{
		resultantForce += m_forcesToApply.top();
		m_forcesToApply.pop(); ;
	}

	// Apply resistance
	resultantForce.x += (-m_xVelocity / m_maxRunSpeed) * m_runAcceleration;

	m_xVelocity += resultantForce.x;
    m_yVelocity += resultantForce.y;

	Run(p_trees);
	MoveVertically(p_trees);
	m_pCollider->SetPosition(m_position);

	int xSpeed = static_cast<int>(m_xVelocity);
	if (xSpeed < 0)
		xSpeed *= -1;

	int oldXSpeed = static_cast<int>(m_oldXVelocity);
	if (oldXSpeed < 0)
		oldXSpeed *= -1;

	if (xSpeed >= 0.2 && oldXSpeed < 0.2)
	{
		m_currentAnimation = CreatureAnimations::RUN;
		m_current_frame = 0;
		SDL_RemoveTimer(m_animationTimer);
		m_animationTimer = SDL_AddTimer(MILLISECS_PER_RUN_FRAME, &GameObject::AnimationTimerCallback, this);
	}
	else if (xSpeed < 0.2 && oldXSpeed >= 0.2)
	{
		m_currentAnimation =CreatureAnimations::IDLE;
		m_current_frame = 0;
		SDL_RemoveTimer(m_animationTimer);
		m_animationTimer = SDL_AddTimer(MILLISECS_PER_IDLE_FRAME, &GameObject::AnimationTimerCallback, this);
	}

	CheckEvents();
}

void Creature::RevertPosition()
{
	m_position = m_oldPosition;
	m_yVelocity = 0;
	m_xVelocity = 0;
	m_pCollider->SetPosition({m_oldPosition.x, m_oldPosition.y});
}

void Creature::IterateFrame()
{
	switch (m_currentAnimation)
	{
	case CreatureAnimations::RUN:
		if (++m_current_frame >= static_cast<int>(m_pRunBitmaps.size()))
		{
			m_current_frame = 0;
		}
		break;

	case CreatureAnimations::IDLE:
		if (++m_current_frame >= static_cast<int>(m_pIdleBitmaps.size()))
		{
			m_current_frame = 0;
		}
		break;
	}
}

float Creature::GetHealth()
{
	return m_currentHealth;
}

void Creature::IncrementHealth(float incrementAmount)
{
	m_currentHealth += incrementAmount;
	if (m_currentHealth > m_maxHealth)
	{
		m_currentHealth = m_maxHealth;
	}
}

void Creature::DecrementHealth(float decrementAmount)
{
	m_currentHealth -= decrementAmount;
}

void Creature::SetXVelocity(float newXVelocity)
{
	m_xVelocity = newXVelocity;
}

void Creature::SetYVelocity(float newYVelocity)
{
	m_yVelocity = newYVelocity;
}

void Creature::AddFrame(string fileName, SDL_Renderer* renderer, CreatureAnimations type)
{
	Bitmap* new_bitmap = new Bitmap{ renderer, fileName, true };
	switch (type)
	{
	case CreatureAnimations::RUN:
		m_pRunBitmaps.push_back(new_bitmap);
		break;

	case CreatureAnimations::IDLE:
		m_pIdleBitmaps.push_back(new_bitmap);
		break;
	}
}