
#pragma once

#include "Type/TypeUtilities.h"
#include "TypeTraits/TypeTraits.h"

#include "Containers/Queue/Allocator/QueueAllocator.h"

template<typename InElementType, typename InAllocatorType = TQueueAllocator<InElementType>>
class TQueue
{

public: // Types

	typedef InElementType ElementType;
	typedef InAllocatorType AllocatorType;
	typedef typename AllocatorType::NodeType AllocatorNodeType;

	static constexpr TSize ELEMENT_SIZE = sizeof(ElementType);

public: // Constructors

	FORCEINLINE TQueue() : Allocator() {}

public: // Getters

	FORCEINLINE bool IsEmpty() const { return !Allocator.GetHead(); }

public: // Peek

	bool Peek(ElementType& OutValue) const
	{
		AllocatorNodeType* node = Allocator.GetHead();
		if(node)
		{
			SMemory::Copy(
				&OutValue,
				&node->Value,
				ELEMENT_SIZE
			);
		}

		return node != nullptr;
	}

public: // Enqueue

	void Enqueue(const ElementType& Value)
	{
		AllocatorNodeType* node = Allocator.Allocate(1);
		NMemoryUtilities::CallConstructor(&node->Value, Value);
	}

	void Enqueue(ElementType&& Value)
	{
		AllocatorNodeType* node = Allocator.Allocate(1);
		NMemoryUtilities::CallConstructor(&node->Value, Move(Value));
	}

public: // Dequeue

	bool Dequeue()
	{
		AllocatorNodeType* node = Allocator.GetHead();
		if(!node)
		{
			return false;
		}

		NMemoryUtilities::CallDestructor(node);
		Allocator.Deallocate(node);

		return true;
	}

	bool Dequeue(ElementType& OutValue)
	{
		AllocatorNodeType* node = Allocator.GetHead();
		if(!node)
		{
			return false;
		}

		SMemory::Move(
			&OutValue,
			&node->Value,
			ELEMENT_SIZE
		);

		Allocator.Deallocate(node);
		return true;
	}

public: // Empty

	FORCEINLINE void Empty() 
	{ 
		AllocatorNodeType* currentNode = Allocator.GetHead();
		while(currentNode != nullptr)
		{
			NMemoryUtilities::CallDestructor(&currentNode->Value);
			currentNode = currentNode->Next;
		}

		Allocator.Release();
	}

	FORCEINLINE void Reset() { Allocator.Release(); }

private: // Fields

	AllocatorType Allocator;

};