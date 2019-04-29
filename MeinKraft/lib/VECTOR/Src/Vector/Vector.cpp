#ifdef __VECTOR_H__
#include <memory>
#include <cassert>
#include "Utils/ptr_utils.h"
#include "Warning/Warning.h"
#include "Exception/NullException.h"
#include "Utils/constexpr_utils.h"
namespace ctn
{
	//TODO : more exception, some assert elsewhere, secure iterator, secure more vector ?, spe bool
#pragma region Constructor/Destructor
	template<class Type, class Allocator>
	vector<Type, Allocator >::vector()noexcept
		: _arr(nullptr), _capacity(0u)
	{
		_size = _capacity;
	}

	template<class Type, class Allocator >
	vector<Type, Allocator >::vector(unsigned int SpaceToReserve)
		: _capacity(SpaceToReserve),
		_size(0u)
	{
		try
		{
			_arr = Allocator().allocate(v._capacity);
		}
		catch (NullException& n)
		{
			std::cout << n.what() << std::endl;
			throw n;
		}
	}

	template<class Type, class Allocator >
	vector<Type, Allocator >::vector(unsigned int arraySize, Type* arr) noexcept
		: _size(arraySize), _capacity(arraySize), _arr(arr)
	{
#ifdef _DEBUG
		assert(arraySize > 0 && arr);
#endif // _DEBUG
	}

	template<class Type, class Allocator >
	vector<Type, Allocator >::vector(const vector& v)
	{
		if (v._capacity)
		{
			try
			{
				_arr = Allocator().allocate(v._capacity);
			}
			catch (NullException& n)
			{
				std::cout << n.what() << std::endl;
				throw n;
			}
		}
		else
			_arr = nullptr;
		_size = v._size;
		_capacity = v._capacity;
		memcpy(_arr, v._arr, v._size * sizeof(Type));
	}

	template<class Type, class Allocator >
	vector<Type, Allocator >::vector(vector&& v)noexcept
	{
		_arr = v._arr;
		_size = v._size;
		_capacity = v._capacity;
		v._size = v._capacity = 0;
		v._arr = nullptr;
	}

	template<class Type, class Allocator >
	vector<Type, Allocator >::~vector()noexcept
	{
		if (_arr)
		{
			if constexpr (!std::is_trivially_destructible_v<Type>)
			{
				for (auto& i : *this)
					i.~Type();
			}
			Allocator().deallocate(_arr, _capacity);
		}
	}
#pragma endregion

#pragma region Operator
	template<class Type, class Allocator >
	vector<Type>& vector<Type, Allocator >::operator=(const vector<Type>& v)
	{
		if (v._capacity)
		{
			try
			{
				_arr = Allocator().allocate(v._capacity);
			}
			catch (NullException& n)
			{
				std::cout << n.what() << std::endl;
				throw n;
			}
		}
		else
			_arr = nullptr;
		_size = v._size;
		_capacity = v._capacity;
		memcpy(_arr, v._arr, v._size * sizeof(Type));
		
		return *this;
	}

	template<class Type, class Allocator >
	vector<Type>& vector<Type, Allocator >::operator=(vector<Type>&& v)noexcept
	{
		_arr = v._arr;
		_size = v._size;
		_capacity = v._capacity;
		v._size = v._capacity = 0;
		v._arr = nullptr;
		return *this;
	}

	template<class Type, class Allocator >
	bool vector<Type, Allocator >::operator==(const vector<Type>& v)const noexcept
	{
		if (v._arr == _arr)
			return true;
		else if( v._size == _size)
		{
			bool same = true;
			for (unsigned int i = 0; i ^ _size; ++i)
			{
				if (v[i] != _arr[i])
				{
					same = false;
					break;
				}
			}
			return same;
		}
		else
			return false;
	}

	template<class Type, class Allocator >
	bool ctn::vector<Type, Allocator >::operator!=(const vector<Type>& v) const noexcept
	{
		return !(*this == v);
	}
#pragma endregion

#pragma region Insert
	template<class Type, class Allocator >
	void vector<Type, Allocator >::insert(const Type& ObjectToInsert)
	{
		static_assert(std::is_copy_constructible_v<Type>, "Cannot copy this Type, try to move it by using std::move()");
		try
		{
			emplace_back(ObjectToInsert);
		}
		catch (NullException& n)
		{
			std::cout << n.what() << std::endl;
			throw n;
		}
	}

	template<class Type, class Allocator >
	void vector<Type, Allocator >::insert(const Type& ObjectToInsert, const vectorIterator<Type>& it)
	{
		static_assert(std::is_copy_constructible_v<Type>, "Cannot copy this Type, try to move it by using std::move()");
		try
		{
			emplace_after(ObjectToInsert, it);
		}
		catch (NullException& n)
		{
			std::cout << n.what() << std::endl;
			throw n;
		}
	}

	template<class Type, class Allocator >
	void vector<Type, Allocator >::insert(Type&& ObjectToInsert)
	{
		try
		{
			emplace_back(std::move(ObjectToInsert));
		}
		catch (NullException& n)
		{
			std::cout << n.what() << std::endl;
			throw n;
		}
	}

	template<class Type, class Allocator >
	void vector<Type, Allocator >::insert(Type&& ObjectToInsert, const vectorIterator<Type>& it)
	{
		try
		{
			emplace_after(std::move(ObjectToInsert), it);
		}
		catch (NullException& n)
		{
			std::cout << n.what() << std::endl;
			throw n;
		}
	}

	template<class Type, class Allocator >
	template<unsigned int duplicationCount>
	void vector<Type, Allocator >::insert(const Type& ObjectToInsert)
	{
		static_assert(std::is_copy_constructible_v<Type>, "Cannot copy this Type, try to move it by using std::move()");
		static_assert(duplicationCount, _ASSERT_MESSAGE_CTN1);
		if constexpr(duplicationCount > 0u)
		{
			try
			{
				emplace_back<duplicationCount>(ObjectToInsert);
			}
			catch (NullException& n)
			{
				std::cout << n.what() << std::endl;
				throw n;
			}
		}
	}

