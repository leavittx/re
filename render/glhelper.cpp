#include "glhelper.h"

//float render::glHelper::m_zNear;
//float render::glHelper::m_zFar;
//float render::glHelper::m_fov;
int render::glHelper::m_width;
int render::glHelper::m_height;
float render::glHelper::m_aspectratio;

render::GLFrustum render::glHelper::m_viewFrustum;
//GLFrustum render::glHelper::m_viewFrustum;

const GLint render::glHelper::cullMode_toGL[] = { GL_POINT, GL_LINE, GL_FILL, GL_NONE };
