#pragma once
#include "GameObject.h"

class Fire : public GameObject
{
private:
	Bitmap* m_pBitmap{ nullptr };
	bool m_isAlive{ true };

public:
	Fire(
		string name,
		Vector2 position,
		string bitmapFile,
		SDL_Renderer* p_renderer,
		Level* p_arena = nullptr,
		Collider* p_collider = nullptr);
	~Fire();

	void Interact(ElementType playerElement);
	void IterateFrame();
	void Update();
	void Draw();
	int GetWidth();
	int GetHeight();
	bool GetIsAlive();
};

