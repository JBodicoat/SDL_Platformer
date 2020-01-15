#include "CircleCollider.h"
#include "BoxCollider.h"

#include <cmath>

CircleCollider::CircleCollider(Vector2 position, float radius)
	:Collider::Collider(position, ColliderType::CIRCLE), m_radius{ radius }
{
}

CircleCollider::~CircleCollider()
{
}

float CircleCollider::GetRadius()
{
	return m_radius;
}

bool CircleCollider::CollidesWith(Collider* p_otherCollider)
{
	if (p_otherCollider)
	{
		if (this == p_otherCollider)
		{
			return false;
		}

		switch (p_otherCollider->GetColliderType())
		{
			case ColliderType::BOX:
			{
				BoxCollider* p_otherBoxCollider = static_cast<BoxCollider*>(p_otherCollider);

				float halfBoxWidth = p_otherBoxCollider->GetWidth() / 2.0f;
				float halfBoxHeight = p_otherBoxCollider->GetHeight() / 2.0f;

				float distanceX = abs(m_position.x - (p_otherBoxCollider->GetPosition().x + halfBoxWidth));
				float distanceY = abs(m_position.y - (p_otherBoxCollider->GetPosition().y + halfBoxHeight));

				if (distanceX > (halfBoxWidth + m_radius))
				{
					return false;
				}
				if (distanceY > (halfBoxHeight + m_radius))
				{
					return false;
				}

				if (distanceX <= halfBoxWidth)
				{
					return true;
				}
				if (distanceY <= halfBoxHeight)
				{
					return true;
				}

				float distanceTopLeftX = abs(p_otherBoxCollider->GetPosition().x - m_position.x);
				float distanceTopLeftY = abs(p_otherBoxCollider->GetPosition().y - m_position.y);

				if (distanceTopLeftX <= m_radius && distanceTopLeftY <= m_radius)
				{
					return true;
				}

				float distanceTopRightX = abs(p_otherBoxCollider->GetPosition().x + p_otherBoxCollider->GetWidth() -  m_position.x);
				float distanceTopRightY = abs(p_otherBoxCollider->GetPosition().y - m_position.y);

				if (distanceTopRightX <= m_radius && distanceTopRightY <= m_radius)
				{
					return true;
				}

				float distanceBottomLeftX = abs(p_otherBoxCollider->GetPosition().x - m_position.x);
				float distanceBottomLeftY = abs(p_otherBoxCollider->GetPosition().y + p_otherBoxCollider->GetHeight() - m_position.y);

				if (distanceBottomLeftX <= m_radius && distanceBottomLeftY <= m_radius)
				{
					return true;
				}

				float distanceBottomRightX = abs(p_otherBoxCollider->GetPosition().x + p_otherBoxCollider->GetWidth() - m_position.x);
				float distanceBottomRightY = abs(p_otherBoxCollider->GetPosition().y + p_otherBoxCollider->GetHeight() - m_position.y);

				if (distanceBottomRightX <= m_radius && distanceBottomRightY <= m_radius)
				{
					return true;
				}

			}

			case ColliderType::CIRCLE:
			{
				CircleCollider* p_otherCircleCollider = static_cast<CircleCollider*>(p_otherCollider);

				float distanceSquared = pow(m_position.x - p_otherCircleCollider->GetPosition().x, 2) + pow(m_position.y - p_otherCircleCollider->GetPosition().y, 2);

				if (distanceSquared <= pow(m_radius + m_radius, 2))
				{
					return true;
				}

				return false;
			}
		}
	}

	return false;
}

bool CircleCollider::CollidesWith(Vector2 point)
{
	Vector2 differrenceVec = point - m_position;
	float differenceMagSquared = differrenceVec.x * differrenceVec.x + differrenceVec.y * differrenceVec.y;

	if (differenceMagSquared <= m_radisuSquared)
	{
		return true;
	}

	return false;
}
