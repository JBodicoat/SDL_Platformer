#include "Input.h"
#include "Game.h"

Input::Input()
	:m_keysPressed{ NULL }, m_keysPressedLast{ NULL }
{
}

Input::~Input()
{
}


void Input::Update(void)
{
	for (int i = 0; i < sizeof(m_keysPressedLast); i++)
	{
		m_keysPressedLast[i] = m_keysPressed[i];
	}

	for (int i = 0; i < sizeof(m_buttonsPressedLast); i++)
	{
		m_buttonsPressedLast[i] = m_buttonsPressed[i];
	}

	m_mouseX = 0;
	m_mouseY = 0;

	while (!m_events.empty())
	{
		switch (m_events.front().type)
		{
			case SDL_KEYDOWN:
			{
				SDL_Keycode keyPressed = m_events.front().key.keysym.sym;

				switch (keyPressed)
				{
				case SDLK_ESCAPE:
					m_keysPressed[static_cast<unsigned>(KEYS_PRESSED_LIST::KEY_ESCAPE)] = true;
					break;

				case SDLK_e:
					m_keysPressed[static_cast<unsigned>(KEYS_PRESSED_LIST::KEY_E)] = true;
					break;

				case SDLK_f:
					m_keysPressed[static_cast<unsigned>(KEYS_PRESSED_LIST::KEY_F)] = true;
					break;

				case SDLK_LEFT:
					m_keysPressed[static_cast<unsigned>(KEYS_PRESSED_LIST::KEY_LEFT)] = true;
					break;

				case SDLK_RIGHT:
					m_keysPressed[static_cast<unsigned>(KEYS_PRESSED_LIST::KEY_RIGHT)] = true;
					break;

				case SDLK_UP:
					m_keysPressed[static_cast<unsigned>(KEYS_PRESSED_LIST::KEY_UP)] = true;
					break;

				case SDLK_DOWN:
					m_keysPressed[static_cast<unsigned>(KEYS_PRESSED_LIST::KEY_DOWN)] = true;
					break;

				case SDLK_SPACE:
					m_keysPressed[static_cast<unsigned>(KEYS_PRESSED_LIST::KEY_SPACE)] = true;
					break;
				}
			}
			break;

			case SDL_KEYUP:
			{
				SDL_Keycode keyPressed = m_events.front().key.keysym.sym;

				switch (keyPressed)
				{
				case SDLK_ESCAPE:
					m_keysPressed[static_cast<unsigned>(KEYS_PRESSED_LIST::KEY_ESCAPE)] = false;
					break;

				case SDLK_e:
					m_keysPressed[static_cast<unsigned>(KEYS_PRESSED_LIST::KEY_E)] = false;
					break;

				case SDLK_f:
					m_keysPressed[static_cast<unsigned>(KEYS_PRESSED_LIST::KEY_F)] = false;
					break;

				case SDLK_LEFT:
					m_keysPressed[static_cast<unsigned>(KEYS_PRESSED_LIST::KEY_LEFT)] = false;
					break;

				case SDLK_RIGHT:
					m_keysPressed[static_cast<unsigned>(KEYS_PRESSED_LIST::KEY_RIGHT)] = false;
					break;

				case SDLK_UP:
					m_keysPressed[static_cast<unsigned>(KEYS_PRESSED_LIST::KEY_UP)] = false;
					break;

				case SDLK_DOWN:
					m_keysPressed[static_cast<unsigned>(KEYS_PRESSED_LIST::KEY_DOWN)] = false;
					break;

				case SDLK_SPACE:
					m_keysPressed[static_cast<unsigned>(KEYS_PRESSED_LIST::KEY_SPACE)] = false;
					break;
				}
			}
			break;

			case SDL_MOUSEBUTTONDOWN:
			{
				Uint8 buttonPressed = m_events.front().button.button;

				switch (buttonPressed)
				{
				case SDL_BUTTON_LEFT:
					m_buttonsPressed[static_cast<unsigned>(BUTTONS_PRESSED_LIST::LMB)] = true;
					break;

				case SDL_BUTTON_RIGHT:
					m_buttonsPressed[static_cast<unsigned>(BUTTONS_PRESSED_LIST::RMB)] = true;
					break;
				}
			}
			break;

			case SDL_MOUSEBUTTONUP:
			{
				Uint8 buttonPressed = m_events.front().button.button;

				switch (buttonPressed)
				{
				case SDL_BUTTON_LEFT:
					m_buttonsPressed[static_cast<unsigned>(BUTTONS_PRESSED_LIST::LMB)] = false;
					break;

				case SDL_BUTTON_RIGHT:
					m_buttonsPressed[static_cast<unsigned>(BUTTONS_PRESSED_LIST::RMB)] = false;
					break;
				}
			}
			break;

			case SDL_MOUSEMOTION:
			{
				SDL_MouseMotionEvent mouseEvent = m_events.front().motion;

				if (mouseEvent.xrel > 0)
				{
					m_mouseX = 1;
				}
				else if (mouseEvent.xrel < 0)
				{
					m_mouseX = -1;
				}

				if (mouseEvent.yrel > 0)
				{
					m_mouseY = 1;
				}
				else if (mouseEvent.yrel < 0)
				{
					m_mouseY = -1;
				}
			}
			break;
		}

		m_events.pop();
	}
}

bool Input::KeyIsPressed(KEYS_PRESSED_LIST key)
{
	return m_keysPressed[static_cast<unsigned>(key)];
}

bool Input::KeyWasPressed(KEYS_PRESSED_LIST key)
{
	return m_keysPressedLast[static_cast<unsigned>(key)];
}

bool Input::GetKeyDown(KEYS_PRESSED_LIST key)
{
	return (KeyWasPressed(key) == false && KeyIsPressed(key) == true);
}

bool Input::GetKeyUp(KEYS_PRESSED_LIST key)
{
	return (KeyWasPressed(key) == true && KeyIsPressed(key) == false);
}

bool Input::ButtonIsPressed(BUTTONS_PRESSED_LIST button)
{
	return m_buttonsPressed[static_cast<unsigned>(button)];
}

bool Input::ButtonWasPressed(BUTTONS_PRESSED_LIST button)
{
	return m_buttonsPressedLast[static_cast<unsigned>(button)];
}

int Input::GetMouseX(void)
{
	return m_mouseX;
}

int Input::GetMouseY(void)
{
	return m_mouseY;
}

void Input::PushEvent(SDL_Event event)
{
	m_events.push(event);
}
