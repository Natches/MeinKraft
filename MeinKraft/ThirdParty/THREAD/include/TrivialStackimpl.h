#ifdef __TRIVIAL_STACK__

#include <iostream>
#include "TrivialStack.h"

template<typename T>
void TrivialStack<T>::push(T Data)
{
	uniqueLockMutex.lock();
	if (empty())
	{		
		first = new TrivialStackNode<T>();
		last = first;
		first->setData(Data);
		first->setPrevious(nullptr);
	}
	else
	{
		TrivialStackNode<T>* previous = first;
		first = new TrivialStackNode<T>();
		first->setPrevious(previous);
		first->setData(Data);
	}
	_size.store(_size.load(std::memory_order_acquire) + 1, std::memory_order_release);
	uniqueLockMutex.unlock();
}

template<typename T>
inline TrivialStack<T>::~TrivialStack()
{
	Clear();
}

template<typename T>
void TrivialStack<T>::pop()
{
	uniqueLockMutex.lock();
	if (first)
	{
		if(first == last)
		{
			delete first;
			first = last = nullptr;
		}
		else
		{
			TrivialStackNode<T>* previous = first->getPrevious();
			delete first;
			first = previous;
		}
		_size.store(_size.load(std::memory_order_acquire) - 1, std::memory_order_release);
	}
	uniqueLockMutex.unlock();
}

template<typename T>
void TrivialStack<T>::popBack()
{
	uniqueLockMutex.lock();
	if (first && last)
	{
		TrivialStackNode<T>* node = first;
		while (node->getPrevious() && node->getPrevious() != last)
			node = node->getPrevious();
		if(node)
		{
			TrivialStackNode<T>* previous = last->getPrevious();
			delete last;
			last = node == first ? nullptr : node;
			if( node )
			{
				last->setPrevious(previous);
			}
		}
		_size.store(_size.load(std::memory_order_acquire) - 1, std::memory_order_release);
	}
	uniqueLockMutex.unlock();
}

template<typename T>
T TrivialStack<T>::back()
{
	std::unique_lock<std::mutex> lck(uniqueLockMutex);
	return last->getData();
}

template<typename T>
inline void TrivialStack<T>::Clear()
{
	if (first)
	{
		TrivialStackNode<T>* node = first;
		TrivialStackNode<T>* previous = first->getPrevious();
		while (node)
		{
			delete node;
			node = previous;
			previous = node->getPrevious();
		}
	}
	if (last)
	{
		delete last;
	}
}
#endif // __TRIVIAL_STACK__

