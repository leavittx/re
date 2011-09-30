#pragma once

#include "../globals.h"

#define BITSIZE 8
#define SIZE (1 << BITSIZE)
#define MASK (SIZE - 1)

class TerrainScene : public redemo::Scene
{
public:
	TerrainScene() {}
	~TerrainScene() {}

	void init();
	void update();
	void draw();
	void release();

private:
	render::GLBatch m_triangleBatch;

	bool m_wireframe;
	bool m_texture;

	float m_landscapeSize;
	float m_step;
	float m_turbOmega;
	float m_turbK;
	float m_texCoordK;

	struct Colormap
	{
	  float n;
	  remath::Color3 col;
	};
	Colormap m_colormap[4];

	void SetColormap();
	remath::Color3 get_color(float h);

	void UploadTerrainBatch();

	float Tab1[SIZE];
	float Tab2[SIZE][SIZE];
	int Perm[SIZE];

	void SetTab1();
	void SetTab2();
	float fmod1(float t, float x);
	float noise(float t);
	float noise1d(float t);
	float noise2d(float t1, float t2);
	float noise1d_turb(float t, int omega);
	float noise2d_turb(float t1, float t2, int omega);
	float bezier(float x);
};
