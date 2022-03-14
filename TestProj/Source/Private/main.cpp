
#include <iostream>
#include <vector>

#include "ASTD.h"

struct SCustomData
{
	SCustomData()
	{
		SLogger::Begin() << "Default constructor" << SLogger::End();
	}

	SCustomData(const SCustomData& Other)
		: A(Other.A)
		, B(Other.B)
	{
		SLogger::Begin() << "Copy constructor" << SLogger::End();
	}

	SCustomData(SCustomData&& Other)
		: A(Other.A)
		, B(Other.B)
	{
		SLogger::Begin() << "Move constructor" << SLogger::End();
	}

	SCustomData(uint8 InA, uint8 InB)
		: A(InA)
		, B(InB)
	{
		SLogger::Begin() << "Constructor with [A = " << A << ", B = " << B << "]" << SLogger::End();
	}

	~SCustomData()
	{
		SLogger::Begin() << "Destructor" << SLogger::End();
	}

	uint8 A, B;
};

void StringTest1()
{
	SString test = TEXT("Hello World");
	test.Append(TEXT('!'));
	test.Append(TEXT("dlroW olleH"));

	SLogger::Begin() << test << SLogger::End();

	SString left, right;
	CHECK(test.Split(TEXT('!'), &left, &right));

	SLogger::Begin() << test << TEXT(" [ LEFT: ") << left << TEXT(", RIGHT: ") << right << TEXT("]") << SLogger::End();

}

int main()
{
	SString test = TEXT("ccc:cc: :cc::cc:ccc");
	TArray<SString> result = test.SplitToArray(TEXT(':'));

	for(const SString& str : result)
	{
		std::cout << str.GetChars() << std::endl;
	}

	return 0;
}
