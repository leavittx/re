#pragma once

#include "../globals.h"

class TerrainScene : public redemo::Scene
{
public:
	TerrainScene() {}
	~TerrainScene() {}

	void init();
	void update();
	void draw();
	void release();
};
