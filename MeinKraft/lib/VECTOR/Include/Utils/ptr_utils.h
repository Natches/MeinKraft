#pragma once

template<class T>
unsigned int range(T first, T last) { return (last - first); }

template<class T>
T* realloc(T*& ptr, unsigned int oldSize, unsigned int newSize)
{
	T* newPtr = (T*)calloc(newSize, sizeof(T));
	memcpy(newPtr, ptr, sizeof(T) * oldSize);
	free(ptr);
	ptr = newPtr;
	return ptr;
};