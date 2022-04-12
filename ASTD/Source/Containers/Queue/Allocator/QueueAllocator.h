
#pragma once

#include "Core/Types.h"

#include "Core/Memory.h"

template<typename InElementType>
class TQueueAllocator
{

public: // Types

	typedef InElementType ElementType;
	typedef uint32 SizeType;

	struct SNode
	{
		ElementType Value;

		SNode* Next;
		SNode* Previous;
	};

	typedef SNode NodeType;

public: // Constructor

	FORCEINLINE TQueueAllocator() : Head(nullptr), Tail(nullptr) {}

public: // Operators

	FORCEINLINE bool operator==(const TQueueAllocator& Other) const { return Head == Other.Head && Tail == Other.Tail; }
	FORCEINLINE bool operator!=(const TQueueAllocator& Other) const { return !operator==(Other); }

public: // Getters

	FORCEINLINE NodeType* GetHead() const { return Head; }
	FORCEINLINE void SetHead(NodeType* Node) { Head = Node; }

	FORCEINLINE NodeType* GetTail() const { return Tail; }
	FORCEINLINE void SetTail(NodeType* Node) { Tail = Node; }

public: // Methods

	NodeType* Allocate(SizeType Num)
	{
		NodeType* firstNode = nullptr;
		NodeType* prevNode = Tail;
		for(SizeType i = 0; i < Num; ++i)
		{
			NodeType* newNode = NMemoryType::Allocate<NodeType>();
			newNode->Previous = prevNode;
			newNode->Next = nullptr;

			if(prevNode)
				prevNode->Next = newNode;

			if(!firstNode)
				firstNode = newNode;

			prevNode = newNode;
		}

		Tail = prevNode;
		if(!Head) { Head = firstNode; }

		return firstNode;
	}

	void Deallocate(NodeType* Node)
	{
		// Link previous node with next and vice versa
		{
			if(Node != Head)
			{
				Node->Previous->Next = Node->Next;
			}
			else
			{
				Head = Node->Next;
			}

			if(Node != Tail)
			{
				Node->Next->Previous = Node->Previous;
			}
			else
			{
				Tail = Node->Previous;
			}
		}

		NMemoryType::Deallocate(Node);
	}

	void Release()
	{
		NodeType* currentNode = Head;
		while(currentNode != nullptr)
		{
			NodeType* nextNode = currentNode->Next;
			NMemoryType::Deallocate(currentNode);
			currentNode = nextNode;
		}

		Head = nullptr;
		Tail = nullptr;
	}

private: // Fields

	NodeType* Head;
	NodeType* Tail;
};