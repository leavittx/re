#include "stringutils.h"

#ifdef linux
	#include <stdlib.h>
	#include <dirent.h>
	#include <glob.h>
#endif

using namespace reutil;
using namespace std;

// Splits a string into tokens
void StringUtils::tokenize(string str, vector<string> &tokens)
{
	stringstream ss(str);
	string buffer;
	while (ss >> buffer)
	{
		tokens.push_back(buffer);
	}
}

void StringUtils::iterateDirectory(string path, vector<string>& filenames, bool appendWildcard)
{
	filenames.clear();
#ifdef linux
	if (appendWildcard)
	{
		glob_t globbuf;
		glob(path.c_str(), 0, NULL, &globbuf);
		int n = globbuf.gl_pathc;

		if (n <= 0)
		{
			return;
		}
		n--;

		do
		{
			//TODO: make sure it's a file, not dir
			string filename = string(globbuf.gl_pathv[n]);
			filenames.push_back(filename);
		} while (n--);

		globfree(&globbuf);
	}
	else
	{
		struct dirent **namelist;
		int n = scandir(path.c_str(), &namelist, 0, alphasort);

		if (n < 0)
		{
			return;
		}
		n--;

		// List all the files in the directory with some info about them.
		do
		{
			if (namelist[n]->d_type == DT_DIR)
			{
				//directory
			}
			else if (namelist[n]->d_type == DT_REG)
			{
				//file
				string filename = string(namelist[n]->d_name);
				//free memory allocated for current file name
				free(namelist[n]);

				filenames.push_back(filename);
			}
		} while (n--);

		free(namelist);
	}
#endif // #ifdef linux
}

// Removes all instances of a character
void StringUtils::removeChar(string &str, char remove)
{
	string::iterator it;
	for (it = str.begin(); it < str.end(); )
	{
		if (*it == remove)
		{
			it = str.erase(it);
		}
		else
		{
			it++;
		}
	}
}

bool StringUtils::readFile(string path, vector<string>& buffer, bool lowercase)
{
	ifstream file(path.c_str(), ios::in);
	if (file.is_open())
	{
		while (!file.eof())
		{
			string line;
			getline(file, line);
			if (lowercase)
			{
				//TODO
			}
			buffer.push_back(line);
		}
		file.close();
		return true;
	}
	else
	{
		reutil::g_debug << "Could not open file " << path << "for reading" << std::endl;
		return false;
	}
}
