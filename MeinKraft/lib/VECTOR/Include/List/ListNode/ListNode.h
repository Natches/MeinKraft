#ifndef __LIST_NODE__
#define __LIST_NODE__


template<class Type>
class ListNode final
{
public:
	ListNode(const Type& _Data, const ListNode<Type>* const _Next, const ListNode<Type>* const _Previous, const unsigned int ID);
	ListNode();
	~ListNode() = default;
	
	inline const Type getData() { return Data; }
	inline void setData(const Type value) { Data = value; DirtyData = false; }

	inline const ListNode<Type>* getNext() { return Next; }
	inline const ListNode<Type>* getPrevious() { return Previous; }
	inline const bool isDirty() { return DirtyData; }
	inline const unsigned int getID(){ return id; }

private:
	void DestructNode();
	void DeleteNode();

private:
	Type Data;
	ListNode<Type>* Next;
	ListNode<Type>* Previous;
	bool DirtyData;

private:
	unsigned int id;
};

#include "../Src/List/ListNode/ListNode.cpp"

#endif//__LIST_NODE__
