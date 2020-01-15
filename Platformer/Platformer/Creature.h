#pragma once
#include <string>
#include <stack>
#include <queue>
#include "Bitmap.h"
#include "Level.h"
#include "Vector2.h"
#include "GameObject.h"
#include "Tree.h"

using namespace std;

class Bitmap;
class Level;

enum class CreatureEvents { CE_TIMER, NUM_CREATURE_EVENTS };
enum class CreatureAnimations {IDLE, RUN, SIZE_OF_ANIMATIONS_ENUM};

class Creature : public GameObject
{
protected:
	Vector2 m_oldPosition{ 0, 0 };

	stack<Vector2> m_forcesToApply;

	const float m_maxRunSpeed{ 1.0f };
	float m_xVelocity{ 0.0f };
	float m_runAcceleration{ 0.5f };
	float m_oldXVelocity{ 0.0f };

	const float m_jumpForce{ -10.0f };
	float m_yVelocity{ 0.0f };

	const float m_minimumBounceVelocity{ 10.0f };
	const float m_bounceScale{ 0.2f };

	vector<Bitmap*> m_pRunBitmaps;
	vector<Bitmap*> m_pIdleBitmaps;
	CreatureAnimations m_currentAnimation{ CreatureAnimations::IDLE };

	void Run(vector<Tree*> p_trees);
	void MoveVertically(vector<Tree*> p_trees);

	const float m_maxHealth{ 100 };
	float m_currentHealth{ m_maxHealth };

public:
	Creature(
		string name, 
		Vector2 position,
		ElementType elementType, 
		float speed,
		float maxHealth,
		Level* p_arena, 
		Collider* p_collider = nullptr);
	~Creature();

	int GetWidth();
	int GetHeight();

	void Update(vector<Tree*> p_trees);
	void Draw();

	void Left();
	void Right();
	void Jump(vector<Tree*> p_trees);

	void RevertPosition();

	void AddFrame(string fileName, SDL_Renderer* renderer, CreatureAnimations type);
	void IterateFrame();

	float GetHealth();
	void IncrementHealth(float incrementAmount);
	void DecrementHealth(float decrementAmount);

	void SetXVelocity(float newXVelocity);
	void SetYVelocity(float newYVelocity);
};
