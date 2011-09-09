#pragma once

namespace reutil {

// Meyers singleton class
template <class T>
class Singleton
{
public:
	static T& inst()
	{
		static T sm_instance;
		return sm_instance;
	}

protected:
	Singleton() {}
};

} // end of namespace 'reutil'
