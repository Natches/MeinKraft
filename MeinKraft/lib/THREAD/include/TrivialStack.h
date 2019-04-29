#ifndef __TRIVIAL_STACK__
#define __TRIVIAL_STACK__

#include <atomic>
#include <mutex>

template<typename T>
class TrivialStackNode
{
public:
	TrivialStackNode() = default;
	~TrivialStackNode() = default;

	inline TrivialStackNode<T>* getPrevious()const { return previous; }
	inline T getData()const { return data; }
	inline void setData(T _data) { data = _data; }
	inline void setPrevious(TrivialStackNode<T>* _previous) { previous = _previous; }

private:
	TrivialStackNode<T>* previous = nullptr;
	T data;
};

template<typename T>
class TrivialStack
{
public:
	TrivialStack() = default;
	~TrivialStack();

	void pop();
	void popBack();
	void push(T Data);
	inline T top(){ std::unique_lock<std::mutex> lck(uniqueLockMutex); return first->getData(); }
	T back();
	inline const unsigned int size() { return _size.load(std::memory_order_acquire); }
	inline const bool empty(){ return _size.load(std::memory_order_acquire)== 0; }
	inline std::mutex& getUniqueLockMutex() { return uniqueLockMutex; }
	void Clear();

private:
	TrivialStackNode<T>* first	= nullptr;
	TrivialStackNode<T>* last	= nullptr;
	std::mutex	uniqueLockMutex;
	std::atomic<unsigned int> _size = 0;
};

#include "TrivialStackimpl.h"

#endif // !__TRIVIAL_STACK__

