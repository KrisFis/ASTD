
#pragma once

#include "Memory/MemoryUtilities.h"

template<typename T>
class TTwoWayListAllocatorNode
{
public:

	T Value;
	TTwoWayListAllocatorNode* Next;
	TTwoWayListAllocatorNode* Previous;
};

template<typename T>
class TTwoWayListAllocator
{

public: // Typedefs

	typedef T ElementType;
	typedef TTwoWayListAllocatorNode<T> ElementNode;

public: // Constructors

	FORCEINLINE TTwoWayListAllocator() : Head(nullptr), Tail(nullptr) {}
	FORCEINLINE ~TTwoWayListAllocator() { Reset(); }

public: // Getters

	FORCEINLINE bool IsValid() const { return Head && Tail; }
	FORCEINLINE ElementNode* GetHead() const { return Head; }
	FORCEINLINE ElementNode* GetTail() const { return Tail; }

public: // Manipulation

	ElementType* Allocate(uint32 Num)
	{
		for(uint32 i = 0; i < Num; ++i)
		{
			ElementNode* newNode = SMemory::AllocateTyped<ElementNode>();

			if(IsValid())
			{
				Tail->Next = newNode;
				newNode->Previous = Tail;
			}
			else
			{
				Head = newNode;
				newNode->Previous = Head;
			}

			Tail = newNode;
		}

		Tail->Next = Head;

		return &Tail->Value;
	}

	void Reset()
	{
		if(IsValid())
		{
			ElementNode* currentNode = Head;

			do
			{
				ElementNode* nextNode = currentNode->Next;
				SMemory::DeallocateTyped(currentNode);
				currentNode = nextNode;
			}
			while(currentNode != Head);

			Head = nullptr;
			Tail = nullptr;
		}
	}

private: // Fields

	ElementNode* Head;
	ElementNode* Tail;
};