#pragma once
#include "Collider.h"

class BoxCollider : public Collider
{
private:
	float m_width{ 0.0f };
	float m_height{ 0.0f };

public:
	BoxCollider(Vector2 position, float width, float height);
	~BoxCollider();

	float GetWidth();
	void SetWidth(float newWidth);
	float GetHeight();
	void SetHeight(float newHeight);

	bool CollidesWith(Collider* p_otherCollider);
	bool CollidesWith(Vector2 point);
};

