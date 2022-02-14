
#pragma once

#include "Memory/MemoryUtilities.h"

template<typename T>
class TListAllocatorNode
{
public:

	T Value;
	TListAllocatorNode* Next;
};

template<typename T>
class TListAllocator
{

public: // Typedefs

	typedef T ElementType;
	typedef TListAllocatorNode<T> ElementNode;

public: // Constructors

	FORCEINLINE TListAllocator() : Head(nullptr), Tail(nullptr) {}
	FORCEINLINE ~TListAllocator() { Reset(); }

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
			}
			else
			{
				Head = newNode;
			}

			Tail = newNode;
		}

		Tail->Next = nullptr;

		return &Tail->Value;
	}

	void Reset()
	{
		if(IsValid())
		{
			ElementNode* currentNode = Head;
			while(currentNode != nullptr)
			{
				ElementNode* nextNode = currentNode->Next;
				SMemory::DeallocateTyped(currentNode);
				currentNode = nextNode;
			}

			Head = nullptr;
			Tail = nullptr;
		}
	}

private: // Fields

	ElementNode* Head;
	ElementNode* Tail;
};