#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "Vector/Vector.h"


	void Init(const unsigned int spaceToReserve, CVector* vector)
	{
		if (vector)
		{
			vector->size = 0;
			vector->capacity = spaceToReserve;
			vector->nodes = (void**)malloc(sizeof(void*) * spaceToReserve);
			memset(vector->nodes, 0, vector->capacity);
		}
	}

	void swap(unsigned int firstElem, unsigned int secondElem, CVector* vector)
	{
		if (vector)
		{
			void* temp = vector->nodes[firstElem];
			vector->nodes[firstElem] = vector->nodes[secondElem];
			vector->nodes[secondElem] = temp;
		}
	}

#pragma region InsertThings
	void pushBack(void* Data, CVector* vector)
	{
		if (vector)
		{
			CheckCapacity(vector);
			vector->nodes[vector->size++] = Data;
		}
	}
	void pushFront(void*Data, CVector* vector)
	{
		if (vector)
		{
			CheckCapacity(vector);
			memcpy(&vector->nodes[1], &vector->nodes[0], vector->size++ * sizeof(void*));
			vector->nodes[0] = Data;
		}
	}
	void insert(void* Data, unsigned int elementNumber, CVector* vector)
	{
		--elementNumber;
		if (vector)
		{
			CheckCapacity(vector);
			memcpy(&vector->nodes[elementNumber + 1], &vector->nodes[elementNumber], (vector->size++ - elementNumber) * sizeof(void*));
			vector->nodes[elementNumber] = Data;
		}
	}
	void insertFront(void* Data, unsigned int elementNumber, CVector* vector)
	{
		--elementNumber;
		if (vector)
		{
			CheckCapacity(vector);
			memcpy(&vector->nodes[elementNumber + 1], &vector->nodes[elementNumber], (vector->size++ - elementNumber) * sizeof(void*));
			vector->nodes[elementNumber] = Data;
		}
	}
	void insertBack(void* Data, unsigned int elementNumber, CVector* vector)
	{
		--elementNumber;
		if (vector)
		{
			CheckCapacity(vector);
			memcpy(&vector->nodes[elementNumber + 2], &vector->nodes[elementNumber + 1], (vector->size++ - elementNumber - 1) * sizeof(void*));
			vector->nodes[elementNumber + 1] = Data;
		}
	}
#pragma endregion
#pragma region EraseThings
	void* popBack(CVector* vector)
	{
		void* res = NULL;
		if (vector)
		{
			res = vector->nodes[vector->size];
			vector->nodes[--vector->size] = NULL;
		}
		return res;
	}
	void* popFront(CVector* vector)
	{
		void* res = NULL;
		if (vector)
		{
			res = vector->nodes[0];
			vector->nodes[0] = NULL;
			memcpy(&vector->nodes[0], &vector->nodes[1], --vector->size * sizeof(void*));
		}
		return res;
	}
	void* popFrontOf(unsigned int elementNumber, CVector* vector)
	{
		--elementNumber;
		void* res = NULL;
		if (vector && elementNumber && vector->size)
		{
			res = vector->nodes[elementNumber - 1];
			memcpy(&vector->nodes[elementNumber - 1], &vector->nodes[elementNumber], (vector->size-- - elementNumber) * sizeof(void*));
			vector->nodes[vector->size] = NULL;
		}
		return res;
	}
	void* popBackOf(unsigned int elementNumber, CVector* vector)
	{
		--elementNumber;
		void* res = NULL;
		if (vector&& elementNumber + 2 < vector->size && vector->size > 0)
		{
			res = vector->nodes[elementNumber + 1];
			memcpy(&vector->nodes[elementNumber + 1], &vector->nodes[elementNumber + 2], (vector->size-- - (elementNumber + 2)) * sizeof(void*));
			vector->nodes[vector->size] = NULL;
		}
		return res;
	}
	void* erase(void* Data, CVector* vector)
	{
		if (vector)
		{
			for (unsigned int i = 0; i ^ vector->size; ++i)
			{
				if (Data == vector->nodes[i])
				{
					memcpy(&vector->nodes[i], &vector->nodes[i + 1], sizeof(void*) * (vector->size-- - i));
					vector->nodes[vector->size] = NULL;
					break;
				}
			}
		}
		return Data;
	}

	void* eraseAllSameThing(void* Data, CVector* vector)
	{
		if (vector)
		{
			for (unsigned int i = 0; i ^ vector->size; ++i)
			{
				if (Data == vector->nodes[i])
				{
					memcpy(&vector->nodes[i], &vector->nodes[i + 1], sizeof(void*) * (vector->size-- - i));
					vector->nodes[vector->size] = NULL;
					--i;
				}
			}
		}
		return Data;
	}

	void clear(CVector* vector)
	{
		if (vector)
		{
			vector->size = 0;
			memset(vector->nodes, 0, vector->capacity);
		}
	}

	void clearAll(CVector* vector)
	{
		if (vector)
		{
			free(vector->nodes);
			vector->nodes = NULL;
			vector->capacity = vector->size = 0;
		}
	}
#pragma endregion
#pragma region Reserve/Resize
	void reserve(const unsigned int spaceToReserve, CVector* vector)
	{
		if (vector)
		{
			vector->nodes = (void**)realloc(vector->nodes, (vector->capacity += spaceToReserve) * sizeof(void*));
		}
	}
	void shrinkToFit(CVector* vector)
	{
		if (vector)
		{
			if (vector->size)
				vector->nodes = (void**)realloc(vector->nodes, sizeof(void*) * vector->size);
			else
			{
				free(vector->nodes);
				vector->nodes = NULL;
			}
			vector->capacity = vector->size;
		}
	}
	void CheckCapacity(CVector* vector)
	{
		if (vector)
		{
			if (vector->nodes)
			{
				if (vector->capacity && vector->capacity == vector->size)
				{
					vector->nodes = (void**)realloc(vector->nodes, sizeof(void*) *
						(vector->capacity *= 2));
				}
				else if (vector->capacity == vector->size)
				{
					vector->nodes = (void**)realloc(vector->nodes, sizeof(void*));
					vector->capacity = 1;
				}
			}
			else
			{
				vector->size = 0;
				vector->capacity = 1u;
				vector->nodes = (void**)malloc(sizeof(void*));
				memset(vector->nodes, 0, vector->capacity);
			}
		}
	}
#pragma endregion
#pragma region FindThings
	void* getNode(const unsigned int nb, CVector* vector)
	{
		if (vector && vector->size >= nb)
			return vector->nodes[(nb - 1)];
		else
		{
			printf("Index out of bounds");
			return NULL;
		}
	}
	unsigned int find(const void* const  Data, CVector* vector)
	{
		int res = -1;
		for (unsigned int i = 0; i ^ vector->size; ++i)
		{
			if (Data == vector->nodes[i])
			{
				res = i;
				break;
			}
		}
		return res;
	}
#pragma endregion
