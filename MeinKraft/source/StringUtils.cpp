#include "StringUtils.h"
namespace strutils
{
	void ParseVector(const std::string& line, float* array)
	{
		unsigned int occurrence = std::count(line.begin(), line.end(), ',');

		switch (occurrence)
		{
		case 1:
			sscanf_s(line.c_str(), "%f,%f", &array[0], &array[1]);
			break;
		case 2:
			sscanf_s(line.c_str(), "%f,%f,%f", &array[0], &array[1], &array[2]);
			break;
		case 3:
			sscanf_s(line.c_str(), "%f,%f,%f,%f", &array[0], &array[1], &array[2], &array[3]);
			break;
		}
	}
}