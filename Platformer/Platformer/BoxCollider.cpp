#include "BoxCollider.h"
#include "CircleCollider.h"

#include <cmath>

BoxCollider::BoxCollider(Vector2 position, float width, float height)
	:Collider::Collider(position, ColliderType::BOX), m_width{ width }, m_height{ height }
{

}

BoxCollider::~BoxCollider()
{
}

float BoxCollider::GetWidth()
{
	return m_width;
}

void BoxCollider::SetWidth(float newWidth)
{
	m_width = newWidth;
}

float BoxCollider::GetHeight()
{
	return m_height;
}

void BoxCollider::SetHeight(float newHeight)
{
	m_height = newHeight;
}

bool BoxCollider::CollidesWith(Collider* p_otherCollider)
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

				float m_right_x = m_position.x + m_width;
				float m_bottom_y = m_position.y + m_height;

				float otherCollider_x = p_otherBoxCollider->GetPosition().x;
				float otherCollider_y = p_otherBoxCollider->GetPosition().y;
				float otherColliderRight_x = otherCollider_x + p_otherBoxCollider->GetWidth();
				float otherColliderBottom_y = otherCollider_y + p_otherBoxCollider->GetHeight();

				if (otherCollider_x >= m_position.x && otherCollider_x <= m_right_x)
				{
					// Top left
					if (otherCollider_y >= m_position.y && otherCollider_y <= m_bottom_y)
					{
						return true;
					}

					// Bottom left
					if (otherColliderBottom_y >= m_position.y && otherColliderBottom_y <= m_bottom_y)
					{
						return true;
					}
				}

				if (otherColliderRight_x >= m_position.x && otherColliderRight_x <= m_right_x)
				{
					// Top right
					if (otherCollider_y >= m_position.y && otherCollider_y <= m_bottom_y)
					{
						return true;
					}

					// Bottom right
					if (otherColliderBottom_y >= m_position.y && otherColliderBottom_y <= m_bottom_y)
					{
						return true;
					}
				}

				if (m_position.x >= otherCollider_x && m_position.x <= otherColliderRight_x)
				{
					// Top left
					if (m_position.y >= otherCollider_y && m_position.y <= otherColliderBottom_y)
					{
						return true;
					}

					// Bottom left
					if (m_bottom_y >= otherCollider_y && m_bottom_y <= otherColliderBottom_y)
					{
						return true;
					}
				}

				if (m_right_x >= otherCollider_x && m_right_x <= otherColliderRight_x)
				{
					// Top right
					if (m_position.y >= otherCollider_y && m_position.y <= otherColliderBottom_y)
					{
						return true;
					}

					// Bottom right
					if (m_bottom_y >= otherCollider_y && m_bottom_y <= otherColliderBottom_y)
					{
						return true;
					}
				}

				return false;
			}

			case ColliderType::CIRCLE:
			{
				CircleCollider* p_otherCircleCollider = static_cast<CircleCollider*>(p_otherCollider);

				float halfBoxWidth = m_width / 2.0f;
				float halfBoxHeight = m_height / 2.0f;

				float distanceX = abs(p_otherCircleCollider->GetPosition().x - (m_position.x + halfBoxWidth));
				float distanceY = abs(p_otherCircleCollider->GetPosition().y - (m_position.y + halfBoxHeight));

				if (distanceX > (halfBoxWidth + p_otherCircleCollider->GetRadius()))
				{
					return false;
				}
				if (distanceY > (halfBoxHeight + p_otherCircleCollider->GetRadius()))
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

				float distanceTopLeftX = abs(p_otherCircleCollider->GetPosition().x - m_position.x);
				float distanceTopLeftY = abs(p_otherCircleCollider->GetPosition().y - m_position.y);

				if (distanceTopLeftX <= p_otherCircleCollider->GetRadius() && distanceTopLeftY <= p_otherCircleCollider->GetRadius())
				{
					return true;
				}

				float distanceTopRightX = abs(p_otherCircleCollider->GetPosition().x - (m_position.x + m_width));
				float distanceTopRightY = abs(p_otherCircleCollider->GetPosition().y - m_position.y);

				if (distanceTopRightX <= p_otherCircleCollider->GetRadius() && distanceTopRightY <= p_otherCircleCollider->GetRadius())
				{
					return true;
				}

				float distanceBottomLeftX = abs(p_otherCircleCollider->GetPosition().x - m_position.x);
				float distanceBottomLeftY = abs(p_otherCircleCollider->GetPosition().y - (m_position.y + m_height));

				if (distanceBottomLeftX <= p_otherCircleCollider->GetRadius() && distanceBottomLeftY <= p_otherCircleCollider->GetRadius())
				{
					return true;
				}

				float distanceBottomRightX = abs(p_otherCircleCollider->GetPosition().x - (m_position.x + m_width));
				float distanceBottomRightY = abs(p_otherCircleCollider->GetPosition().y - (m_position.y + m_height));

				if (distanceBottomRightX <= p_otherCircleCollider->GetRadius() && distanceBottomRightY <= p_otherCircleCollider->GetRadius())
				{
					return true;
				}

				return false;
			}
		}
	}

	return false;
}

bool BoxCollider::CollidesWith(Vector2 point)
{
	if (point.x <= m_position.x + GetWidth() && point.x >= m_position.x)
	{
		if (point.y <= m_position.y + GetHeight() && point.y >= m_position.y)
		{
			return true;
		}
	}

	return false;
}
