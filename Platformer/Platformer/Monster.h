#pragma once
#include "Creature.h"
#include "Hero.h"
#include "CircleCollider.h"
#include "Tree.h"

using namespace std;

class Monster : public Creature
{
protected:
	bool m_enraged{ false };
	float m_enrageRange{ 500.0f };
	CircleCollider* m_pEnrageCollider{ nullptr };

	void Chase(Hero* p_hero);

public:
	Monster(
		string name,
		Vector2 position, 
		ElementType elementType,
		float speed,
		float maxHealth,
		Level* p_arena, 
		Collider* p_collider = nullptr);

	void Update(vector<Tree*> p_trees, Hero* p_hero);
	void Draw();

	void Interact(ElementType playerElement);
};

