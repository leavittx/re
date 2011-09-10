#pragma once

#include "config.h"

namespace recore {

class ConfigSimple : public Config
{
public:
	ConfigSimple();
	virtual ~ConfigSimple();

	virtual bool run();

	virtual int getScreenWidth();
	virtual int getScreenHeight();
	virtual int getScreenFrequency();
	virtual bool getFullscreen();
	virtual bool getSound();
	virtual bool getOnTop();
	virtual bool getRunFlag();
};

} // end of namespace 'recore'
