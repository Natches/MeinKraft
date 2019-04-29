#include <io.h>
#include <direct.h>
#include <fstream>
#include <sstream>
#include "FileUtils.h"

void CheckIfFolderExist(const char * path)
{
	if (_access(path, 0) & -1)
		_mkdir(path);
}

void ResetCursorPos(std::fstream& file)
{
	file.seekg(0, file.beg);
	file.seekp(0, file.beg);
}

void ResetCursorPos(std::stringstream& file)
{
	file.seekg(0, file.beg);
	file.seekp(0, file.beg);
}

void JumpToNext(std::stringstream& fileStream)
{
	fileStream >> std::string();
}

void FindSTR(std::stringstream& sstr, std::string& str)
{
	std::string temp;
	while (temp != str && !sstr.fail())
	{
		sstr >> temp;
	}
}
