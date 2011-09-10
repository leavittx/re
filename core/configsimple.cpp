#include "configsimple.h"

using namespace recore;

ConfigSimple::ConfigSimple()
{
}

ConfigSimple::~ConfigSimple()
{
}

bool ConfigSimple::run()
{
	return true;
}

int ConfigSimple::getScreenWidth()
{
	return 1024;
//	return 1366;
}

int ConfigSimple::getScreenHeight()
{
	return 768;
}

int ConfigSimple::getScreenFrequency()
{
	return 0;
}

bool ConfigSimple::getFullscreen()
{
	return false;
}

bool ConfigSimple::getSound()
{
	return false;
}

bool ConfigSimple::getOnTop()
{
	return false;
}

bool ConfigSimple::getRunFlag()
{
	return true;
}
