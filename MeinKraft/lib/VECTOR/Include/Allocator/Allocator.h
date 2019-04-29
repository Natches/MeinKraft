#pragma once
#ifdef __cplusplus
extern "C++"
{
#ifndef __ALLOCATOR_H__
#define __ALLOCATOR_H__
	template<class Type>
	class default_allocator final
	{
	public:
		default_allocator() = default;
		default_allocator(const default_allocator&) = default;
		~default_allocator() = default;

		constexpr const bool operator!=(const default_allocator&)const { return false; }
		constexpr const bool operator==(const default_allocator&)const { return true; }

		Type* allocate(const unsigned int SizeToAllocate);
		inline void deallocate(Type*& ThingToDelete, const unsigned int n = 0u)noexcept;
		template<class... TypeArgs>
		void construct(Type* ptr, TypeArgs&&... obj)noexcept;
	};
#endif
#include "../src/Allocator/Allocator.cpp"
}
#endif