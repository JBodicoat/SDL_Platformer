#include "GameObject.h"
#include "Level.h"
#include <string>

using namespace std;

GameObject::GameObject(
	string name, 
	Vector2 position,
	ElementType elementType,
	Level* p_arena,
	Collider* p_collider)
	:m_name{ name }, m_position{ position }, m_elementType{ elementType }, m_pLevel{ p_arena }, m_pCollider{ p_collider }
{
	++numOfGameObjects;
}

GameObject::~GameObject()
{
	SDL_RemoveTimer(m_animationTimer);
	delete m_pCollider;
}

Vector2 GameObject::GetPosition()
{
	return m_position;
}

ElementType GameObject::GetElementType()
{
	return m_elementType;
}

Collider* GameObject::GetCollider()
{
	return m_pCollider;
}

void GameObject::SetCollider(Collider* p_collider)
{
	m_pCollider = p_collider;
}

void GameObject::MoveBy(Vector2 velocity)
{
	m_position += velocity;
}

void GameObject::MoveTo(Vector2 newPosition)
{
	m_position = newPosition;
}

void GameObject::Update()
{
	CheckEvents();
}

string GameObject::GetName()
{
	return m_name;
}

Uint32 GameObject::AnimationTimerCallback(Uint32 interval, void* gameObjectIn)
{
	if (gameObjectIn)
	{
		GameObject* gameObject = static_cast<GameObject*>(gameObjectIn);
		gameObject->CreateTimerEvent();
	}

	return interval;
}

void GameObject::CreateTimerEvent()
{
	SDL_Event event;
	SDL_UserEvent userEvent;

	//set up userEvent with default data
	userEvent.type = SDL_USEREVENT;
	userEvent.data1 = this;
	userEvent.data2 = NULL;

	//this is a TIMER event
	userEvent.code = GE_ANIMATION_TIMER;

	//create the event
	event.type = SDL_USEREVENT;
	event.user = userEvent;

	//push the event
	SDL_PushEvent(&event);
}


void GameObject::CheckEvents()
{
	//poll the event queue
	while (!m_events.empty())
	{
		switch (m_events.front().type)
		{
			case SDL_USEREVENT:
			{
				if (m_events.front().user.code == GE_ANIMATION_TIMER)
				{
					IterateFrame();
				}
				break;
			}
		}

		m_events.pop();
	}
}

void GameObject::PushEvent(SDL_Event event)
{
	m_events.push(event);
}
