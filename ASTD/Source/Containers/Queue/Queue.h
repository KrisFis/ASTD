
#pragma once

#include "Core/Types.h"
#include "Core/Type/TypeTraits.h"
#include "Algo/Memory.h"

#include "Containers/Queue/Allocator/QueueAllocator.h"

template<typename InElementType, typename InAllocatorType = TQueueAllocator<InElementType>>
class TQueue
{

public: // Types

	typedef InElementType ElementType;
	typedef InAllocatorType AllocatorType;
	typedef typename AllocatorType::NodeType AllocatorNodeType;

public: // Constructors

	FORCEINLINE TQueue() : Allocator() {}
	FORCEINLINE TQueue(const TQueue& Other) : Allocator() { CopyFrom(Other); }
	FORCEINLINE TQueue(TQueue&& Other) : Allocator() { MoveFrom(Move(Other)); }

public: // Destructor

	FORCEINLINE ~TQueue() { EmptyImpl(); }

public: // Operators

	FORCEINLINE bool operator==(const TQueue& Other) const { return Allocator == Other.Allocator; }
	FORCEINLINE bool operator!=(const TQueue& Other) const { return Allocator != Other.Allocator; }

	FORCEINLINE TQueue& operator=(const TQueue& Other) { CopyFrom(Other); return *this; }
	FORCEINLINE TQueue& operator=(TQueue&& Other) { MoveFrom(Move(Other)); return *this; }

public: // Getters

	FORCEINLINE bool IsEmpty() const { return !Allocator.GetHead(); }

public: // Peek

	FORCEINLINE bool Peek(ElementType& OutValue) const { return PeekImpl(OutValue); }

public: // Enqueue

	FORCEINLINE void Enqueue(const ElementType& Value) { AddImpl(Value); }
	FORCEINLINE void Enqueue(ElementType&& Value) { AddImpl(Move(Value)); }

public: // Dequeue

	FORCEINLINE bool Dequeue() { return RemoveFromHeadImpl(); }
	FORCEINLINE bool Dequeue(ElementType& OutValue) { return RemoveFromHeadImpl(OutValue); }

public: // Empty

	FORCEINLINE void Empty() { EmptyImpl(); }
	FORCEINLINE void Reset() { EmptyImpl(); }

private: // Helper methods

	bool PeekImpl(ElementType& OutValue) const
	{
		AllocatorNodeType* node = Allocator.GetHead();
		if(node)
		{
			NAlgo::CopyElement(&OutValue, &node->Value);
		}

		return node != nullptr;
	}

	AllocatorNodeType* AddImpl(const ElementType& Value)
	{
		AllocatorNodeType* node = Allocator.Allocate(1);
		NAlgo::MoveElement(&node->Value, &Value);
		return node;
	}

	AllocatorNodeType* AddImpl(ElementType&& Value)
	{
		AllocatorNodeType* node = Allocator.Allocate(1);
		NAlgo::MoveElement(&node->Value, &Value);
		return node;
	}

	bool RemoveFromHeadImpl()
	{
		AllocatorNodeType* node = Allocator.GetHead();
		if(!node)
		{
			return false;
		}

		NAlgo::DestructElement(&node->Value);
		Allocator.Deallocate(node);

		return true;
	}

	bool RemoveFromHeadImpl(ElementType& OutValue)
	{
		AllocatorNodeType* node = Allocator.GetHead();
		if(!node)
		{
			return false;
		}

		NAlgo::MoveElement(&OutValue, &node->Value);
		Allocator.Deallocate(node);

		return true;
	}

	void EmptyImpl()
	{
		AllocatorNodeType* currentNode = Allocator.GetHead();
		if(currentNode)
		{
			while(currentNode != nullptr)
			{
				NAlgo::DestructElement(&currentNode->Value);
				currentNode = currentNode->Next;
			}

			Allocator.Release();
		}
	}

	void CopyFrom(const TQueue& Other)
	{
		EmptyImpl();

		AllocatorNodeType* currentNode = Other.Allocator.GetHead();
		while(currentNode != nullptr)
		{
			AllocatorNodeType* newNode = Allocator.Allocate(1);
			NAlgo::CopyElement(&newNode->Value, &currentNode->Value);
		}
	}

	void MoveFrom(TQueue&& Other)
	{
		EmptyImpl();

		Allocator.SetHead(Other.Allocator.GetHead());
		Allocator.SetTail(Other.Allocator.GetTail());

		Other.Allocator.SetHead(nullptr);
		Other.Allocator.SetTail(nullptr);
	}

private: // Fields

	AllocatorType Allocator;

};