	template<class Type, class Allocator >
	template<unsigned int duplicationCount>
	void vector<Type, Allocator >::insert(const Type& ObjectToInsert, unsigned int Where)
	{
		static_assert(std::is_copy_constructible_v<Type>, "Cannot copy this Type, try to move it by using std::move()");
		static_assert(duplicationCount, _ASSERT_MESSAGE_CTN1);
#ifdef _DEBUG
		assert(outofrange(Where));
#endif // _DEBUG
		if constexpr(duplicationCount > 0u)
		{
			try
			{
				emplace_after<duplicationCount>(Where, ObjectToInsert);
			}
			catch (NullException& n)
			{
				std::cout << n.what() << std::endl;
				throw n;
			}
		}
	}

	template<class Type, class Allocator >
	template<unsigned int duplicationCount>
	void vector<Type, Allocator >::insert(Type&& ObjectToInsert)
	{
		static_assert(duplicationCount, _ASSERT_MESSAGE_CTN1);
		if constexpr(duplicationCount > 0u)
		{
			try
			{
				emplace_back<duplicationCount>(ObjectToInsert);
			}
			catch (NullException& n)
			{
				std::cout << n.what() << std::endl;
				throw n;
			}
		}
	}

	template<class Type, class Allocator >
	template<unsigned int duplicationCount>
	void vector<Type, Allocator >::insert(Type&& ObjectToInsert, unsigned int Where)
	{
		static_assert(duplicationCount, _ASSERT_MESSAGE_CTN1);
#ifdef _DEBUG
		assert(outofrange(Where));
#endif // _DEBUG
		if constexpr(duplicationCount > 0u)
		{
			try
			{
				emplace_after<duplicationCount>(Where, ObjectToInsert);
			}
			catch (NullException& n)
			{
				std::cout << n.what() << std::endl;
				throw n;
			}
		}
	}
#pragma endregion

#pragma region InsertAfter
	template<class Type, class Allocator >
	void vector<Type, Allocator >::insert_after(const Type& ObjectToInsert, const vectorIterator<Type>& it)
	{
		static_assert(std::is_copy_constructible_v<Type>, "Cannot copy this Type, try to move it by using std::move()");
		try
		{
			emplace_after(ObjectToInsert, find(it));
		}
		catch (NullException& n)
		{
			std::cout << n.what() << std::endl;
			throw n;
		}
	}

	template<class Type, class Allocator >
	void vector<Type, Allocator >::insert_after(Type&& ObjectToInsert, const vectorIterator<Type>& it)
	{
		try
		{
			emplace_after(std::move(ObjectToInsert), find(it));
		}
		catch (NullException& n)
		{
			std::cout << n.what() << std::endl;
			throw n;
		}
	}


	template<class Type, class Allocator >
	template<unsigned int duplicationCount>
	void vector<Type, Allocator >::insert_after(const Type & ObjectToInsert, unsigned int Where)
	{
		static_assert(std::is_copy_constructible_v<Type>, "Cannot copy this Type, try to move it by using std::move()");
		static_assert(duplicationCount, _ASSERT_MESSAGE_CTN1);
#ifdef _DEBUG
		assert(outofrange(Where));
#endif // _DEBUG
		if constexpr(duplicationCount > 0u)
		{
			try
			{
				emplace_after<duplicationCount>(Where, ObjectToInsert);
			}
			catch (NullException& n)
			{
				std::cout << n.what() << std::endl;
				throw n;
			}
		}
	}

	template<class Type, class Allocator >
	template<unsigned int duplicationCount>
	void vector<Type, Allocator >::insert_after(Type&& ObjectToInsert, unsigned int Where)
	{
		static_assert(duplicationCount, _ASSERT_MESSAGE_CTN1);
#ifdef _DEBUG
		assert(outofrange(Where));
#endif // _DEBUG
		if constexpr(duplicationCount > 0u)
		{
			try
			{
				emplace_after<duplicationCount>(Where, ObjectToInsert);
			}
			catch (NullException& n)
			{
				std::cout << n.what() << std::endl;
				throw n;
			}
		}
	}
#pragma endregion

#pragma region InsertBefore
	template<class Type, class Allocator >
	void vector<Type, Allocator >::insert_before(const Type& ObjectToInsert, const vectorIterator<Type>& it)
	{
		static_assert(std::is_copy_constructible_v<Type>, "Cannot copy this Type, try to move it by using std::move()");
		try
		{
			emplace_before(ObjectToInsert, find(it));
		}
		catch (NullException& n)
		{
			std::cout << n.what() << std::endl;
			throw n;
		}
	}

	template<class Type, class Allocator >
	void vector<Type, Allocator >::insert_before(Type&& ObjectToInsert, const vectorIterator<Type>& it)
	{
		try
		{
			emplace_before(std::move(ObjectToInsert), find(it));
		}
		catch (NullException& n)
		{
			std::cout << n.what() << std::endl;
			throw n;
		}
	}

	template<class Type, class Allocator >
	template<unsigned int duplicationCount>
	void vector<Type, Allocator >::insert_before(const Type& ObjectToInsert, unsigned int Where)
	{
		static_assert(std::is_copy_constructible_v<Type>, "Cannot copy this Type, try to move it by using std::move()");
		static_assert(duplicationCount, _ASSERT_MESSAGE_CTN1);
#ifdef _DEBUG
		assert(outofrange(Where));
#endif // _DEBUG
		if constexpr(duplicationCount > 0u)
		{
			try
			{
				emplace_before<duplicationCount>(Where, ObjectToInsert);
			}
			catch (NullException& n)
			{
				std::cout << n.what() << std::endl;
				throw n;
			}
		}
	}

	template<class Type, class Allocator >
	template<unsigned int duplicationCount>
	void vector<Type, Allocator >::insert_before(Type&& ObjectToInsert, unsigned int Where)
	{
		static_assert(std::is_copy_constructible_v<Type>, "Cannot copy this Type, try to move it by using std::move()");
		static_assert(duplicationCount, _ASSERT_MESSAGE_CTN1);
#ifdef _DEBUG
		assert(outofrange(Where));
#endif // _DEBUG
		if constexpr(duplicationCount > 0u)
		{
			try
			{
				emplace_before<duplicationCount>(Where, ObjectToInsert);
			}
			catch (NullException& n)
			{
				std::cout << n.what() << std::endl;
				throw n;
			}
		}
	}
#pragma endregion

#pragma region Push
	template<class Type, class Allocator >
	void ctn::vector<Type, Allocator >::push_front(const Type& ObjectToPushFront)
	{
		static_assert(std::is_copy_constructible_v<Type>, "Cannot copy this Type, try to move it by using std::move()");
		try
		{
			emplace_front(ObjectToPushFront);
		}
		catch (NullException& n)
		{
			std::cout << n.what() << std::endl;
			throw n;
		}
	}

