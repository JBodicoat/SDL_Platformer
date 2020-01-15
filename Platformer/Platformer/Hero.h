#pragma once
#include <string>
#include "Creature.h"
#include "Input.h"
#include "Tree.h"

using namespace std;

class Hero : public Creature
{
public:
	Hero(string name, 
		Vector2 position, 
		ElementType elementType, 
		float speed,
		float maxHealth,
		Level* p_arena, 
		Collider* p_collider = nullptr);
	void HandleInput(Input* p_input, vector<Tree*> p_trees);
	void Update(vector<Tree*> p_trees);
	void Draw();

	void Push(Vector2 direction);
	void Interact(ElementType playerElement);
};

