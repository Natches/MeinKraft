#ifdef __ALLOCATOR_H__
#include <cassert>
#include "Exception/NullException.h"
template<class Type>
Type* default_allocator<Type>::allocate(const unsigned int SizeToAllocate)
{
#ifdef _DEBUG
	assert(SizeToAllocate > 0);
#endif // _DEBUG
	Type* NewPtr = nullptr;
	if (SizeToAllocate >= 0)
	{
		NewPtr = (Type*)calloc(SizeToAllocate, sizeof(Type));
		if (!NewPtr)
			throw NullException();
	}
	return NewPtr;
}

template<class Type>
void default_allocator<Type>::deallocate( Type*& ThingToDelete, const unsigned int n )noexcept
{
#ifdef _DEBUG
	assert(ThingToDelete);
#endif // _DEBUG
	(void)n;
	free(ThingToDelete);
	ThingToDelete = nullptr;
}

template<class Type>
template<class ...TypeArgs>
void default_allocator<Type>::construct(Type* ptr, TypeArgs&&... obj)noexcept
{
#ifdef _DEBUG
	assert(ptr);
#endif // _DEBUG
	new (ptr) Type(std::forward<TypeArgs>(obj)...);
}
#endif