#pragma once
 
#include "Allocator/Allocator.h"

/*
extern "C++"
{
	template<class Type, class Allocator>
	class List;
	template<class Type>
	class IIterator;

	template<class Type, class Allocator>
	class IContainer
	{

	public:
#pragma region InsertThings
		virtual void insert(const Type& ObjectToInsert) = 0;
		virtual void insert(const Type& ObjectToInsert, unsigned int Where) = 0;
		virtual void insert_before(const Type& ObjectToInsert, unsigned int Where) = 0;
		virtual void insert_after(const Type& ObjectToInsert, unsigned int Where) = 0;
		virtual void push_front(const Type& ObjectToPushFront) = 0;
		//virtual void push_back(const Type& ObjectToPushBack) = 0;
		
		virtual void insert( Type&& ObjectToInsert) = 0;
		virtual void insert( Type&& ObjectToInsert, unsigned int Where) = 0;
		virtual void insert_before( Type&& ObjectToInsert, unsigned int Where) = 0;
		virtual void insert_after( Type&& ObjectToInsert, unsigned int Where) = 0;
		virtual void push_front( Type&& ObjectToPushFront) = 0;
		//virtual void push_back( Type&& ObjectToPushBack) = 0;
#pragma endregion

	public:
#pragma region EraseThings
		virtual void pop_front(const bool DeleteAuto = false) = 0;
		virtual void pop_back(const bool DeleteAuto = false) = 0;

		/ *Erase, clean Data but let Node usable with insert,
		Erase, return Pointer To Data of Node,
		if you doesn't need the data anymore just use turn true the booleen
		(Destructor | Delete will be called), nothing return in this case* /
		virtual void erase(unsigned int Where, const bool DeleteAuto = false) = 0;

		/ *Remove, clean the Data & Node,
		Remove, return Pointer To Data of Node,
		if you doesn't need the data anymore just use turn true the booleen
		(Destructor | Delete will be called), nothing return in this case* /
		virtual void remove(unsigned int Where, const bool DeleteAuto = false) = 0;

		/ *Clear, Delete all Node && Data,
		if you doesn't need the data anymore just use turn true the booleen
		(Destructor | Delete will be called)* /
		virtual void clear(const bool DeleteAuto = false) = 0;
#pragma endregion

	public:
#pragma region Reserve/Resize
		virtual void reserve(const unsigned int How) = 0;
		virtual void resize(const unsigned int NewSize) = 0;
#pragma endregion


	public:
#pragma region Convert
		virtual Type*					ToArray() = 0;
//virtual List<Type, Allocator>	ToList() = 0;
#pragma endregion

	public:
#pragma region Copy
		/ *virtual void CopyAllIn	(const IContainer<Type, Allocator> containerB) = 0;
		virtual void CopyIn		(const IContainer<Type, Allocator> containerB) = 0;* /
#pragma endregion
	};

	///!To Implement separately!\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
	//	virtual void remove(const IIterator<Type>& it, const bool DeleteAuto = false) = 0;	\\
	//	virtual void erase(const IIterator<Type>& it, const bool DeleteAuto = false) = 0;	\\
	//	virtual void insert_after( Type&& ObjectToInsert, const IIterator<Type>& it) = 0;	\\
	//	virtual void insert_before( Type&& ObjectToInsert, const IIterator<Type>& it) = 0;	\\
	//	virtual void insert_after(Type& ObjectToInsert, const IIterator<Type>& it) = 0;		\\
	//	virtual void insert_before(Type& ObjectToInsert, const IIterator<Type>& it) = 0;	\\
	//////////////////////////////////////////////////////////////////////////////////////////
}*/