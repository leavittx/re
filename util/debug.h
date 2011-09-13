#pragma once

#include <fstream>

/**
	Debug class used for writing the log file.
*/

namespace reutil {

class Debug;
extern Debug g_debug;

class Debug
{
public:
	Debug()
	{
		m_debugFile.open("log.txt");
	}

	~Debug()
	{
		m_debugFile.close();
	}

	void flush()
	{
	   m_debugFile.flush();
	}

	template<class T>
		std::ofstream& operator<<(const T &str)
	{
		//printDate();
		m_debugFile << str;
		m_debugFile.flush();
		return m_debugFile;
	}

	template<class T>
	std::ofstream& operator + (const T &str)
	{
		m_debugFile << str << std::endl;
		m_debugFile.flush();
		return m_debugFile;
	}

private:
	std::ofstream m_debugFile;

	//TODO: virtual method for different os frontends
	//void printDate();
};

} // end of namespace 'reutil'

using reutil::g_debug;
