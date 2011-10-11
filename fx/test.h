#ifdef __DEPRECATED_PROFILE__

#pragma once

#include "../globals.h"

class TestScene : public redemo::Scene
{
public:
	TestScene() {}
	~TestScene() {}

	void init();
	void update();
	void draw();
	void release();
};

#endif /* ifdef __DEPRECATED_PROFILE__ */
