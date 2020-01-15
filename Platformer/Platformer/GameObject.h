#pragma once
#include "Vector2.h"
#include "Collider.h"
#include "Enums.h"
#include "Bitmap.h"
#include "Level.h"
#include <queue>
#include <string>

enum class ElementType { NONE, FIRE, WATER, SIZE_OF_ELEMENT_TYPES };

static int numOfGameObjects = 0;

using namespace std;

class Level;

class GameObject
{
protected:
	const string m_name{ "" };
	Vector2 m_position{ 0, 0 };
	Collider* m_pCollider{ nullptr };
	ElementType m_elementType{ ElementType::NONE };
	bool m_flipH{ false };
	const int m_ID{ numOfGameObjects };
	Level* m_pLevel{ nullptr };

	int m_current_frame{ 0 };
	SDL_TimerID m_animationTimer{ NULL };
	queue<SDL_Event> m_events;

public:
	GameObject(
		string name,
		Vector2 position,
		ElementType elementType,
		Level* p_arena = nullptr,
		Collider* p_collider = nullptr
	);
	virtual ~GameObject();

	Vector2 GetPosition();
	ElementType GetElementType();
	Collider* GetCollider();

	void SetCollider(Collider* p_collider);

	string GetName();

	void MoveBy(Vector2 velocity);
	void MoveTo(Vector2 newPosition);

	virtual void Update();
	virtual void Draw() = 0;
	virtual int GetWidth() = 0;
	virtual int GetHeight() = 0;

	// Animation
	virtual void IterateFrame() = 0;
	static Uint32 AnimationTimerCallback(Uint32 interval, void* gameObjectIn);
	void CreateTimerEvent();
	void CheckEvents();
	void PushEvent(SDL_Event event);

	virtual void Interact(ElementType playerElement) = 0;
};