	template<class Type, class Allocator >
	void vector<Type, Allocator >::push_back(const Type& ObjectToPushBack)
	{
		static_assert(std::is_copy_constructible_v<Type>, "Cannot copy this Type, try to move it by using std::move()");
		if (_capacity == _size)
		{
			try
			{
				emplace_back(ObjectToPushBack);
			}
			catch (NullException& n)
			{
				std::cout << n.what() << std::endl;
				throw n;
			}
		}
		else
			emplace_back_no_except(ObjectToPushBack);
	}

	template<class Type, class Allocator >
	void vector<Type, Allocator >::push_front(Type&& ObjectToPushFront)
	{
		try
		{
			emplace_front(std::move(ObjectToPushFront));
		}
		catch (NullException& n)
		{
			std::cout << n.what() << std::endl;
			throw n;
		}
	}

	template<class Type, class Allocator >
	void vector<Type, Allocator >::push_back(Type&& ObjectToPushBack)
	{
		if (_capacity == _size)
		{
			try
			{
				emplace_back(std::move(ObjectToPushBack));
			}
			catch (NullException& n)
			{
				std::cout << n.what() << std::endl;
				throw n;
			}
		}
		else
			emplace_back_no_except(std::move(ObjectToPushBack));
	}

	template<class Type, class Allocator >
	template<unsigned int duplicationCount>
	void vector<Type, Allocator >::push_front(const Type& ObjectToPushFront)
	{
		static_assert(std::is_copy_constructible_v<Type>, "Cannot copy this Type, try to move it by using std::move()");
		static_assert(duplicationCount, _ASSERT_MESSAGE_CTN1);
		if constexpr(duplicationCount > 0u)
		{
			try
			{
				emplace_front<duplicationCount>(ObjectToPushFront);
			}
			catch (NullException& n)
			{
				std::cout << n.what() << std::endl;
				throw n;
			}
		}
	}

	template<class Type, class Allocator >
	template<unsigned int duplicationCount>
	void vector<Type, Allocator >::push_back(const Type& ObjectToPushBack)
	{
		static_assert(std::is_copy_constructible_v<Type>, "Cannot copy this Type, try to move it by using std::move()");
		static_assert(duplicationCount, _ASSERT_MESSAGE_CTN1);
		if constexpr(duplicationCount > 0u)
		{
			try
			{
				emplace_back<duplicationCount>(ObjectToPushBack);
			}
			catch (NullException& n)
			{
				std::cout << n.what() << std::endl;
				throw n;
			}
		}
	}

	template<class Type, class Allocator >
	template<unsigned int duplicationCount>
	void vector<Type, Allocator >::push_front(Type&& ObjectToPushFront)
	{
		static_assert(duplicationCount, _ASSERT_MESSAGE_CTN1);
		if constexpr(duplicationCount > 0u)
		{
			try
			{
				emplace_front<duplicationCount>(ObjectToPushFront);
			}
			catch (NullException& n)
			{
				std::cout << n.what() << std::endl;
				throw n;
			}
		}
	}

	template<class Type, class Allocator >
	template<unsigned int duplicationCount>
	void vector<Type, Allocator >::push_back(Type&& ObjectToPushBack)
	{
		static_assert(duplicationCount, _ASSERT_MESSAGE_CTN1);
		if constexpr(duplicationCount > 0u)
		{
			try
			{
				emplace_back<duplicationCount>(ObjectToPushBack);
			}
			catch (NullException& n)
			{
				std::cout << n.what() << std::endl;
				throw n;
			}
		}
	}

#pragma endregion

#pragma region EmplaceAfter
	template<class Type, class Allocator >
	template<class ...TypeArgs>
	void vector<Type, Allocator >::emplace_after(const vectorIterator<Type>& it, TypeArgs&& ...val)
	{
		if (it < --end())
		{
			if (_capacity == _size)
			{
				try
				{
					CheckCapacity();
				}
				catch (NullException& n)
				{
					std::cout << n.what() << std::endl;
					throw n;
				}
			}
			unsigned int Where = (it.get() - _arr);
#ifdef _DEBUG
			assert(outofrange(Where));
#endif // _DEBUG
			memcpy(&_arr[Where + 2], &_arr[Where + 1], (_size - Where - 1) * sizeof(Type));
			construct(&_arr[Where + 1], std::forward<TypeArgs>(val)...);
			++_size;
		}
		else
		{
			try
			{
				emplace_back(std::forward<TypeArgs>(val)...);
			}
			catch (NullException& n)
			{
				std::cout << n.what() << std::endl;
				throw n;
			}
		}
	}

	template<class Type, class Allocator>
	template<unsigned int duplicationCount, class ...TypeArgs>
	void ctn::vector<Type, Allocator>::emplace_after(unsigned int Where, TypeArgs && ...val)
	{
		static_assert(duplicationCount, _ASSERT_MESSAGE_CTN1);
#ifdef _DEBUG		
		assert(outofrange(Where));
#endif // _DEBUG
		if constexpr (duplicationCount > 0u)
		{
			if ((int)Where < (int)_size - 1)
			{
				try
				{
					reallocExactly(duplicationCount);
				}
				catch (NullException& n)
				{
					std::cout << n.what() << std::endl;
					throw n;
				}
				memcpy(&_arr[Where + 2 + duplicationCount - 1], &_arr[Where + 1], (_size - Where - 1) * sizeof(Type));
				construct_range(&_arr[Where + 1], &_arr[Where + 1 + duplicationCount], value);
				_size += nb;
			}
			else
			{
				try
				{
					emplace_back<duplicationCount>(value, val...);
				}
				catch (NullException& n)
				{
					std::cout << n.what() << std::endl;
					throw n;
				}
			}
		}
	}

