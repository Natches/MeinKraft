#pragma once


#ifdef __cplusplus
	extern "C"
	{
#endif

		typedef struct s_CVector CVector;
		struct s_CVector
		{
			void** nodes;
			unsigned int size;
			unsigned int capacity;
		};

		void			Init(const unsigned int spaceToReserve, CVector* vector);
		void			swap(unsigned int firstElem, unsigned int secondElem, CVector* vector);
#pragma region InsertThings
		void			pushBack(void* Data, CVector* vector);
		void			pushFront(void* Data, CVector* vector);
		void			insert(void* Data, unsigned int elementNumber, CVector* vector);
		void			insertFront(void* Data, unsigned int elementNumber, CVector* vector);
		void			insertBack(void* Data, unsigned int elementNumber, CVector* vector);
#pragma endregion


#pragma region EraseThings
		void*			popBack(CVector* vector);
		void*			popFront(CVector* vector);
		void*			popFrontOf(unsigned int elementNumber, CVector* vector);
		void*			popBackOf(unsigned int elementNumber, CVector* vector);
		void*			erase(void* const Data, CVector* vector);
		void*			eraseAllSameThing(void* const Data, CVector* vector);
		void			clear(CVector* vector);
		void			clearAll(CVector* vector);
#pragma endregion


#pragma region Reserve/Resize
		void			reserve(const unsigned int spaceToReserve, CVector* vector);
		void			shrinkToFit(CVector* vector);
		void			CheckCapacity(CVector* vector);
#pragma endregion


#pragma region FindThings
		void*			getNode(const unsigned int nb, CVector* vector);
		unsigned int	find(const void* const Data, CVector* vector);
#pragma endregion


#pragma region Convert

#pragma endregion
#ifdef __cplusplus
	}
#endif



