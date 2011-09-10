#pragma once

/**
	Config class used for the demo settings. Includes the startup dialog.
*/

namespace recore {

class Config
{
public:
	Config() {}
	virtual ~Config() {}

	virtual bool run() = 0;

	virtual int getScreenWidth() = 0;
	virtual int getScreenHeight() = 0;
	virtual int getScreenFrequency() = 0;
	virtual bool getFullscreen() = 0;
	virtual bool getSound() = 0;
	virtual bool getOnTop() = 0;
	virtual bool getRunFlag() = 0;
};

} // end of namespace 'recore'
