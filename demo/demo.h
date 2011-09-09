#pragma once

#include <map>
#include <vector>
#include <string>
#include "core/config.h"
#include "frametimer.h"
#include "scene.h"

/**
	An entry in the timeline.
	It has a pointer to the actual effect that it'll run, plus a bunch of other related stuff.
*/

namespace redemo
{

template <typename TimeT = unsigned int>
	class TimelineEntry
{
public:
	TimelineEntry();
	~TimelineEntry();
	void destroy();

	//comparison operator for STL sort
	inline bool operator < (const TimelineEntry& otherEntry) const
	{
		return m_priority < otherEntry.m_priority;
	}
	Scene* m_scene; //ptr to the scene
	FrameTimer<TimeT>* m_frametimer; //frameskipper
	TimeT m_startTime; //starts here
	TimeT m_endTime;   //ends then
	int m_priority;  //priority in which the simultaneous effects will be drawn. bigger priority will be drawn first
	std::string m_name;   //name with what this is referenced
};

/**
	The main class for the demo.
*/

template <typename TimeT = unsigned int>
	class Demo
{
public:
	Demo(std::string& scriptfile);
	~Demo();

	//returns true if we are all go for launch
	bool initOk();

	//add a scene to the scene list
	void addScene(const std::string& name, Scene* s);
	//add a scene to the timeline
	void addSceneToTimeline(const std::string& name, TimeT startTime, TimeT endTime, int priority);
	//get scene from the scene list
	Scene* getScene(const std::string& name);

	//inits all effects
	void initEffects();
	//releases all effects
	void releaseEffects();
	//toggles running state
	void toggleRunning();

	//start the demo
	void start();
	//stop the demo
	void stop();

	//draw currently active scenes
	void draw();
	//update currently active scenes
	void update();
	//clear everything
	void destroy();


private:
	//this contains all the effects
	std::map<std::string, Scene*> m_scenes;
	//this contains all the timeline entries that will be played
	std::vector< TimelineEntry<TimeT>* > m_timeline;

	recore::Config* m_config;
	bool m_start;
	std::string m_scriptName;
	bool m_running;
};

} // end of namespace 'redemo'