#ifdef __cplusplus
	extern "C++"
	{
#ifndef __VECTOR_H__
#define __VECTOR_H__
#include <type_traits>
#include "Allocator/Allocator.h"
#include "Iterator/Vector_Iterator.h"
#define FORCEINLINE __forceinline
		namespace ctn
		{
			template<class Type, class Allocator = default_allocator<Type>>
			class vector final
			{
			public:
#pragma region Constructor/Destructor
				vector		()									noexcept;
				vector		(unsigned int SpaceToReserve);
				vector		(unsigned int arraySize, Type* arr) noexcept;
				vector		(const vector& v);
				vector		(vector&& v)						noexcept;
				~vector		()									noexcept;
#pragma endregion
			public:
#pragma region Operator
				vector<Type>&	operator=	(const vector<Type>& v);
				vector<Type>&	operator=	(vector<Type>&& v)				noexcept;
				bool			operator==	(const vector<Type>& v)const	noexcept;
				bool			operator!=	(const vector<Type>& v)const	noexcept;
#pragma endregion
			public:
#pragma region Getter
				FORCEINLINE Type*			data()		const noexcept { return _arr; };
				FORCEINLINE unsigned int	capacity()	const noexcept { return _capacity; };
				FORCEINLINE unsigned int	size()		const noexcept { return _size; };
				FORCEINLINE bool			empty()		const noexcept { return !(bool)_size; };

				FORCEINLINE vectorIterator<Type>			begin()							const noexcept { return &_arr[0]; };
				FORCEINLINE vectorIterator<Type>			end()							const noexcept { return &_arr[_size]; };
				FORCEINLINE const vectorIterator<Type>		getIteratorAt(unsigned int i)	const noexcept { return _arr + i; };

				FORCEINLINE Type& at(unsigned int i)				const noexcept { return _arr[i]; };
				FORCEINLINE Type& operator[](unsigned int Where)	const noexcept { return _arr[Where]; };
#pragma endregion
			public:
#pragma region Insert
				void insert(const Type& ObjectToInsert);
				void insert(const Type& ObjectToInsert, const vectorIterator<Type>& it);
				void insert(Type&& ObjectToInsert);
				void insert(Type&& ObjectToInsert, const vectorIterator<Type>& it);
				
				template<unsigned int duplicationCount>
				void insert(const Type& ObjectToInsert);
				template<unsigned int duplicationCount>
				void insert(const Type& ObjectToInsert, unsigned int Where);
				template<unsigned int duplicationCount>
				void insert(Type&& ObjectToInsert);
				template<unsigned int duplicationCount>
				void insert(Type&& ObjectToInsert, unsigned int Where);
#pragma endregion
			public:
#pragma region InsertAfter
				inline void insert_after(const Type& ObjectToInsert, const vectorIterator<Type>& it);
				inline void insert_after(Type&& ObjectToInsert, const vectorIterator<Type>& it);
				
				template<unsigned int duplicationCount>
				inline void insert_after(const Type& ObjectToInsert, unsigned int Where);
				template<unsigned int duplicationCount>
				inline void insert_after(Type&& ObjectToInsert, unsigned int Where);
#pragma endregion
			public:
#pragma region InsertBefore
				inline void insert_before(const Type& ObjectToInsert, const vectorIterator<Type>& it);
				inline void insert_before(Type&& ObjectToInsert, const vectorIterator<Type>& it);
				
				template<unsigned int duplicationCount>
				inline void insert_before(const Type& ObjectToInsert, unsigned int Where);
				template<unsigned int duplicationCount>
				inline void insert_before(Type&& ObjectToInsert, unsigned int Where);
#pragma endregion
			public:
#pragma region Push
				inline void push_front(const Type& ObjectToPushFront);
				inline void push_back(const Type& ObjectToPushBack);

				inline void push_front(Type&& ObjectToPushFront);
				void push_back(Type&& ObjectToPushBack);

				template<unsigned int duplicationCount>
				inline void push_front(const Type& ObjectToPushFront);
				template<unsigned int duplicationCount>
				inline void push_back(const Type& ObjectToPushBack);
				
				template<unsigned int duplicationCount>
				inline void push_front(Type&& ObjectToPushFront);
				template<unsigned int duplicationCount>
				inline void push_back(Type&& ObjectToPushBack);
#pragma endregion
			public:
#pragma region EmplaceAfter
				template<class...TypeArgs>
				void emplace_after		(const vectorIterator<Type>& it, TypeArgs&&...val);

				template<unsigned int duplicationCount, class...TypeArgs>
				void emplace_after(unsigned int Where, TypeArgs&&...val);

				template<class...TypeArgs>
				void emplace_after_varidic(unsigned int Where, const Type& value, TypeArgs&&...val);
				
				template<unsigned int duplicationCount, class...TypeArgs>
				void emplace_after_varidic(unsigned int Where ,const Type& value, TypeArgs&&...val);

			private:
#pragma region PrivateDefinition
				template<class...TypeArgs>
				void emplace_after_varidic_prealloc(unsigned int Where, const Type& value, TypeArgs&&...val) noexcept;
				
				template<unsigned int duplicationCount, class...TypeArgs>
				void emplace_after_varidic_prealloc(unsigned int Where, const Type& value, TypeArgs&&...val) noexcept;
				
				void emplace_after_varidic_prealloc() noexcept {};
				
				void emplace_after_varidic_prealloc(unsigned int Where) noexcept {};
				
				template<unsigned int duplicationCount>
				void emplace_after_varidic_prealloc(unsigned int Where) noexcept {};
#pragma endregion
#pragma endregion
			public:
#pragma region EmplaceBefore
				template<class...TypeArgs>
				void emplace_before(const vectorIterator<Type>& it, TypeArgs&&...val);
				
				template<unsigned int duplicationCount, class...TypeArgs>
				void emplace_before(unsigned int Where, TypeArgs&&...val);
				
				template<class...TypeArgs>
				void emplace_before_varidic(unsigned int Where, const Type& value, TypeArgs&&...val);
				
				template<unsigned int duplicationCount, class...TypeArgs>
				void emplace_before_varidic(unsigned int Where, const Type& value, TypeArgs&&...val);
			
			private:
#pragma region PrivateDefinition
				template<class...TypeArgs>
				void emplace_before_varidic_prealloc(unsigned int Where, const Type& value, TypeArgs&&...val) noexcept;
				
				template<unsigned int duplicationCount, class...TypeArgs>
				void emplace_before_varidic_prealloc(unsigned int Where, const Type& value, TypeArgs&&...val) noexcept;
				
				void emplace_before_varidic_prealloc() noexcept {};
				
				void emplace_before_varidic_prealloc(unsigned int Where) noexcept {};
				
				template<unsigned int duplicationCount>
				void emplace_before_varidic_prealloc(unsigned int Where) noexcept {};
#pragma endregion
#pragma endregion
			public:
#pragma region EmplaceFrontBack
				template<class...TypeArgs>
				void emplace_back(TypeArgs&&...val);
				template<class...TypeArgs>
				void emplace_back_no_except(TypeArgs&&...val)noexcept;
				template<class...TypeArgs>
				void emplace_front(TypeArgs&&...val);

				template<unsigned int duplicationCount, class...TypeArgs>
				void emplace_back(TypeArgs&&...val);
				template<unsigned int duplicationCount, class...TypeArgs>
				void emplace_front(TypeArgs&&...val);

				template<class...TypeArgs>
				void emplace_back_varidic(const Type& value, TypeArgs&&...val);
				template<class...TypeArgs>
				void emplace_front_varidic(const Type& value, TypeArgs&&...val);

				template<unsigned int duplicationCount, class...TypeArgs>
				void emplace_back_varidic(const Type& value, TypeArgs&&...val);
				template<unsigned int duplicationCount, class...TypeArgs>
				void emplace_front_varidic(const Type& value, TypeArgs&&...val);
			
			private:
#pragma region PrivateDefinition
				template<class...TypeArgs>
				void emplace_back_varidic_prealloc(const Type& value, TypeArgs&&...val) noexcept;
				
				template<class...TypeArgs>
				void emplace_front_varidic_prealloc(const Type& value, TypeArgs&&...val)noexcept;
				
				template<unsigned int duplicationCount, class...TypeArgs>
				void emplace_back_varidic_prealloc(unsigned int index, const Type& value, TypeArgs&&...val)noexcept;
				
				template<unsigned int duplicationCount, class...TypeArgs>
				void emplace_front_varidic_prealloc(const Type& value, TypeArgs&&...val)noexcept;
				
				void emplace_back_varidic_prealloc()noexcept {};
				
				void emplace_front_varidic_prealloc()noexcept {};
				
				template<unsigned int duplicationCount>
				void emplace_back_varidic_prealloc(unsigned int index)noexcept {};
				
				template<unsigned int duplicationCount>
				void emplace_front_varidic_prealloc()noexcept {};
#pragma endregion
#pragma endregion
			public:
#pragma region EraseThings
				template<bool DeleteAuto = false>
				void pop_front();
				template<bool DeleteAuto = false>
				void pop_back();
				template<bool DeleteAuto = false>
				void erase(unsigned int Where);
				template<bool DeleteAuto = false>
				void erase(const vectorIterator<Type>& it);
				template<bool DeleteAuto = false>
				void erase_range(const unsigned int& begin, const unsigned int& end);
				template<bool DeleteAuto = false>
				void erase_range(const vectorIterator<Type>& begin, const vectorIterator<Type>& end);
				template<bool DeleteAuto = false>
				void remove(unsigned int Where) noexcept;
				template<bool DeleteAuto = false>
				void remove(const vectorIterator<Type>& it)noexcept;
				template<bool DeleteAuto = false>
				void remove_range(const unsigned int& begin, const unsigned int& end)noexcept;
				template<bool DeleteAuto = false>
				void remove_range(const vectorIterator<Type>& begin, const vectorIterator<Type>& end)noexcept;
				template<bool DeleteAuto = false>
				void clear()noexcept;
#pragma endregion
			public:
#pragma region Reserve/Resize
				void reserve(const unsigned int How);
				void resize(const unsigned int NewSize);
				void shrink_to_fit();
#pragma endregion

			public:
#pragma region FindThings
				template<bool iterator = true>
				const auto find		(const Type& What)const noexcept;
				template<bool iterator = true>
				const auto operator[]	(const Type& What)const noexcept { return find<iterator>(What); };
				template<bool iterator = true>
				const auto find		(const vectorIterator<Type>& begin, const vectorIterator<Type>& end, const Type& What)const noexcept;
				template<bool iterator = true>
				const auto operator[]	(const vectorIterator<Type>& What)const noexcept { return find<iterator>(*What); };
				Type find_maximum();
				Type find_minimum();
#pragma endregion
			public:
#pragma region Convert
				vector<Type> to_vector() { return *this; };
#pragma endregion
			public:
#pragma region Copy
				void copy(const vector<Type>& toCopy);
#pragma endregion
			private:
#pragma region PrivateFunc
				void CheckCapacity();
				inline void deletePtr(void* ptr)noexcept { delete ptr; };
				template<class ...TypeArgs>
				void construct(Type* ptr, TypeArgs&&... obj)noexcept;
				template<class ...TypeArgs>
				void construct_range(Type* begin, Type* end, TypeArgs&&... obj) noexcept;
				void reallocExactly(unsigned int i);
				bool inrange(unsigned int i)const noexcept { return i >= 0 && i <= _size; };
				bool inrange(Type* ptr)const noexcept { return ptr >= _arr && ptr <= _arr + _size; };
#pragma endregion
			private:
#pragma region PrivateVar
				Type* _arr;
				unsigned int _size = 0u;
				unsigned int _capacity = 0u;
#pragma endregion
			};
}

#include "../Src/Vector/Vector.cpp"
#undef FORCEINLINE
#endif // !__VECTOR_H__
	}
#endif
