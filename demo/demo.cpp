#include <algorithm>
#include "demo.h"
#include "core/system.h"
//#include "parser.h"
#include "math/math.h"
#include "util/debug.h"

using namespace std;
using namespace recore;
using namespace remath;
using namespace redemo;

/***********************************************
				TimelineEntry
***********************************************/

template <typename TimeT>
	TimelineEntry<TimeT>::TimelineEntry()
{
	m_endTime = 0;
	m_startTime = 0;
	m_scene = 0;
	m_priority = 0;
	m_name = "";
	m_frametimer = NULL;
}

template <typename TimeT>
	TimelineEntry<TimeT>::~TimelineEntry()
{
	destroy();
}

template <typename TimeT>
	void TimelineEntry<TimeT>::destroy()
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

template <typename TimeT>
	bool Demo<TimeT>::initOk()
{
	return m_start;
}

template <typename TimeT>
	Demo<TimeT>::Demo(string& scriptfile):m_scriptName(scriptfile)
{
	g_debug << "initializing demo" << endl;

	//TODO: script parsing here

	//initialize configuration dialog
	m_config = new recore::Config();
	m_config->run();

	if (m_config->getRunFlag())
	{
		System<TimeT>::inst()->initOpenGL(*m_config);

		//TODO: Sound loading here
	}
	else
	{
		m_start = false;
	}

	//TODO: maybe Config singleton
	delete m_config;
}

template <typename TimeT>
	Demo<TimeT>::~Demo()
{
	g_debug << "demo quitting.." << endl;
	destroy();
	System<TimeT>::inst()->kill();
}

template <typename TimeT>
	void Demo<TimeT>::start()
{
	g_debug << "starting demo now!" << endl;
	m_running = true;
	System<TimeT>::inst()->startSong();
}

template <typename TimeT>
	void Demo<TimeT>::stop()
{
	System<TimeT>::inst()->stopSong();
	g_debug << "demo stopped!" << endl;
}

template <typename TimeT>
	void Demo<TimeT>::destroy()
{
	if (m_start)
	{
		map<string, Scene *>::iterator it;
		//TODO: this looks quite scairy
		typename vector< TimelineEntry<TimeT>* >::iterator it2;

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
		System<TimeT>::inst()->freeSound();
	}
}

template <typename TimeT>
	void Demo<TimeT>::addScene(const string& name, Scene *s)
{
	//insert the scene to the map
	m_scenes[name] = s;
	g_debug << "added scene \"" << name << "\" to the demo" << endl;
}

template <typename TimeT>
	Scene* Demo<TimeT>::getScene(const string& name)
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

template <typename TimeT>
	void Demo<TimeT>::addSceneToTimeline(const string& name, TimeT startTime, TimeT endTime, int priority)
{
	//check if such a scene exists
	if (m_scenes.find(name) != m_scenes.end())
	{
		//create timeline entry for it
		TimelineEntry<TimeT>* entry = new TimelineEntry<TimeT>();

		entry->m_scene = m_scenes[name];
		entry->m_startTime = startTime;
		entry->m_endTime = endTime;
		entry->m_priority = priority;
		entry->m_name = name;
		const int FPS = 100;
		const int MAXITERATIONS = 30;
		entry->m_frametimer = new FrameTimer<TimeT>(1000/FPS, MAXITERATIONS); //
		g_debug << "created a timeline entry for scene \"" << name << "\" at [" << startTime << "," << endTime << "]" << endl;

		m_timeline.push_back(entry);
	}
	else
	{
		g_debug << "ERROR! no scene with name \"" << name << "\" exists!" << endl;
		//no such scene exists
	}
}

template <typename TimeT>
	void Demo<TimeT>::initEffects()
{
	//call init on all effects
	map<string, Scene *>::iterator it;

	for (it = m_scenes.begin(); it != m_scenes.end(); it++)
	{
		(*it).second->init();
	}
}

template <typename TimeT>
	void Demo<TimeT>::releaseEffects()
{
	//call init on all effects
	map<string, Scene *>::iterator it;

	for (it = m_scenes.begin(); it != m_scenes.end(); it++)
	{
		(*it).second->release();
	}
}

//we need this since STL doesn't know how to sort vectors of pointers
template <typename TimeT>
	bool timelineEntryComparisonFunction(const TimelineEntry<TimeT> *a, const TimelineEntry<TimeT> *b)
{
	return a->m_priority < b->m_priority;
}

template <typename TimeT>
	void Demo<TimeT>::draw()
{
	typename vector< TimelineEntry<TimeT>* >::iterator it;
	vector< TimelineEntry<TimeT>* > activeEffects;

	int time = System<TimeT>::inst()->getTime();
	for (it = m_timeline.begin(); it < m_timeline.end(); it++)
	{
		TimelineEntry<TimeT> *e = *it;

		if (time >= e->m_startTime && time < e->m_endTime)
		{
			activeEffects.push_back(e);
		}
	}
	sort(activeEffects.begin(), activeEffects.end(), timelineEntryComparisonFunction<TimeT>);
	for (it = activeEffects.begin(); it < activeEffects.end(); it++)
	{
		TimelineEntry<TimeT> *e = *it;
		e->m_scene->draw();
	}

}

template <typename TimeT>
	void Demo<TimeT>::toggleRunning()
{
	if (m_running)
	{
		m_running = false;
		System<TimeT>::inst()->pauseSong(true);
	}
	else
	{
		m_running = true;
		System<TimeT>::inst()->pauseSong(false);
	}
}

template <typename TimeT>
	void Demo<TimeT>::update()
{
	// TODO: if developer mode enabled, handle keyboard input (reload effects, etc)

	if (m_running)
	{
		typename vector< TimelineEntry<TimeT>* >::iterator it;
		int time = System<TimeT>::inst()->getTime();

		for (it = m_timeline.begin(); it < m_timeline.end(); it++)
		{
			TimelineEntry<TimeT> *e = *it;
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
