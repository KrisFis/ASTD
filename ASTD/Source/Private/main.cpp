
#include "ASTDMinimal.h"
#include "STDMinimal.h"

struct A {};
struct B : A {};

int main(int argc, char *argv[])
{
	while (true)
	{
		TSharedPtr<A> hello = MakeShared<A>();
		TSharedPtr<B> castedHello = CastShared<B>(hello);
		TSharedPtr<A> hello2 = CastShared<A>(castedHello);
		TWeakPtr<A> helloWeak = TWeakPtr<A>(hello);
		
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