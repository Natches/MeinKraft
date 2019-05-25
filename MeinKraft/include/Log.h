#pragma once
#include <string.h>
#include <fstream>

class Log final
{
public:
#pragma region Public Attributes
#pragma endregion

#pragma region Public Constants
	enum e_Output_Type
	{
		CONSOLE,
		FILE,
		BOTH
	};

	enum e_Message_Type
	{
		WARNING,
		FAULT,
		INFO
	};

	static const std::string	DEFAULT_OUTPUT_PATH;
#pragma endregion

#pragma region Public Methods
	~Log();

	Log(Log const&) = delete;
	Log(Log&&) = delete;

	static Log* GetInstance();
	void		WriteMessage(const std::string& message, const e_Message_Type& messageType, const e_Output_Type& outputType, const char* file = "", int line = 0);
	void		ChangeDestinationFolder(const std::string& destination);
	void		Clean();
#pragma endregion


private:
#pragma region Private Attributes
	static Log*		_instance;
	e_Output_Type	_outputType;
	std::ofstream	_file;
	std::string		_fileName;
	std::string		_filePath;
#pragma endregion

#pragma region Private Constants
	static const e_Output_Type	DEFAULT_OUTPUT_TYPE;
	static const std::string	DEFAULT_NAME_FORMAT;
#pragma endregion

#pragma region Private Methods
	Log();

	void		SetOuputType	(const e_Output_Type& type);
	void		CloseFile		();
	void		OpenFile		();
	void		ChooseHeader	(std::string& header, const e_Message_Type& messageType);
	void		WriteLines		(const std::string& header, const std::string& message, const char* file, const int line);
	std::string	ParseCurrentTime();
#pragma endregion
};

