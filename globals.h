#pragma once

//#include <GL/glew.h>
//#include <GL/gl.h>
//#include <GL/glu.h>

#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <cstdlib>
//#include <cmath>

typedef unsigned int TimeT;

#define __SDL__ 0
#define __X11__ 1

#define __GLTOOLS_MY__

#include "util/debug.h"
#include "util/singleton.h"

#include "core/keyboard.h"
#include "core/inputmanager.h"
//#include "core/time.h"

#include "demo/scene.h"

#include "math/math.h"

#include "render/glhelper.h"
#include "render/shadermanager.h"
//#include "render/texturemanager.h"

//#include "util/templateinstantiations.cpp"
