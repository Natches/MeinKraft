#include "Log.h"
#include <iostream>
#include <direct.h>
#include <time.h>
#include <iomanip>
#include <sstream>
#include <windows.h>

Log*						Log::_instance				= nullptr;
const Log::e_Output_Type	Log::DEFAULT_OUTPUT_TYPE	= e_Output_Type::FILE;
const std::string			Log::DEFAULT_NAME_FORMAT	= "%d_%m_%Y__%H_%M";

#ifdef _DEBUG
const std::string			Log::DEFAULT_OUTPUT_PATH = "Logs\\";
#else
const std::string			Log::DEFAULT_OUTPUT_PATH = "Logs\\";
#endif


Log::Log()
{
	_outputType = DEFAULT_OUTPUT_TYPE;
	_filePath	= DEFAULT_OUTPUT_PATH;
	_fileName	= _filePath;
	_fileName	+= ParseCurrentTime();
	_fileName	+= ".log";
}

Log::~Log()
{
	Clean();
}

void Log::Clean()
{
	delete _instance;
	CloseFile();
}

Log* Log::GetInstance()
{
	if (!_instance)
	{
		_instance = new Log();
	}
	return _instance;
}

void Log::SetOuputType(const e_Output_Type& type)
{
	if (type != _outputType)
	{
		if (_outputType == e_Output_Type::FILE || _outputType == e_Output_Type::BOTH)
		{
			CloseFile();
		}
		_outputType = type;
		if (_outputType == e_Output_Type::FILE || _outputType == e_Output_Type::BOTH)
		{
			OpenFile();
		}
	}
}

void Log::WriteMessage(const std::string& message, const e_Message_Type& messageType, const e_Output_Type& outputType, const char* file, int line)
{
	SetOuputType(outputType);
	std::string header;

	ChooseHeader(header, messageType);
	WriteLines(header, message, file, line);
}

void Log::ChooseHeader(std::string& header, const e_Message_Type& messageType)
{
	switch (messageType)
	{
	case e_Message_Type::INFO:
		header = "[INFORMATION]\n";
		break;
	case e_Message_Type::WARNING:
		header = "[WARNING] => ";
		break;
	case e_Message_Type::FAULT:
		header = "[ERROR]\n";
		break;
	default:
		header = "[UNKNOWN]\n";
	}
}

void Log::WriteLines(const std::string& header, const std::string& message, const char* file, const int line)
{
	switch (_outputType)
	{
	case e_Output_Type::FILE:
		if (_file.is_open())
		{
			_file << header << message << "\n";
			_file << "[" << file << ";" << line << "]" << std::endl;
		}
		break;
	case e_Output_Type::CONSOLE:
		std::cout << header << message << "\n";
		std::cout << "[" << file << ";" << line << "]" << std::endl;
		break;
	case e_Output_Type::BOTH:
		if (_file.is_open())
		{
			_file << header << message << "\n";
			_file << "[" << file << ";" << line << "]" << std::endl;
		}
		std::cout << header << message << "\n";
		std::cout << "[" << file << ";" << line << "]" << std::endl;
		break;
	default:
		if (_file.is_open())
		{
			_file << "Unknown\n";
		}
		std::cout << "Unknown\n";
	}
}

void Log::OpenFile()
{
	if (!_file.is_open())
	{
		_mkdir(_filePath.c_str());
		if (errno == ENOENT)
		{
			std::cout << "The folder at " << DEFAULT_OUTPUT_PATH << " could not be created. Path not found." << std::endl;
		}
		else
		{
			_file.open(_fileName, std::ios_base::app);
			if (_file.is_open())
			{
				_file << "[FILE OPENED]\n";
			}
		}
	}
}

void Log::CloseFile()
{
	if (_file.is_open())
	{
		_file << "[FILE CLOSED]\n";
		_file.close();
	}
}

void Log::ChangeDestinationFolder(const std::string& destination)
{
	CloseFile();
	_filePath = destination;
	_fileName = _filePath;
	_fileName += ParseCurrentTime();
	_fileName += ".log";
	OpenFile();
}

std::string Log::ParseCurrentTime()
{
	std::ostringstream oss;
	struct tm buf;
	auto t = std::time(nullptr);
	localtime_s(&buf, &t);
	oss << std::put_time(&buf, DEFAULT_NAME_FORMAT.c_str());
	return oss.str();
}