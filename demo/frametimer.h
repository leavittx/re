#pragma once

/**
	A constant framerate timer / frameskipping.
*/

namespace redemo {

template <typename TimeT = unsigned int>
	class FrameTimer
{
public:
	FrameTimer();
	~FrameTimer() {}
	//first number = interval between updates (ms), second number is the maximum number of iterations
	FrameTimer(TimeT updatetime, int maxiterations);

	TimeT getDeltaTime();
	void set(TimeT updatetime, int maxiterations);

	//needs to be called once every frame
	void update(TimeT currenttime);
	//during update loop until this returns false
	bool stepsLeft();
	//this goes to the end of the step
	void endStep();

private:
	//TODO
	int m_dt; //delta time
	TimeT m_prevtime; //time previous frame
	TimeT m_cumultime; //accumulated time
	TimeT m_updatetime; //how often the update is done (ms)
	TimeT m_maxiterations; //maximum updates
};

} // end of namespace 'redemo'
