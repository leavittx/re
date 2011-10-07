#pragma once

#include "../globals.h"

class TestCoreProfileScene : public redemo::Scene, recore::KeyboardEventsListener
{
public:
	TestCoreProfileScene() {}
	~TestCoreProfileScene() {}

	void init();
	void update();
	void draw();
	void release();

	virtual void handleKeyboardEvent(recore::Key key);
};
