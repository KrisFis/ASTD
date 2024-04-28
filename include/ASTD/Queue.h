// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include "ASTD/Core/Build.h"

#include "ASTD/Memory.h"
#include "ASTD/QueueAllocator.h"

template<typename ElementT, typename AllocatorT = TQueueAllocator<ElementT>>
class TQueue
{
public:
	// Types
	/////////////////////////////////

	typedef typename AllocatorT::NodeType AllocatorNodeT;

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

	FORCEINLINE bool Peek(ElementT& outVal) const { return PeekImpl(outVal); }

	// Enqueue
	/////////////////////////////////

	FORCEINLINE void Enqueue(const ElementT& val) { AddImpl(val); }
	FORCEINLINE void Enqueue(ElementT&& val) { AddImpl(Move(val)); }

	// Dequeue
	/////////////////////////////////

	FORCEINLINE bool Dequeue() { return RemoveFromHeadImpl(); }
	FORCEINLINE bool Dequeue(ElementT& outVal) { return RemoveFromHeadImpl(outVal); }

	// Empty
	/////////////////////////////////

	FORCEINLINE void Empty() { EmptyImpl(); }
	FORCEINLINE void Reset() { EmptyImpl(); }

private:

	bool PeekImpl(ElementT& outVal) const
	{
		AllocatorNodeT* node = _allocator.GetHead();
		if(node)
		{
			SMemory::CopyElement(&outVal, &node->Value);
		}

		return node != nullptr;
	}

	AllocatorNodeT* AddImpl(const ElementT& val)
	{
		AllocatorNodeT* node = _allocator.Allocate(1);
		SMemory::MoveElement(&node->Value, &val);
		return node;
	}

	AllocatorNodeT* AddImpl(ElementT&& val)
	{
		AllocatorNodeT* node = _allocator.Allocate(1);
		SMemory::MoveElement(&node->Value, &val);
		return node;
	}

	bool RemoveFromHeadImpl()
	{
		AllocatorNodeT* node = _allocator.GetHead();
		if(!node)
		{
			return false;
		}

		SMemory::DestructElement(&node->Value);
		_allocator.Deallocate(node);

		return true;
	}

	bool RemoveFromHeadImpl(ElementT& outVal)
	{
		AllocatorNodeT* node = _allocator.GetHead();
		if(!node)
		{
			return false;
		}

		SMemory::MoveElement(&outVal, &node->Value);
		_allocator.Deallocate(node);

		return true;
	}

	void EmptyImpl()
	{
		AllocatorNodeT* currentNode = _allocator.GetHead();
		if(currentNode)
		{
			while(currentNode != nullptr)
			{
				SMemory::DestructElement(&currentNode->Value);
				currentNode = currentNode->Next;
			}

			_allocator.Release();
		}
	}

	void CopyFrom(const TQueue& other)
	{
		EmptyImpl();

		AllocatorNodeT* currentNode = other._allocator.GetHead();
		while(currentNode != nullptr)
		{
			AllocatorNodeT* newNode = _allocator.Allocate(1);
			SMemory::CopyElement(&newNode->Value, &currentNode->Value);
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

	AllocatorT _allocator = {};
};

template<typename ElementT, typename AllocatorT>
struct TContainerTypeTraits<TQueue<ElementT, AllocatorT>> : public TContainerTypeTraits<void>
{
	using ElementType = ElementT;
	using AllocatorType = AllocatorT;

	enum
	{
		IsContainer = true,
		IsDynamic = true
	};
};
