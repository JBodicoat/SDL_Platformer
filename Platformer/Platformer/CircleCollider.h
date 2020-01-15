#pragma once
#include "Collider.h"

class CircleCollider : public Collider
{
private:
	const float m_radius{ 0.0f };
	const float m_radisuSquared = m_radius * m_radius;

public:
	CircleCollider(Vector2 position, float radius);
	~CircleCollider();

	float GetRadius();

	bool CollidesWith(Collider* p_otherCollider);
	bool CollidesWith(Vector2 point);
};

