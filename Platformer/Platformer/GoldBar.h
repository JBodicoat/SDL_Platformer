#pragma once
#include <vector>
#include "Bitmap.h"
#include "Hero.h"
#include "Vector2.h"

using namespace std;

class GoldBar : public GameObject
{
private:
	vector<Bitmap*> m_pBitmaps;

public:
	GoldBar(
		string name, 
		Vector2 position, 
		ElementType elementType,
		Level* p_arena, 
		Collider* p_collider = nullptr);
	~GoldBar();

	int GetWidth();
	int GetHeight();

	void Update();
	void Draw();
	void AddFrame(string fileName, SDL_Renderer* renderer);
	void IterateFrame();

	void Interact(ElementType playerElement);
};

