#include "Mushroom.h"

Mushroom::Mushroom(string name, Vector2 position, ElementType elementType, float speed, float maxHealth, Level* p_arena, Collider* p_collider)
	:Creature::Creature{name, position, elementType, speed, maxHealth, p_arena, p_collider}
{
	m_runAcceleration = m_maxRunSpeed;
	Right();
}

void Mushroom::SetCollidingWithPlayer(bool newBool)
{
	m_collidingWithPlayer = newBool;
}

bool Mushroom::isCollingWithPlayer()
{
	return m_collidingWithPlayer;
}

void Mushroom::Update(vector<Tree*> p_trees)
{
	Creature::Update(p_trees);

	if (m_position.x == m_oldPosition.x)
	{
		m_goingRight = !m_goingRight;
	}

	if (m_goingRight)
	{
		Right();
	}
	else
	{
		Left();
	}
}

void Mushroom::Draw()
{
	Creature::Draw();
}

void Mushroom::Interact(ElementType playerElement)
{
	// Do nothing
}