	template<class Type, class Allocator >
	template<class ...TypeArgs>
	void vector<Type, Allocator >::emplace_after_varidic(unsigned int Where, const Type& value, TypeArgs&&...val)
	{
#ifdef _DEBUG
		assert(outofrange(Where));
#endif // _DEBUG
		if ((int)Where < (int)_size - 1)
		{
			try
			{
				reallocExactly(sizeof...(val)+1);
			}
			catch (NullException& n)
			{
				std::cout << n.what() << std::endl;
				throw n;
			}
			memcpy(&_arr[Where + 2 + sizeof...(val)], &_arr[Where + 1], (_size - Where - 1) * sizeof(Type));
			construct(&_arr[Where + 1], value);
			if constexpr (sizeof...(val))
				emplace_after_varidic_prealloc(++Where, val...);
			_size += sizeof...(val)+1;
		}
		else
		{
			try
			{
				emplace_back_varidic(value, val...);
			}
			catch (NullException& n)
			{
				std::cout << n.what() << std::endl;
				throw n;
			}
		}
	}

	template<class Type, class Allocator >
	template<unsigned int duplicationCount, class ...TypeArgs>
	void vector<Type, Allocator >::emplace_after_varidic(unsigned int Where, const Type & value, TypeArgs && ...val)
	{
		static_assert(duplicationCount, _ASSERT_MESSAGE_CTN1);
#ifdef _DEBUG
		assert(outofrange(Where));
#endif // _DEBUG
		if constexpr (duplicationCount > 0u)
		{
			if ((int)Where < (int)_size - 1)
			{
				constexpr int nb = duplicationCount * (sizeof...(val)+1);
				try
				{
					reallocExactly(nb);
				}
				catch (NullException& n)
				{
					std::cout << n.what() << std::endl;
					throw n;
				}
				memcpy(&_arr[Where + 2 + nb - 1], &_arr[Where + 1], (_size - Where - 1) * sizeof(Type));
				construct_range(&_arr[Where + 1], &_arr[Where + 1 + duplicationCount], value);
				if constexpr (sizeof...(val))
					emplace_after_varidic_prealloc<duplicationCount>(Where, val...);
				_size += nb;
			}
			else
			{
				try
				{
					emplace_back_varidic<duplicationCount>(value, val...);
				}
				catch (NullException& n)
				{
					std::cout << n.what() << std::endl;
					throw n;
				}
			}
		}
	}
#pragma region PrivateDefinition
	template<class Type, class Allocator >
	template<class ...TypeArgs>
	void vector<Type, Allocator >::emplace_after_varidic_prealloc(unsigned int Where, const Type& value, TypeArgs && ...val)noexcept
	{
#ifdef _DEBUG
		assert(outofrange(Where));
#endif // _DEBUG
		construct(&_arr[Where + 1], value);
		if constexpr (sizeof...(val))
			emplace_after_varidic_prealloc(++Where, val...);
	}

	template<class Type, class Allocator >
	template<unsigned int duplicationCount, class ...TypeArgs>
	void vector<Type, Allocator >::emplace_after_varidic_prealloc(unsigned int Where, const Type & value, TypeArgs && ...val)noexcept
	{
		static_assert(duplicationCount, _ASSERT_MESSAGE_CTN1);
#ifdef _DEBUG
		assert(outofrange(Where));
#endif // _DEBUG
		if constexpr (duplicationCount > 0u)
		{
			constexpr unsigned int nb = (sizeof...(val)+1) * duplicationCount;
			construct_range(&_arr[Where + 1 + nb], &_arr[Where + 1 + duplicationCount + nb], value);
			if constexpr (sizeof...(val))
				emplace_after_varidic_prealloc<duplicationCount>(Where, val...);
		}
	}
#pragma endregion
#pragma endregion

#pragma region EmplaceBefore
	template<class Type, class Allocator >
	template<class ...TypeArgs>
	void vector<Type, Allocator >::emplace_before(const vectorIterator<Type>& it, TypeArgs&&... val)
	{
		if (_capacity == _size)
		{
			try
			{
				CheckCapacity();
			}
			catch (NullException& n)
			{
				std::cout << n.what() << std::endl;
				throw n;
			}
		}
		unsigned int Where = (it.get() - begin().get());
#ifdef _DEBUG
		assert(outofrange(Where));
#endif // _DEBUG
		if (_size >= 1)
			memcpy(&_arr[Where + 1], &_arr[Where], (_size - Where) * sizeof(Type));
		construct(&_arr[Where], std::forward<TypeArgs>(val)...);
		++_size;
	}

	template<class Type, class Allocator>
	template<unsigned int duplicationCount, class ...TypeArgs>
	void ctn::vector<Type, Allocator>::emplace_before(unsigned int Where, TypeArgs && ...val)
	{
#ifdef _DEBUG
		assert(outofrange(Where));
#endif // _DEBUG
		static_assert(duplicationCount, _ASSERT_MESSAGE_CTN1);
		if constexpr (duplicationCount > 0u)
		{
			try
			{
				reallocExactly(duplicationCount);
			}
			catch (NullException& n)
			{
				std::cout << n.what() << std::endl;
				throw n;
			}
			if (_size >= 1)
				memcpy(&_arr[Where + duplicationCount], &_arr[Where], (_size - Where) * sizeof(Type));
			construct_range(&_arr[Where], &_arr[Where + duplicationCount], value);
			_size += duplicationCount;
		}
	}

	template<class Type, class Allocator >
	template<class ...TypeArgs>
	void vector<Type, Allocator >::emplace_before_varidic(unsigned int Where, const Type& value, TypeArgs&&...val)
	{
#ifdef _DEBUG
		assert(outofrange(Where));
#endif // _DEBUG
		try
		{
			reallocExactly(sizeof...(val)+1);
		}
		catch (NullException& n)
		{
			std::cout << n.what() << std::endl;
			throw n;
		}
		if (_size >= 1)
			memcpy(&_arr[Where + 1 + sizeof...(val)], &_arr[Where], (_size - Where) * sizeof(Type));
		construct(&_arr[Where], value);
		if constexpr (sizeof...(val))
			emplace_before_varidic_prealloc(++Where, val...);
		_size += sizeof...(val)+1;
	}

