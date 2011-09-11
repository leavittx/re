#include "inputmanager.h"

using namespace recore;
using namespace std;

InputManager::InputManager()
{
}

InputManager::~InputManager()
{
}

void InputManager::acceptKeyboardEvents(KeyboardEventsListener* who)
{
	for (int i = 0; i < (int)m_keyboardListeners.size(); i++)
	{
		if (m_keyboardListeners[i] == who)
			return;
	}
	m_keyboardListeners.push_back(who);
}

void InputManager::rejectKeyboardEvents(KeyboardEventsListener* who)
{
	for (int i = 0; i < (int)m_keyboardListeners.size(); i++)
	{
		if (m_keyboardListeners[i] == who)
			m_keyboardListeners.erase(m_keyboardListeners.begin() + i);
	}
}

void InputManager::sendKeyboardEvent(Key key)
{
	for (int i = 0; i < (int)m_keyboardListeners.size(); i++)
	{
		m_keyboardListeners[i]->handleKeyboardEvent(key);
	}
}
