#pragma once

#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <cstdlib>

typedef unsigned int TimeT;

#define __SDL__ 0
#define __X11__ 1

#include "util/debug.h"
#include "util/singleton.h"

#include "core/keyboard.h"
#include "core/inputmanager.h"

#include "demo/scene.h"

#include "math/math.h"

#include "render/glhelper.h"
#include "render/shadermanager.h"