	template<class Type, class Allocator >
	template<unsigned int duplicationCount, class ...TypeArgs>
	void vector<Type, Allocator >::emplace_before_varidic(unsigned int Where, const Type & value, TypeArgs && ...val)
	{
#ifdef _DEBUG
		assert(outofrange(Where));
#endif // _DEBUG
		static_assert(duplicationCount, _ASSERT_MESSAGE_CTN1);
		if constexpr (duplicationCount > 0u)
		{
			constexpr int nb = duplicationCount * (sizeof...(val)+1);
			try
			{
				reallocExactly(nb);
			}
			catch (NullException& n)
			{
				std::cout << n.what() << std::endl;
				throw n;
			}
			if (_size >= 1)
				memcpy(&_arr[Where + nb], &_arr[Where], (_size - Where) * sizeof(Type));
			construct_range(&_arr[Where], &_arr[Where + duplicationCount], value);
			if constexpr (sizeof...(val))
				emplace_before_varidic_prealloc<duplicationCount>(1, val...);
			_size += nb;
		}
	}
#pragma region PrivateDefinition
	template<class Type, class Allocator >
	template<class ...TypeArgs>
	void vector<Type, Allocator >::emplace_before_varidic_prealloc(unsigned int Where, const Type & value, TypeArgs && ...val)noexcept
	{
#ifdef _DEBUG
		assert(outofrange(Where));
#endif // _DEBUG
		construct(&_arr[Where], value);
		if constexpr (sizeof...(val))
			emplace_before_varidic_prealloc(++Where, val...);
	}

	template<class Type, class Allocator >
	template<unsigned int duplicationCount, class ...TypeArgs>
	void vector<Type, Allocator >::emplace_before_varidic_prealloc( unsigned int Where, const Type & value, TypeArgs && ...val)noexcept
	{
		static_assert(duplicationCount, _ASSERT_MESSAGE_CTN1);
#ifdef _DEBUG
		assert(outofrange(Where));
#endif // _DEBUG
		if constexpr (duplicationCount > 0u)
		{
			construct_range(&_arr[(Where * duplicationCount) + 1], &_arr[((Where + 1) * duplicationCount ) + 1 ], value);
			if constexpr (sizeof...(val))
				emplace_before_varidic_prealloc<duplicationCount>(++Where, val...);
		}
	}
#pragma endregion
#pragma endregion

#pragma region EmplaceFrontBack
	template<class Type, class Allocator >
	template<class ...TypeArgs>
	void vector<Type, Allocator >::emplace_back(TypeArgs&& ...val)
	{	
		if (_capacity == _size)
		{
			try
			{
				CheckCapacity();
			}
			catch (NullException& n)
			{
				std::cout << n.what() << std::endl;
				throw n;
			}
		}
		construct(&_arr[_size++], std::forward<TypeArgs>(val)...);
	}

	template<class Type, class Allocator >
	template<class ...TypeArgs>
	void vector<Type, Allocator >::emplace_back_no_except(TypeArgs&& ...val)noexcept
	{
		construct(&_arr[_size++], std::forward<TypeArgs>(val)...);
	}

	template<class Type, class Allocator >
	template<class ...TypeArgs>
	void vector<Type, Allocator >::emplace_front(TypeArgs&& ...val)
	{
		if (_capacity == _size)
		{
			try
			{
				CheckCapacity();
			}
			catch (NullException& n)
			{
				std::cout << n.what() << std::endl;
				throw n;
			}
		}
		memcpy(&_arr[1], &_arr[0], _size++ * sizeof(Type));
		construct(&_arr[0], std::forward<TypeArgs>(val)...);

	}

	template<class Type, class Allocator >
	template<unsigned int duplicationCount, class ...TypeArgs>
	void vector<Type, Allocator >::emplace_back(TypeArgs&& ...val)
	{
		static_assert(duplicationCount, _ASSERT_MESSAGE_CTN1);
		if constexpr(duplicationCount > 0u)
		{
			try
			{
				reallocExactly(duplicationCount);
			}
			catch (NullException& n)
			{
				std::cout << n.what() << std::endl;
				throw n;
			}
			construct_range(&_arr[_size], &_arr[_size + duplicationCount], std::forward<TypeArgs>(val)...);
			_size += duplicationCount;
		}
	}

	template<class Type, class Allocator >
	template<unsigned int duplicationCount, class ...TypeArgs>
	void vector<Type, Allocator >::emplace_front(TypeArgs&& ...val)
	{
		static_assert(duplicationCount, _ASSERT_MESSAGE_CTN1);
		if constexpr(duplicationCount > 0u)
		{
			try
			{
				reallocExactly(duplicationCount);
			}
			catch (NullException& n)
			{
				std::cout << n.what() << std::endl;
				throw n;
			}
			memcpy(&_arr[duplicationCount], &_arr[0], _size * sizeof(Type));
			construct_range(&_arr[0], &_arr[duplicationCount], std::forward<TypeArgs>(val)...);
			_size += duplicationCount;
		}
	}

	template<class Type, class Allocator >
	template<class ...TypeArgs>
	void vector<Type, Allocator >::emplace_back_varidic(const Type& value, TypeArgs&&...val)
	{
		try
		{
			reallocExactly(sizeof...(val)+1);
		}
		catch (NullException& n)
		{
			std::cout << n.what() << std::endl;
			throw n;
		}
		construct(&_arr[_size++], value);
		if constexpr (sizeof...(val))
			emplace_back_varidic_prealloc(val...);
	}
	template<class Type, class Allocator >
	template<class ...TypeArgs>
	void vector<Type, Allocator >::emplace_front_varidic(const Type& value, TypeArgs&&...val)
	{
		constexpr int nb = sizeof...(val)+1;
		try
		{
			reallocExactly(nb);
		}
		catch (NullException& n)
		{
			std::cout << n.what() << std::endl;
			throw n;
		}
		memcpy(&_arr[nb], &_arr[0], _size * sizeof(Type));
		construct(&_arr[nb - 1], value);
		if constexpr (sizeof...(val))
			emplace_front_varidic_prealloc(val...);
		_size += sizeof...(val) + 1;
	}

