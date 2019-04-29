#ifdef __cplusplus
extern "C++"
{
#ifndef __VECTOR_ITERATOR_H__
#define __VECTOR_ITERATOR_H__

#include "Iterator/IIterator.h"
	template<class Type, class Allocator = default_allocator<Type>>
	class vector;
	namespace ctn
	{
		template<class Type>
		class vectorIterator final : public IIterator<Type>
		{
		public:
#pragma region Constructor/Destructor
			vectorIterator();
			vectorIterator(const vector<Type>& v);
			vectorIterator(const Type* ptr);
			vectorIterator(const vectorIterator<Type>& v);
			vectorIterator(const vectorIterator<Type>&& v);
			~vectorIterator() = default;
#pragma endregion
		public:
#pragma region Operator
			vectorIterator<Type>&		operator=(const Type* ptr);
			vectorIterator<Type>&		operator=(const vectorIterator<Type>& v);
			vectorIterator<Type>&		operator=(const vectorIterator<Type>&& v);
			virtual Type&				operator*	()						const;
			vectorIterator<Type>&		operator++	();
			vectorIterator<Type>&		operator--	();
			vectorIterator<Type>		operator++	(int)					const;
			vectorIterator<Type>		operator--	(int)					const;
			vectorIterator<Type>&		operator+=	(int i);
			vectorIterator<Type>&		operator-=	(int i);
			vectorIterator<Type>		operator+	(int i)					const;
			vectorIterator<Type>		operator-	(int i)					const;
			virtual bool		operator==	(const IIterator& it)	const;
			virtual bool		operator!=	(const IIterator& it)	const;
			virtual bool		operator>	(const IIterator& it)	const;
			virtual bool		operator<	(const IIterator& it)	const;
			virtual bool		operator>=	(const IIterator& it)	const;
			virtual bool		operator<=	(const IIterator& it)	const;
#pragma endregion
		public:
#pragma region Getter
			virtual Type*				get()								const;
#pragma endregion

		private:
			Type* ptr;
		};
	}
#include "../Src/Iterator/VectorIterator.cpp"
#endif
}
#endif