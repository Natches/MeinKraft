
#ifdef __VECTOR_ITERATOR_H__
namespace ctn
{
#pragma region Constructor/Destructor
template<class Type>
vectorIterator<Type>::vectorIterator()
{
	ptr = nullptr;
}
template<class Type>
vectorIterator<Type>::vectorIterator(const vector<Type>& v)
{
	ptr = &v._arr[0];
}
template<class Type>
vectorIterator<Type>::vectorIterator(const Type* _ptr)
{
	ptr = const_cast<Type*>(_ptr);
}
template<class Type>
vectorIterator<Type>::vectorIterator(const vectorIterator<Type>& v)
{
	*this = v;
}
template<class Type>
vectorIterator<Type>::vectorIterator(const vectorIterator<Type>&& v)
{
	*this = std::move(v);
}
#pragma endregion

#pragma region Operator
template<class Type>
vectorIterator<Type>& vectorIterator<Type>::operator=(const Type* _ptr)
{
	ptr = const_cast<Type*>(_ptr);
	return *this;
}
template<class Type>
vectorIterator<Type>& vectorIterator<Type>::operator=(const vectorIterator<Type>& v)
{
	ptr = v.ptr;
	return (*this);
}
template<class Type>
vectorIterator<Type>& vectorIterator<Type>::operator=(const vectorIterator<Type>&& v)
{
	return (*this = v);
}
template<class Type>
Type& vectorIterator<Type>::operator*() const
{
	return *ptr;
}
template<class Type>
vectorIterator<Type>& vectorIterator<Type>::operator++()
{
	++ptr;
	return *this;
}
template<class Type>
vectorIterator<Type>& vectorIterator<Type>::operator--()
{
	--ptr;
	return *this;
}
template<class Type>
vectorIterator<Type> vectorIterator<Type>::operator++(int) const
{
	vectorIterator<Type> tmp = *this;
	++*this;
	return tmp;
}
template<class Type>
vectorIterator<Type> vectorIterator<Type>::operator--(int) const
{
	vectorIterator<Type> tmp = *this;
	--*this;
	return tmp;
}
template<class Type>
vectorIterator<Type>& vectorIterator<Type>::operator+=(int i)
{
	ptr += i; 
	return *this;
}
template<class Type>
vectorIterator<Type>& vectorIterator<Type>::operator-=(int i)
{
	ptr -= i;
	return *this;
}
template<class Type>
vectorIterator<Type> vectorIterator<Type>::operator+(int i) const
{
	vectorIterator<Type> tmp = *this;
	return (tmp += i);
}
template<class Type>
vectorIterator<Type> vectorIterator<Type>::operator-(int i) const
{
	vectorIterator<Type> tmp = *this;
	return (tmp -= i);
}
template<class Type>
constexpr bool vectorIterator<Type>::operator==(const IIterator& it) const
{
	return (ptr == it.get());
}
template<class Type>
constexpr bool vectorIterator<Type>::operator!=(const IIterator& it) const
{
	return (ptr != it.get());
}
template<class Type>
constexpr bool vectorIterator<Type>::operator>(const IIterator& it) const
{
	return (ptr > it.get());
}
template<class Type>
constexpr bool vectorIterator<Type>::operator<(const IIterator& it) const
{
	return (ptr < it.get());
}
template<class Type>
constexpr bool vectorIterator<Type>::operator>=(const IIterator& it) const
{
	return (ptr >= it.get());
}
template<class Type>
constexpr bool vectorIterator<Type>::operator<=(const IIterator& it) const
{
	return (ptr <= it.get());
}
#pragma endregion

#pragma region Getter
template<class Type>
Type* vectorIterator<Type>::get() const
{
	return ptr;
}
#pragma endregion
}
#endif