	template<class Type, class Allocator >
	template<unsigned int duplicationCount, class ...TypeArgs>
	void vector<Type, Allocator >::emplace_back_varidic(const Type & value, TypeArgs&& ...val)
	{
		static_assert(duplicationCount, _ASSERT_MESSAGE_CTN1);
		if constexpr (duplicationCount > 0u)
		{
			constexpr unsigned int nb = (sizeof...(val)+1) * duplicationCount;
			try
			{
				reallocExactly(nb);
			}
			catch (NullException& n)
			{
				std::cout << n.what() << std::endl;
				throw n;
			}
			construct_range(&_arr[_size], &_arr[_size + duplicationCount], value);
			if (sizeof...(val))
				emplace_back_varidic_prealloc<duplicationCount>(1, val...);
			_size += nb;
		}
	}

	template<class Type, class Allocator >
	template<unsigned int duplicationCount, class ...TypeArgs>
	void vector<Type, Allocator >::emplace_front_varidic(const Type& value, TypeArgs&& ...val)
	{
		static_assert(duplicationCount, _ASSERT_MESSAGE_CTN1);
		if constexpr (duplicationCount > 0u)
		{
			constexpr unsigned int nb = (sizeof...(val)+1) * duplicationCount;
			try
			{
				reallocExactly(nb);
			}
			catch (NullException& n)
			{
				std::cout << n.what() << std::endl;
				throw n;
			}
			memcpy(&_arr[nb], &_arr[0], _size * sizeof(Type));
			construct_range(&_arr[nb - duplicationCount], &_arr[nb], value);
			if constexpr (sizeof...(val))
				emplace_front_varidic_prealloc<duplicationCount>(val...);
			_size += nb;
		}
	}
#pragma region PrivateDefinition
	template<class Type, class Allocator >
	template<class ...TypeArgs>
	void vector<Type, Allocator >::emplace_back_varidic_prealloc(const Type& value, TypeArgs && ...val)noexcept
	{
		construct(&_arr[_size++], value);
		if constexpr (sizeof...(val))
			emplace_back_varidic_prealloc(val...);
	}

	template<class Type, class Allocator >
	template<class ...TypeArgs>
	void vector<Type, Allocator >::emplace_front_varidic_prealloc(const Type& value, TypeArgs && ...val)noexcept
	{
		construct(&_arr[sizeof...(val)], value);
		if constexpr (sizeof...(val))
			emplace_front_varidic_prealloc(val...);
		
	}

	template<class Type, class Allocator >
	template<unsigned int duplicationCount, class ...TypeArgs>
	void vector<Type, Allocator >::emplace_back_varidic_prealloc(unsigned int index, const Type & value, TypeArgs && ...val)noexcept
	{
		static_assert(duplicationCount, _ASSERT_MESSAGE_CTN1);
		if constexpr (duplicationCount > 0u)
		{
			construct_range(&_arr[_size + (duplicationCount * index)], &_arr[_size + (duplicationCount * (index + 1))], value);
			if constexpr(sizeof...(val))
				emplace_back_varidic_prealloc<duplicationCount>(++index, val...);
		}
	}

	template<class Type, class Allocator >
	template<unsigned int duplicationCount, class ...TypeArgs>
	void vector<Type, Allocator >::emplace_front_varidic_prealloc(const Type & value, TypeArgs && ...val)noexcept
	{
		static_assert(duplicationCount, _ASSERT_MESSAGE_CTN1);
		if constexpr (duplicationCount > 0u)
		{
			constexpr unsigned int nb = (sizeof...(val)+1) * duplicationCount;
			construct_range(&_arr[nb - duplicationCount], &_arr[nb] value);
			if constexpr (sizeof...(val))
				emplace_front_varidic_prealloc<duplicationCount>(index, val...);
		}
	}
#pragma endregion
#pragma endregion

#pragma region EraseThings
	template<class Type, class Allocator>
	template<bool DeleteAuto>
	void vector<Type, Allocator >::pop_front()
	{
		if constexpr(!std::is_trivially_destructible_v<Type>)
			_arr[0].~Type();
		if constexpr (std::is_pointer_v<Type> && DeleteAuto)
			deletePtr(*(void**)&_arr[0]);
		memcpy(&_arr[0], &_arr[1], --_size * sizeof(Type));
		if ((float)_size < (float)(_capacity) * 0.25f)
		{
			try
			{
				shrink_to_fit();
			}
			catch (NullException& n)
			{
				std::cout << n.what() << std::endl;
				throw n;
			}
		}
	}
	template<class Type, class Allocator >
	template<bool DeleteAuto>
	void vector<Type, Allocator >::pop_back()
	{
		--_size;
		if constexpr(!std::is_trivially_destructible_v<Type>)
			_arr[_size].~Type();
		if constexpr(std::is_pointer_v<Type> && DeleteAuto)
			deletePtr(*(void**)&_arr[_size]);
		if ((float)_size < (float)(_capacity) * 0.25f)
		{
			try
			{
				shrink_to_fit();
			}
			catch (NullException& n)
			{
				std::cout << n.what() << std::endl;
				throw n;
			}
		}
	}
	template<class Type, class Allocator >
	template<bool DeleteAuto>
	void vector<Type, Allocator >::erase(unsigned int Where)
	{
#ifdef _DEBUG
		assert(inrange(Where));
#endif // _DEBUG
		if (Where < _size)
		{
			if constexpr(!std::is_trivially_destructible_v<Type>)
				_arr[Where].~Type();
			if constexpr(std::is_pointer_v<Type> && DeleteAuto)
				deletePtr(*(void**)&_arr[Where]);
			memcpy(&_arr[Where], &_arr[Where + 1], sizeof(Type) * (_size - Where));
			--_size;
		}
		if ((float)_size < (float)(_capacity) * 0.25f)
		{
			try
			{
				shrink_to_fit();
			}
			catch(NullException& n)
			{
				std::cout << n.what() << std::endl;
				throw n;
			}
		}
	}

	template<class Type, class Allocator >
	template<bool DeleteAuto>
	void vector<Type, Allocator >::erase(const vectorIterator<Type>& it)
	{
		if (inrange(it.get()))
		{
			erase<DeleteAuto>(it.get() - _arr);
		}
		else
		{
			unsigned int i = find<false>(*it);
			i ^ _size ? erase<DeleteAuto>(i) : nullptr;
		}
		if ((float)_size < (float)(_capacity) * 0.25f)
		{
			try
			{
				shrink_to_fit();
			}
			catch (NullException& n)
			{
				std::cout << n.what() << std::endl;
				throw n;
			}
		}
	}

