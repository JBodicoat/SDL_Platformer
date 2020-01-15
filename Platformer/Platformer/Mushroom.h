#pragma once
#include <string>
#include "Creature.h"
#include "Tree.h"

using namespace std;

class Mushroom : public Creature
{
private:
	bool m_goingRight{ true };
	bool m_collidingWithPlayer{ false };

public:
	Mushroom(
		string name,
		Vector2 position,
		ElementType elementType,
		float speed,
		float maxHealth,
		Level* p_arena,
		Collider* p_collider = nullptr);

	void SetCollidingWithPlayer(bool newBool);
	bool isCollingWithPlayer();

	void Update(vector<Tree*> p_trees);
	void Draw();

	void Interact(ElementType playerElement);

};

