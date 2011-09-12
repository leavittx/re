#pragma once

#include "../globals.h"

namespace reutil {

class StringUtils
{
public:
	// Convert a std::string to number
	template <class T>
	static bool convertStringToNumber(T& t, const std::string& s, std::ios_base& (*f)(std::ios_base&))
	{
		std::stringstream iss(s);
		return !(iss >> f >> t).fail();
	}
	// Iterates a directory and gets all filenames from it into a std::vector
	static void iterateDirectory(std::string path, std::vector<std::string>& filenames, bool appendWildcard = true);
	// Read a text file into buffer
	static bool readFile(std::string path, std::vector<std::string>& buffer, bool lowercase = false);
	// Split the std::string into tokens
	static void tokenize(std::string str, std::vector<std::string> &tokens);
	// Remove all instances of a character from a string
	static void removeChar(std::string &str, char remove);
};

} // end of namespace 'reutil'