	template<class Type, class Allocator>
	template<bool DeleteAuto>
	void vector<Type, Allocator>::erase_range(const unsigned int& begin, const unsigned int& end)
	{
#ifdef _DEBUG
		assert(inrange(begin) && inrange(end));
#endif // _DEBUG
		if (begin == end)
			erase(begin);
		else
		{
			int inc = 0;
			begin.get() < end.get() ?
				inc = 1 :
				inc = -1;
			vectorIterator<Type> temp = begin;
			for (; temp != end; temp += inc)
			{
				if constexpr(!std::is_trivially_destructible_v<Type>)
					(*temp).~Type();
				if constexpr(std::is_pointer_v<Type> && DeleteAuto)
					deletePtr(*(void**)&(*temp));
			}
			unsigned int i = range(begin, end), first = range(_arr, begin);
			memcpy(&_arr[first], &_arr[first + i], sizeof(Type) * _(size - (first + i)));
			size -= i;
		}
		if ((float)_size < (float)(_capacity) * 0.25f)
		{
			try
			{
				shrink_to_fit()
			}
			catch (NullException& n)
			{
				std::cout << n.what() << std::endl;
				throw n;
			}
		}
	}

	template<class Type, class Allocator >
	template<bool DeleteAuto>
	void vector<Type, Allocator >::erase_range(const vectorIterator<Type>& begin, const vectorIterator<Type>& end)
	{
		if (begin == end)
			erase(begin);
		else if (inrange(begin.get()) && inrange(end.get()))
		{
			int inc = 0;
			begin.get() < end.get() ?
				inc = 1 :
				inc = -1;
			vectorIterator<Type> temp = begin;
			for (; temp != end; temp += inc)
			{
				if constexpr(!std::is_trivially_destructible_v<Type>)
					(*temp).~Type();
				if constexpr(std::is_pointer_v<Type> && DeleteAuto)
					deletePtr(*(void**)&(*temp));
			}
			unsigned int i = range(begin.get(), end.get()), first = range(_arr, begin.get());
			memcpy(&_arr[first], &_arr[first + i], sizeof(Type) * (_size - (first + i)));
			_size -= i;
		}
		else
		{
			auto beginIt = (*this)[begin];
			if (beginIt.get() != _arr + _size)
			{
				auto endIt = (*this)[end];
				erase_range<DeleteAuto>(beginIt, endIt);
			}
		}
		if((float)_size < (float)(_capacity) * 0.25f)
		{
			try
			{
				shrink_to_fit();
			}
			catch (NullException& n)
			{
				std::cout << n.what() << std::endl;
				throw n;
			}
		}
	}

	template<class Type, class Allocator >
	template<bool DeleteAuto>
	void vector<Type, Allocator >::remove(unsigned int Where)noexcept
	{
#ifdef _DEBUG
		assert(outofrange(Where));
#endif // _DEBUG
		if (Where < _size)
		{
			if constexpr(!std::is_trivially_destructible_v<Type>)
				_arr[Where].~Type();
			if constexpr(std::is_pointer_v<Type> && DeleteAuto)
				deletePtr(*(void**)&_arr[Where]);
			memcpy(&_arr[Where], &_arr[Where + 1], sizeof(Type) * (_size-- - Where));
		}
	}

	template<class Type, class Allocator >
	template<bool DeleteAuto>
	void vector<Type, Allocator >::remove(const vectorIterator<Type>& it)noexcept
	{
		if (inrange(it.get()))
		{
			remove<DeleteAuto>(it.get() - _arr);
		}
		else
		{
			unsigned int i = find<false>(*it);
			i ^ _size ? remove<DeleteAuto>(i) : nullptr;
		}
	}

	template<class Type, class Allocator>
	template<bool DeleteAuto>
	void vector<Type, Allocator>::remove_range(const unsigned int & begin, const unsigned int & end)noexcept
	{
#ifdef _DEBUG
		assert(inrange(begin) && inrange(end));
#endif // _DEBUG
		if (begin == end)
			erase(begin);
		else
		{
			int inc = 0;
			begin.get() < end.get() ?
				inc = 1 :
				inc = -1;
			vectorIterator<Type> temp = begin;
			for (; temp != end; temp += inc)
			{
				if constexpr(!std::is_trivially_destructible_v<Type>)
					(*temp).~Type();
				if constexpr(std::is_pointer_v<Type> && DeleteAuto)
					deletePtr(*(void**)&(*temp));
			}
			unsigned int i = range(begin, end), first = range(_arr, begin);
			memcpy(&_arr[first], &_arr[first + i], sizeof(Type) * _(size - (first + i)));
			size -= i;
		}
	}

	template<class Type, class Allocator >
	template<bool DeleteAuto>
	void vector<Type, Allocator >::remove_range(const vectorIterator<Type>& begin, const vectorIterator<Type>& end)noexcept
	{
		if (begin == end)
			remove(begin);
		else if (inrange(begin.get()) && inrange(end.get()))
		{
			int inc = 0;
			begin.get() < end.get() ?
				inc = 1 :
				inc = -1;
			vectorIterator<Type> temp = begin;
			for (; temp != end; temp += inc)
			{
				if constexpr(!std::is_trivially_destructible_v<Type>)
					(*temp).~Type();
				if constexpr(std::is_pointer_v<Type> && DeleteAuto)
					deletePtr(*(void**)&(*temp));
			}
			unsigned int i = range(begin.get(), end.get()), first = range(_arr, begin.get());
			memcpy(&_arr[first], &_arr[first + i], sizeof(Type) * (_size - (first + i)));
			_size -= i;
		}
		else
		{
			auto beginIt = *this[begin];
			if (beginIt.get() != _arr + _size)
			{
				auto endIt = *this[begin];
				erase_range<DeleteAuto>(beginIt, endIt);
			}
		}
	}

