#pragma once

#include "../globals.h"

#define BITSIZE 8
#define SIZE (1 << BITSIZE)
#define MASK (SIZE - 1)

class NoisyScene : public redemo::Scene, public recore::KeyboardEventsListener
{
public:
	NoisyScene() {}
	~NoisyScene() {}

	void init();
	void update();
	void draw();
	void release();

	virtual void handleKeyboardEvent(recore::Key key);

private:
	bool m_wireframe;
	bool m_texture;

	float m_landscapeSize;
	float m_step;
	float m_turbOmega;
	float m_turbK;

	struct Colormap
	{
	  float n;
	  remath::Color3 col;
	};
	Colormap m_colormap[4];

	void SetColormap();
	remath::Color3 get_color(float h);

	void draw_1d_noise(TimeT Time);
	void draw_2d_noise(TimeT Time);
	void draw_2d_noise_(TimeT Time);
	void draw_axes();
	void draw_smth();

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
