#include "demo.h"
#include "core/system.h"
#include "math/math.h"
#include "util/debug.h"
#include <algorithm>

using namespace std;
using namespace recore;
using namespace remath;
using namespace redemo;

/***********************************************
	TimelineEntry
***********************************************/

TimelineEntry::TimelineEntry()
{
	m_endTime = 0;
	m_startTime = 0;
	m_scene = 0;
	m_priority = 0;
	m_name = "";
	m_frametimer = NULL;
}

TimelineEntry::~TimelineEntry()
{
	destroy();
}

void TimelineEntry::destroy()
{
	if (m_frametimer != NULL)
	{
		delete m_frametimer;
		m_frametimer = NULL;
	}
}

/***********************************************
	Demo
***********************************************/

bool Demo::initOk()
{
	return m_start;
}

Demo::Demo(string& scriptfile):m_scriptName(scriptfile)
{
	g_debug << "initializing demo" << endl;

	//TODO: script parsing here

	//initialize configuration dialog
	m_config = new recore::ConfigSimple;
	m_config->run();

	if (m_config->getRunFlag())
	{
		System::inst().initOpenGL(*m_config);

		//TODO: Sound loading here

		m_start = true;
	}
	else
	{
		m_start = false;
	}

	//TODO: maybe Config singleton
	delete m_config;

	InputManager::inst().acceptKeyboardEvents(this);
}

Demo::~Demo()
{
	g_debug << "demo quitting.." << endl;
	destroy();
	System::inst().kill();
}

void Demo::start()
{
	g_debug << "starting demo now!" << endl;
	m_running = true;
	//	System::inst().startSong();
}

void Demo::stop()
{
	//	System::inst().stopSong();
	g_debug << "demo stopped!" << endl;
}

void Demo::destroy()
{
	if (m_start)
	{
		map<string, Scene *>::iterator it;
		//TODO: this looks quite scairy
		typename vector< TimelineEntry* >::iterator it2;

		//clear out all the scenes in the demo
		for (it = m_scenes.begin(); it != m_scenes.end(); it++)
		{
			delete (*it).second;
		}
		//and the timeline
		for (it2 = m_timeline.begin(); it2 < m_timeline.end(); it2++)
		{
			(*it2)->destroy();
		}
		m_timeline.clear();
		m_scenes.clear();

		//kill audio
		//		System::inst().freeSound();
	}
}

void Demo::addScene(const string& name, Scene *s)
{
	//insert the scene to the map
	m_scenes[name] = s;
	g_debug << "added scene \"" << name << "\" to the demo" << endl;
}

Scene* Demo::getScene(const string& name)
{
	if (m_scenes.find(name) != m_scenes.end())
	{
		return m_scenes[name];
	}
	else
	{
		g_debug << "error! Trying to fetch scene " << name << " but it does not exist\n";
		return 0;
	}
}

void Demo::addSceneToTimeline(const string& name, TimeT startTime, TimeT endTime, int priority)
{
	//check if such a scene exists
	if (m_scenes.find(name) != m_scenes.end())
	{
		//create timeline entry for it
		TimelineEntry* entry = new TimelineEntry();

		entry->m_scene = m_scenes[name];
		entry->m_startTime = startTime;
		entry->m_endTime = endTime;
		entry->m_priority = priority;
		entry->m_name = name;
		const int FPS = 100;
		const int MAXITERATIONS = 30;
		entry->m_frametimer = new FrameTimer(1000/FPS, MAXITERATIONS); //
		g_debug << "created a timeline entry for scene \"" << name << "\" at [" << startTime << "," << endTime << "]" << endl;

		m_timeline.push_back(entry);
	}
	else
	{
		//no such scene exists
		g_debug << "ERROR! no scene with name \"" << name << "\" exists!" << endl;
	}
}

void Demo::initEffects()
{
	//call init on all effects
	map<string, Scene *>::iterator it;

	for (it = m_scenes.begin(); it != m_scenes.end(); it++)
	{
		(*it).second->init();
	}
}

void Demo::releaseEffects()
{
	//call init on all effects
	map<string, Scene *>::iterator it;

	for (it = m_scenes.begin(); it != m_scenes.end(); it++)
	{
		(*it).second->release();
	}
}

//we need this since STL doesn't know how to sort vectors of pointers
bool timelineEntryComparisonFunction(const TimelineEntry *a, const TimelineEntry *b)
{
	return a->m_priority < b->m_priority;
}

void Demo::draw()
{
	typename vector< TimelineEntry* >::iterator it;
	vector< TimelineEntry* > activeEffects;

	TimeT time = System::inst().getTime();
	for (it = m_timeline.begin(); it < m_timeline.end(); it++)
	{
		TimelineEntry *e = *it;

		if (time >= e->m_startTime && time < e->m_endTime)
		{
			activeEffects.push_back(e);
		}
	}
	sort(activeEffects.begin(), activeEffects.end(), timelineEntryComparisonFunction);
	for (it = activeEffects.begin(); it < activeEffects.end(); it++)
	{
		TimelineEntry *e = *it;
		e->m_scene->draw();
	}
}

//void Demo::toggleRunning()
//{
//	if (m_running)
//	{
//		m_running = false;
//		//		System::inst().pauseSong(true);
//	}
//	else
//	{
//		m_running = true;
//		//		System::inst().pauseSong(false);
//	}
//}

void Demo::update()
{
	if (m_running)
	{
		typename vector< TimelineEntry* >::iterator it;
		TimeT time = System::inst().getTime();

		for (it = m_timeline.begin(); it < m_timeline.end(); it++)
		{
			TimelineEntry *e = *it;
			if (time >= e->m_startTime && time < e->m_endTime)
			{
				float t = Math::calcPosInt(time, e->m_startTime, e->m_endTime);

				e->m_scene->setPosition(t);
				e->m_frametimer->update(time);
				while (e->m_frametimer->stepsLeft())
				{
					e->m_scene->update();
					e->m_frametimer->endStep();
				}
			}
		}
	}
}

void Demo::handleKeyboardEvent(Key key)
{
	// TODO: if developer mode enabled, handle keyboard input (reload effects, etc)
	switch (key)
	{
	case KeySpace:
		// Pause/resume
		m_running = !m_running;
		break;

	case KeyEnter:
		g_debug << System::inst().getTime() << endl;
		break;

	default:
		break;
	}
}
