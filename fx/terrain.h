#pragma once

#include "../globals.h"

#define BITSIZE 8
#define SIZE (1 << BITSIZE)
#define MASK (SIZE - 1)

class TerrainScene : public redemo::Scene, recore::KeyboardEventsListener
{
public:
	TerrainScene() {}
	~TerrainScene() {}

	void init();
	void update();
	void draw();
	void release();

	virtual void handleKeyboardEvent(recore::Key key);

private:
	render::GLBatch m_terrainBatch;

	render::GLFrame m_cameraFrame;
	render::GLMatrixStack m_modelViewMatrix;
	render::GLMatrixStack m_projectionMatrix;
	render::GLGeometryTransform m_transformPipeline;

	bool m_wireframe;
	bool m_points;
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

	float Tab2[SIZE][SIZE];
	int Perm[SIZE];

	void SetTab2();
	float fmod1(float t, float x);
	float noise2d(float t1, float t2);
	float noise2d_turb(float t1, float t2, int omega);

	render::GLBatch m_cubeBatch;
};
