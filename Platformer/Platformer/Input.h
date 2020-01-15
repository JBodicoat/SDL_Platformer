#pragma once
#include "SDL.h"
#include <queue>

using namespace std;

enum class KEYS_PRESSED_LIST
{
	KEY_ESCAPE, KEY_E, KEY_F, KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_DOWN, KEY_SPACE, SIZE_OF_KEYS_PRESSED_ENUM
};

enum class BUTTONS_PRESSED_LIST
{
	LMB, RMB, SIZE_OF_BUTTONS_PRESSED_ENUM
};

class Input
{
public:
	void Update(void);
	bool KeyIsPressed(KEYS_PRESSED_LIST key);
	bool GetKeyDown(KEYS_PRESSED_LIST key);
	bool GetKeyUp(KEYS_PRESSED_LIST key);
	bool ButtonIsPressed(BUTTONS_PRESSED_LIST button);
	int GetMouseX(void);
	int GetMouseY(void);
	void PushEvent(SDL_Event event);

	Input();
	~Input();

private:
	queue<SDL_Event> m_events;
	bool m_keysPressed[static_cast<unsigned>(KEYS_PRESSED_LIST::SIZE_OF_KEYS_PRESSED_ENUM)];
	bool m_keysPressedLast[static_cast<unsigned>(KEYS_PRESSED_LIST::SIZE_OF_KEYS_PRESSED_ENUM)];
	bool m_buttonsPressed[static_cast<unsigned>(BUTTONS_PRESSED_LIST::SIZE_OF_BUTTONS_PRESSED_ENUM)];
	bool m_buttonsPressedLast[static_cast<unsigned>(BUTTONS_PRESSED_LIST::SIZE_OF_BUTTONS_PRESSED_ENUM)];
	int m_mouseX;
	int m_mouseY;

	bool KeyWasPressed(KEYS_PRESSED_LIST key);
	bool ButtonWasPressed(BUTTONS_PRESSED_LIST button);
};

