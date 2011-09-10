#pragma once

#include "../globals.h"

class SphereScene : public redemo::Scene
{
public:
	SphereScene() {}
	~SphereScene() {}

	void init();
	void update();
	void draw();
	void release();

private:
	void RegenerateSphere(void);
	void SetRenderingMethod(void);
	void DrawModels(void);

	unsigned int m_bufferID;

	bool m_useBufferObject;
	bool m_mapBufferObject;
	bool m_animating;

	int m_numSphereVertices;
	GLfloat *m_sphereVertexArray;

	static const float ambientLight[];
	static const float diffuseLight[];
	static const float cameraPos[];
};
