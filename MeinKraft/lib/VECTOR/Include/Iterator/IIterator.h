#pragma once

template<class Type>
class IIterator
{
public:
	virtual Type&			 operator*	()						const	= 0;
	virtual bool	 operator==	(const IIterator& it)	const	= 0;
	virtual bool	 operator!=	(const IIterator& it)	const	= 0;
	virtual bool	 operator>	(const IIterator& it)	const	= 0;
	virtual bool	 operator<	(const IIterator& it)	const	= 0;
	virtual bool	 operator>=	(const IIterator& it)	const	= 0;
	virtual bool	 operator<=	(const IIterator& it)	const	= 0;
	virtual Type*			 get()								const	= 0;
};