	template<class Type, class Allocator >
	template<bool DeleteAuto>
	void vector<Type, Allocator >::clear()noexcept
	{
		if constexpr (!std::is_trivially_destructible_v<Type>)
		{
			Type* first = _arr, *last = _arr + _size;
			for (; first != last; ++first)
			{
				first->~Type();
			}
		}
		if constexpr(std::is_pointer_v<Type> && DeleteAuto)
		{
			for (unsigned i = 0; i ^ _size; ++i)
			{
				deletePtr(*(void**)&_arr[i]);
			}
		}
		_size = 0;
	}
#pragma endregion

#pragma region Reserve/Resize
	template<class Type, class Allocator >
	void vector<Type, Allocator >::reserve(const unsigned int How)
	{
#ifdef _DEBUG
		assert(How > 0u);
#endif // _DEBUG
		if (How > 0u)
		{
			Type* oldarr = _arr;
			_arr = (Type*)_recalloc(_arr, (_capacity + How), sizeof(Type));
			if (!_arr)
			{
				_arr = oldarr;
				throw NullException();
			}
			_capacity += How;
		}
	}
	template<class Type, class Allocator >
	void vector<Type, Allocator >::resize(const unsigned int NewSize)
	{
#ifdef _DEBUG
		assert(NewSize > 0u);
#endif // _DEBUG
		if (NewSize > 0u)
		{
			if (NewSize < _size)
				erase_range(&_arr[NewSize], &_arr[_size]);
			if (NewSize > _size)
			{
				try
				{
					reallocExactly(NewSize - _size);
				}
				catch(NullException& n)
				{
					std::cout << n.what() << std::endl;
					throw n;
				}
			}
			_size = NewSize;
		}
	}
	template<class Type, class Allocator >
	void vector<Type, Allocator >::shrink_to_fit()
	{
		if (_capacity != _size)
		{
			if (!_size)
			{
				free(_arr);
				_arr = nullptr;
			}
			else
			{
				Type* oldarr = _arr;
				_arr = (Type*)_expand(_arr, _size * sizeof(Type));
				if (!_arr)
				{
					_arr = oldarr;
					throw NullException();
				}
			}
			_capacity = _size;
		}
	}
#pragma endregion

#pragma region FindThings
	template<class Type, class Allocator >
	template<bool iterator>
	const auto vector<Type, Allocator >::find(const Type& What) const noexcept
	{
		vectorIterator<Type> res = begin(), last = end();
		for (; res != last; ++res)
		{
			if (*res == What)
				break;
		}
		if constexpr (iterator)
			return res;
		else
			return (res.get() - _arr);
	}
	template<class Type, class Allocator >
	template<bool iterator>
	const auto vector<Type, Allocator >::find(const vectorIterator<Type>& begin, const vectorIterator<Type>& end, const Type& What) const noexcept
	{
		if (inrange(begin.get()) && inrange(end.get()))
		{
			vectorIterator<Type> res = begin;
			for (; res != end; ++res)
			{
				if (*res == What)
					break;
			}
			if constexpr (iterator)
				return res;
			else
				return (res.get() - _arr);
		}
		if constexpr (iterator)
			return	this->end();
		else
			return _size;
	}
#pragma endregion

#pragma region PrivateFunc
	template<class Type, class Allocator>
	Type vector<Type, Allocator>::find_maximum()
	{
		static_assert(std::is_scalar_v<T>, "Must be a scalar vector");
		T max = -std::numeric_limits<T>::infinity();
		T* it = _arr;
		for (; it != _arr + size; ++it)
		{
			if (*it > max)
				max = *it;
		}
		return max;
	}
	template<class Type, class Allocator>
	Type vector<Type, Allocator>::find_minimum()
	{
		static_assert(std::is_scalar_v<T>, "Must be a scalar vector");
		T min = std::numeric_limits<T>::infinity();
		T* it = _arr;
		for (; it != _arr + size; ++it)
		{
			if (*it < min)
				min = *it;
		}
		return min;
	}
#pragma region Copy
	template<class Type, class Allocator>
	void vector<Type, Allocator>::copy(const vector<Type>& toCopy)
	{
		reallocExactly(toCopy.size());
		for (auto& things : toCopy)
			emplace_back_no_except(things);
	}
#pragma endregion
	template<class Type, class Allocator >
	void vector<Type, Allocator >::CheckCapacity()
	{
		if (_capacity && _arr)
		{
			Type* oldarr = _arr;
			unsigned NewSize = (_capacity + 1) + (_capacity + 1) / 3;
			_arr = (Type*)_recalloc(_arr, NewSize, sizeof(Type));
			if (!_arr)
			{
				_arr = oldarr;
				throw NullException();
			}
			else
			{
				_capacity = NewSize;
			}
		}
		else
		{
			Type* oldarr = _arr;
			_arr = (Type*)calloc(1, sizeof(Type));
			if (!_arr)
			{
				_arr = oldarr;
				throw NullException();
			}
			else
			{
				_capacity = 1;
				_size = 0;
			}
		}
	}
	template<class Type, class Allocator >
	void vector<Type, Allocator >::reallocExactly(unsigned int i)
	{
		if (_size + i > _capacity)
		{
			Type* oldarr = _arr;
			_arr = (Type*)_recalloc(_arr, (_size + i), sizeof(Type));
			if (!_arr)
			{
				_arr = oldarr;
				throw NullException();
			}
			else
			{
				_capacity = _size + i;
			}
		}
	}
	template<class Type, class Allocator >
	template<class ...TypeArgs>
	void vector<Type, Allocator >::construct(Type* ptr, TypeArgs&&... obj)noexcept
	{
#ifdef DEBUG
		assert(ptr);
#endif // DEBUG
		Allocator().construct(ptr, std::forward<TypeArgs...>(obj...));
	}
	template<class Type, class Allocator>
	template<class ...TypeArgs>
	void vector<Type, Allocator>::construct_range(Type* begin, Type* end, TypeArgs&& ...obj)noexcept
	{
#ifdef _DEBUG
		assert(begin && end);
		assert(begin < end);
		assert(begin < &_arr[_capacity] && begin >= _arr);
		assert(end > _arr && end <= &_arr[_capacity + 1]);
#endif // _DEBUG
		for(Type* i = begin; i != end; ++i)
			Allocator().construct(i, std::forward<TypeArgs...>(obj...));
	}
#pragma endregion
}
#endif // !__VECTOR_H__