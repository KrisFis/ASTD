
#include "ASTD.h"

///////////////////////////////////////////////////////////////
// Alloc test
///////////////////////////////////////////////////////////////

struct SMemoryTracker
{
public:

	FORCEINLINE SMemoryTracker()
		: Bytes(0)
	{}

	FORCEINLINE void AddBytes(const uint64& InBytes) { Bytes += InBytes; } 
	FORCEINLINE void RemoveBytes(const uint64& InBytes) { Bytes -= InBytes; } 

	FORCEINLINE const uint64& GetBytes() const { return Bytes; }
	FORCEINLINE void Reset() { Bytes = 0; }

	void PrintBytes(const char* EventName = nullptr) const 
	{
		if(EventName)
		{
			std::cout << "Allocated memory [" << Bytes << " bytes] for [" << EventName << "]" << std::endl; 
		}
		else
		{
			std::cout << "Allocated memory [" << Bytes << " bytes]" << std::endl; 
		}
	}

private:

	uint64 Bytes;
	
};

static SMemoryTracker MemTracker;

void* operator new(decltype(sizeof(0)) size)
{
	MemTracker.AddBytes((uint64)size);
	return malloc(size);
}

void operator delete(void* ptr, decltype(sizeof(0)) size)
{
	MemTracker.RemoveBytes((uint64)size);
	free(ptr);
}

struct A : TSharedClass<A> {};
struct B : A {};

int main(int argc, char *argv[])
{
	while (true)
	{
		MemTracker.PrintBytes("Start of loop");
		
		TSharedPtr<A> hello = MakeShared<A>();
		TSharedPtr<B> castedHello = CastShared<B>(hello);
		TSharedPtr<A> hello2 = CastShared<A>(castedHello);
		TWeakPtr<A> helloWeak = TWeakPtr<A>(hello);
		
		hello.Reset();
		castedHello.Reset();
		hello2.Reset();
		helloWeak.Reset();
		
		MemTracker.PrintBytes("Mid of loop");
		
		hello = MakeShareable<A>(new A());
		castedHello = CastShared<B>(hello);
		hello2 = CastShared<A>(castedHello);
		helloWeak = TWeakPtr<A>(hello);
		
		hello.Reset();
		castedHello.Reset();
		hello2.Reset();
		
		hello = MakeShareable<A>(new B());
		castedHello = CastShared<B>(hello);
		hello2 = CastShared<A>(castedHello);
		helloWeak = TWeakPtr<A>(hello);
		
		hello.Reset();
		castedHello.Reset();
		hello2.Reset();
		helloWeak.Reset();
		
		MemTracker.PrintBytes("End of loop");
	}

	return 0;
}