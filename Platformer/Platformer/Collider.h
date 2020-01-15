#pragma once
#include "Vector2.h"

enum class ColliderType { BOX, CIRCLE, SIZE_OF_COLLIDER_TYPES };

class Collider
{
protected:
	Vector2 m_position{ 0.0f, 0.0f };
	const ColliderType m_colliderType;

public:
	Collider(Vector2 position, ColliderType	colliderType);
	~Collider();
	
	Vector2 GetPosition();
	void SetPosition(Vector2 newPosition);

	ColliderType GetColliderType();

	virtual bool CollidesWith(Collider* p_otherCollider) = 0;
	virtual bool CollidesWith(Vector2 point) = 0;
};

