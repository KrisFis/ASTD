// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include "Algo/Memory.h"
#include "Containers/Queue/Allocator/QueueAllocator.h"

template<typename InElementType, typename InAllocatorType = TQueueAllocator<InElementType>>
class TQueue
{
public:
	// Types
	/////////////////////////////////

	typedef InElementType ElementType;
	typedef InAllocatorType AllocatorType;
	typedef typename AllocatorType::NodeType AllocatorNodeType;

	// Constructors
	/////////////////////////////////

	FORCEINLINE TQueue() = default;
	FORCEINLINE TQueue(const TQueue& other) { CopyFrom(other); }
	FORCEINLINE TQueue(TQueue&& other) { MoveFrom(Move(other)); }

	// Destructor
	/////////////////////////////////

	FORCEINLINE ~TQueue() { EmptyImpl(); }

	// Operators
	/////////////////////////////////

	FORCEINLINE bool operator==(const TQueue& other) const { return _allocator == other._allocator; }
	FORCEINLINE bool operator!=(const TQueue& other) const { return _allocator != other._allocator; }

	FORCEINLINE TQueue& operator=(const TQueue& other) { CopyFrom(other); return *this; }
	FORCEINLINE TQueue& operator=(TQueue&& other) { MoveFrom(Move(other)); return *this; }

	// Getters
	/////////////////////////////////

	FORCEINLINE bool IsEmpty() const { return !_allocator.GetHead(); }

	// Peek
	/////////////////////////////////

	FORCEINLINE bool Peek(ElementType& outVal) const { return PeekImpl(outVal); }

	// Enqueue
	/////////////////////////////////

	FORCEINLINE void Enqueue(const ElementType& val) { AddImpl(val); }
	FORCEINLINE void Enqueue(ElementType&& val) { AddImpl(Move(val)); }

	// Dequeue
	/////////////////////////////////

	FORCEINLINE bool Dequeue() { return RemoveFromHeadImpl(); }
	FORCEINLINE bool Dequeue(ElementType& outVal) { return RemoveFromHeadImpl(outVal); }

	// Empty
	/////////////////////////////////

	FORCEINLINE void Empty() { EmptyImpl(); }
	FORCEINLINE void Reset() { EmptyImpl(); }

private:

	bool PeekImpl(ElementType& outVal) const
	{
		AllocatorNodeType* node = _allocator.GetHead();
		if(node)
		{
			NAlgo::CopyElement(&outVal, &node->Value);
		}

		return node != nullptr;
	}

	AllocatorNodeType* AddImpl(const ElementType& val)
	{
		AllocatorNodeType* node = _allocator.Allocate(1);
		NAlgo::MoveElement(&node->Value, &val);
		return node;
	}

	AllocatorNodeType* AddImpl(ElementType&& val)
	{
		AllocatorNodeType* node = _allocator.Allocate(1);
		NAlgo::MoveElement(&node->Value, &val);
		return node;
	}

	bool RemoveFromHeadImpl()
	{
		AllocatorNodeType* node = _allocator.GetHead();
		if(!node)
		{
			return false;
		}

		NAlgo::DestructElement(&node->Value);
		_allocator.Deallocate(node);

		return true;
	}

	bool RemoveFromHeadImpl(ElementType& outVal)
	{
		AllocatorNodeType* node = _allocator.GetHead();
		if(!node)
		{
			return false;
		}

		NAlgo::MoveElement(&outVal, &node->Value);
		_allocator.Deallocate(node);

		return true;
	}

	void EmptyImpl()
	{
		AllocatorNodeType* currentNode = _allocator.GetHead();
		if(currentNode)
		{
			while(currentNode != nullptr)
			{
				NAlgo::DestructElement(&currentNode->Value);
				currentNode = currentNode->Next;
			}

			_allocator.Release();
		}
	}

	void CopyFrom(const TQueue& other)
	{
		EmptyImpl();

		AllocatorNodeType* currentNode = other._allocator.GetHead();
		while(currentNode != nullptr)
		{
			AllocatorNodeType* newNode = _allocator.Allocate(1);
			NAlgo::CopyElement(&newNode->Value, &currentNode->Value);
		}
	}

	void MoveFrom(TQueue&& other)
	{
		EmptyImpl();

		_allocator.SetHead(other._allocator.GetHead());
		_allocator.SetTail(other._allocator.GetTail());

		other._allocator.SetHead(nullptr);
		other._allocator.SetTail(nullptr);
	}

	AllocatorType _allocator = {};
};
