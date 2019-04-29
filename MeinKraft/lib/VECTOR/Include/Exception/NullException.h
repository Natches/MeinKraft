#pragma once

#include <exception>

class NullException : public std::exception
{
public:
	NullException() throw();
};

