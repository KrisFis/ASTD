
#include "ASTD.h"

struct A : TSharedClass<A> {};
struct B : A {};

///////////////////////////////////////////////////////////////
// Alloc test
///////////////////////////////////////////////////////////////

static uint64 AllocatedBytes = 0; 

void* operator new(decltype(sizeof(0)) size)
{
	AllocatedBytes += (uint64)size;
	std::cout << "Allocating " << size << " bytes [currently: " << AllocatedBytes << " ]" << std::endl;
	return malloc(size);
}

void operator delete(void* ptr, decltype(sizeof(0)) size)
{
	AllocatedBytes -= (uint64)size;
	std::cout << "Deallocating " << size << " bytes [currently: " << AllocatedBytes << " ]" << std::endl;
	free(ptr);
}

int main(int argc, char *argv[])
{
	while (true)
	{
		TSharedPtr<A> hello = MakeShared<A>();
		TSharedPtr<B> castedHello = CastShared<B>(hello);
		TSharedPtr<A> hello2 = CastShared<A>(castedHello);
		TWeakPtr<A> helloWeak = TWeakPtr<A>(hello);
		
		TSharedPtr<A> testing = hello->AsShared();
		TSharedPtr<B> testing2 = hello->AsShared<B>();
		
		hello.Reset();
		castedHello.Reset();
		hello2.Reset();
		helloWeak.Reset();
		
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
	}

	return 0;
}