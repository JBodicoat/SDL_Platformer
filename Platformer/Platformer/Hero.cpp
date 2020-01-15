#include <string>
#include "Hero.h"

using namespace std;

Hero::Hero(
	string name, 
	Vector2 position, 
	ElementType elementType, 
	float speed, 
	float maxHealth,
	Level* p_arena, 
	Collider* p_collider)
	:Creature::Creature{ name, position, elementType, speed, maxHealth, p_arena, p_collider }
{
	m_currentAnimation = CreatureAnimations::IDLE;
}

void Hero::HandleInput(Input* p_input, vector<Tree*> p_trees)
{
	if (p_input->KeyIsPressed(KEYS_PRESSED_LIST::KEY_LEFT))
	{
		Left();
	}

	if (p_input->KeyIsPressed(KEYS_PRESSED_LIST::KEY_RIGHT))
	{
		Right();
	}

	if (p_input->KeyIsPressed(KEYS_PRESSED_LIST::KEY_UP) || p_input->KeyIsPressed(KEYS_PRESSED_LIST::KEY_SPACE))
	{
		Jump(p_trees);
	}

	if (p_input->GetKeyDown(KEYS_PRESSED_LIST::KEY_E))
	{
		// Interact with anything in radius & line of sight
		SDL_Event event;
		SDL_UserEvent userEvent;

		//set up userEvent with default data
		userEvent.type = SDL_USEREVENT;
		userEvent.data1 = NULL;
		userEvent.data2 = NULL;

		//this is an INTERACT event
		userEvent.code = GE_INTERACT;

		//create the event
		event.type = SDL_USEREVENT;
		event.user = userEvent;

		//push the event
		SDL_PushEvent(&event);
	}

	if (p_input->GetKeyDown(KEYS_PRESSED_LIST::KEY_F))
	{
		// Swap elements
		m_elementType = (m_elementType == ElementType::FIRE) ? ElementType::WATER : ElementType::FIRE;
	}
}

void Hero::Update(vector<Tree*> p_trees)
{
	Creature::Update(p_trees);
}

void Hero::Draw()
{
	Creature::Draw();
}

void Hero::Push(Vector2 direction)
{
	m_forcesToApply.push(direction);
}

void Hero::Interact(ElementType playerElement)
{
	// Do nothing
}
