#include "Monster.h"
#include "CanSee.h"

using namespace std;

Monster::Monster(
	string name, 
	Vector2 position, 
	ElementType elementType,
	float speed, 
	float maxHealth,
	Level* p_arena, 
	Collider* p_collider)
	:Creature::Creature{ name, position, elementType, speed, maxHealth, p_arena, p_collider }
{
	m_pEnrageCollider = new CircleCollider{ m_position, m_enrageRange };
}

void Monster::Chase(Hero* hero)
{
	Vector2 heroPos = hero->GetPosition();
	if (heroPos.x > m_position.x)
	{
		Right();
	}
	else if (heroPos.x < m_position.x)
	{
		Left();
	}
}

void Monster::Update(vector<Tree*> p_trees, Hero* p_hero)
{
	Creature::Update(p_trees);

	if (m_enraged)
	{
		Chase(p_hero);
	}
	else if(m_pEnrageCollider->CollidesWith(p_hero->GetCollider()))
	{
		// Test line of sight
		Vector2 originPos = m_position;
		originPos.x += static_cast<float>(GetWidth() / 2 - 0.5f);
		originPos.y += static_cast<float>(GetHeight() / 4 - 0.25f);

		Vector2 targetPos = p_hero->GetPosition();
		targetPos.x += static_cast<float>(p_hero->GetWidth() / 2 - 0.5f);
		targetPos.y += static_cast<float>(p_hero->GetHeight() / 2 - 0.5f);

		if (CanSee(originPos, targetPos, m_pLevel->GetHalfBlockWidth(), m_pLevel->GetHalfBlockHeight(), m_pLevel))
		{
			m_enraged = true;
			Chase(p_hero);
		}
	}
}

void Monster::Draw()
{
	Creature::Draw();
}

void Monster::Interact(ElementType playerElement)
{
	switch (playerElement)
	{
	case ElementType::WATER:
		if (m_elementType == ElementType::WATER)
		{
			// Heal
			IncrementHealth(8.0f);
		}
		else
		{
			// Get damaged
			DecrementHealth(8.0f);
		}
		break;

	case ElementType::FIRE:
		if (m_elementType == ElementType::FIRE)
		{
			// Heal
			IncrementHealth(8.0f);
		}
		else
		{
			// Get damaged
			DecrementHealth(8.0f);
		}
		break;

	default:
		// Nothing
		break;
	}
}
