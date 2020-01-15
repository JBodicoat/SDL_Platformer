#pragma once
#include "GameObject.h"

class Tree : public GameObject
{
private:
	Bitmap* m_pTreeBase{ nullptr };
	Bitmap* m_pTreeMid{ nullptr };
	Bitmap* m_pTreeTop{ nullptr };
	int m_numOfSegments{ 3 };
	bool m_isAlive{ true };

public:
	Tree(
		string name,
		Vector2 position,
		string treeBaseFile,
		string treeMidFile,
		string treeTopFile,
		SDL_Renderer* p_renderer,
		Level* p_arena = nullptr,
		Collider* p_collider = nullptr);
	~Tree();

	void Interact(ElementType playerElement);

	void Update();
	void Draw();
	void IterateFrame();
	int GetWidth();
	int GetHeight();
	bool GetIsAlive();
};

