#pragma once

/**
	Config class used for the demo settings. Includes the startup dialog.
*/

namespace recore
{

class Config
{
public:
	Config();
	~Config();

	virtual bool run();

	virtual int getScreenWidth();
	virtual int getScreenHeight();
	virtual int getScreenFrequency();
	virtual bool getFullscreen();
	virtual bool getSound();
	virtual bool getOnTop();
	virtual bool getRunFlag();

private:

};

} // end of namespace 'recore'
