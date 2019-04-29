#ifndef __LIST_H__
#define __LIST_H__

#include <future>
#include <unordered_map>
#include "Container/IContainer.h"
#include "List/ListNode/ListNode.h"
#include "Allocator/Allocator.h"

template<class Type, class Allocator = Allocator<ListNode<Type>>>
class List : public IContainer<Type, Allocator>
{
	enum E_MODE
	{
		SINGLETHREADED,
		MULTITHREADED
	};

public:
	List( const unsigned int NodeToReserve = 0, const E_MODE _mode = E_MODE::SINGLETHREADED);
	~List() = default;

public:
#pragma region InsertThings
	virtual void insert(const Type ObjectToInsert);
	virtual void insert(const Type ObjectToInsert, const unsigned int Where);
	virtual void insert_before(const Type ObjectToInsert, const unsigned int Where);
	virtual void insert_before(const Type ObjectToInsert, const Type What);
	virtual void insert_after(const Type ObjectToInsert, const unsigned int Where);
	virtual void insert_after(const Type ObjectToInsert, const Type What);
	virtual void push_front(const Type ObjectToSetFirst);
	virtual void push_back(const Type ObjectToPushFront);
#pragma endregion

public:
#pragma region EraseThings
	virtual const Type pop_front();
	virtual const Type pop_back();

	/*Erase, clean Data but let Node usable with insert,
	Erase, return Pointer To Data of Node,
	if you doesn't need the data anymore just use turn true the booleen
	(Destructor | Delete will be called), nothing return in this case*/
	virtual const Type erase(const unsigned int Where, const bool DeleteAuto = false);
	virtual const Type erase(const Type& What, const bool DeleteAuto = false);

	/*Remove, clean the Data & Node,
	Remove, return Pointer To Data of Node,
	if you doesn't need the data anymore just use turn true the booleen
	(Destructor | Delete will be called), nothing return in this case*/
	virtual const Type remove(const unsigned int Where, const bool DeleteAuto = false);
	virtual const Type remove(const Type& What, const bool DeleteAuto = false);

	/*Clear, Delete all Node && Data,
	if you doesn't need the data anymore just use turn true the booleen
	(Destructor | Delete will be called)*/
	virtual void clear(const bool DeleteAuto = false);
#pragma endregion

public:
#pragma region Reserve/Resize
	virtual void reserve(const unsigned int How);
	virtual void resize(const unsigned int NewSize);
#pragma endregion

public:
#pragma region FindThings
	Type&						operator[]	(const unsigned int Where)	const;
	unsigned int				find		(const Type& What)			const;
	const ListNode<Type>* const operator[]	(const Type& What)			const;
	const ListNode<Type>* const find(const Type& What)			const;

private:
	const ListNode<Type>* const FindDirtyData(void* useLessParam, const unsigned int First = 0, const unsigned int Last = size)const;
	const ListNode<Type>* const FindDirtyDataMultiThreaded();
	const ListNode<Type>* const LaunchFindThreadController(const unsigned int threadToHave, ListNode<Type>* (*Find)(void*), void* param);
	const ListNode<Type>* const Find(const Type What, const unsigned int First = 0, const unsigned int Last = size)const;
#pragma endregion

public:
#pragma region Convert
	virtual Type*					ToArray();
	virtual List<Type, Allocator>	ToList();
#pragma endregion

public:
	inline const unsigned int getSize() { return nodeMap.size(); }

private:
	E_MODE mode;
	ListNode<Type>* Head;
	ListNode<Type>* Middle;
	ListNode<Type>* End;

private:
	static const unsigned int maxConcurency = (std::thread().hardware_concurrency() - 1);
};

#include "../Src/List/List.cpp"

#endif // !__LIST_H__