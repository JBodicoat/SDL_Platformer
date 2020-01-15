#include "Tree.h"
#include "BoxCollider.h"

Tree::Tree(string name, Vector2 position, string treeBaseFile, string treeMidFile, string treeTopFile, SDL_Renderer* p_renderer, Level* p_arena, Collider* p_collider)
	:GameObject::GameObject(name, position, ElementType::NONE, p_arena, p_collider)
{
	
	m_pTreeBase = new Bitmap(p_renderer, treeBaseFile, true);
	m_pTreeMid = new Bitmap(p_renderer, treeMidFile, true);
	m_pTreeTop = new Bitmap(p_renderer, treeTopFile, true);

	m_position.y -= (m_pTreeBase->GetHeight() - 1) * (m_numOfSegments - 1);

	m_pCollider = new BoxCollider(
		{ m_position.x, m_position.y},
		static_cast<float>(m_pTreeBase->GetWidth()),
		static_cast<float>(GetHeight()));
}

Tree::~Tree()
{
	delete m_pTreeBase;
	delete m_pTreeMid;
	delete m_pTreeTop;
}

void Tree::Interact(ElementType playerElement)
{
	switch (playerElement)
	{
	case ElementType::WATER:
	{
		// Grow
		++m_numOfSegments;
		m_position.y -= m_pTreeBase->GetHeight();
		BoxCollider* p_collider = static_cast<BoxCollider*>(m_pCollider);
		p_collider->SetPosition({ m_position.x, m_position.y });
		p_collider->SetHeight(static_cast<float>(GetHeight()));
		break;
	}

	case ElementType::FIRE:
		// Burn down
		m_isAlive = false;
		break;

	default:
		// Nothing
		break;
	}
}

void Tree::Update()
{
	//Do nothing
}

void Tree::Draw()
{
	int viewportX = m_pLevel->GetViewportX();
	int viewportY = m_pLevel->GetViewportY();

	//Top
	{
		Vector2 screenPos = { m_position.x - viewportX, m_position.y - viewportY };
		m_pTreeTop->Draw(static_cast<int>(screenPos.x), static_cast<int>(screenPos.y));
	}

	//Mid
	int i = 1;
	for (i; i < m_numOfSegments - 1; i++)
	{
		Vector2 screenPos = { m_position.x - viewportX, m_position.y + m_pTreeMid->GetHeight() * i - viewportY };
		m_pTreeMid->Draw(static_cast<int>(screenPos.x), static_cast<int>(screenPos.y));
	}

	//Base
	m_pTreeBase->Draw(static_cast<int>(m_position.x) - viewportX, static_cast<int>(m_position.y) + m_pTreeMid->GetHeight() * i - viewportY);
}

void Tree::IterateFrame()
{
	// Do nothing
}

int Tree::GetWidth()
{
	return m_pTreeBase->GetWidth();
}

int Tree::GetHeight()
{
	return m_pTreeBase->GetHeight() * m_numOfSegments;
}

bool Tree::GetIsAlive()
{
	return m_isAlive;
}
