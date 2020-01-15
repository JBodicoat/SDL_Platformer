#include "Collider.h"

Collider::Collider(Vector2 position, ColliderType colliderType)
	:m_position{ position }, m_colliderType{ colliderType }
{
}

Collider::~Collider()
{
}

Vector2 Collider::GetPosition()
{
	return m_position;
}

void Collider::SetPosition(Vector2 newPosition)
{
	m_position = newPosition;
}

ColliderType Collider::GetColliderType()
{
	return m_colliderType;
}
