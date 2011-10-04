#pragma once

#include "core/keyboard.h"
#include "util/singleton.h"
#include <vector>

namespace recore {

class KeyboardEventsListener
{
public:
	KeyboardEventsListener() {}
	virtual ~KeyboardEventsListener() {}

	virtual void handleKeyboardEvent(Key key) = 0;
};

class InputManager : public reutil::Singleton<InputManager>
{
public:
	void acceptKeyboardEvents(KeyboardEventsListener* who);
	void rejectKeyboardEvents(KeyboardEventsListener* who);
	void sendKeyboardEvent(Key key);

private:
	friend class Singleton;
	InputManager();
	~InputManager();

	std::vector<KeyboardEventsListener *> m_keyboardListeners;
};

} // end of namespace 'recore'
