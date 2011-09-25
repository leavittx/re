#pragma once

#include "../globals.h"

class TestCoreProfileScene : public redemo::Scene
{
public:
	TestCoreProfileScene() {}
	~TestCoreProfileScene() {}

	void init();
	void update();
	void draw();
	void release();
};
