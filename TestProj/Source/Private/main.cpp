
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

	// Instance is valid
	FORCEINLINE bool IsValid() const 
	{ 
		return A == 1;
	}

	uint8 A, B;
};

// Global is valid
static bool IsValid(const SCustomData& Data) 
{ 
	return Data.IsValid();
}

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

void StringTest2()
{
	SString test = TEXT(":-:ccc:cc: :cc::cc:ccc");
	TArray<SString> result = test.SplitToArray(TEXT(':'));

	SLogger::Begin() << "Writing result for split to array for \"" << test << "\"" << SLogger::End();

	for(uint8 i = 0; i < result.GetCount(); ++i)
	{
		SLogger::Begin() << "Index " << i << ": " << result[i] << SLogger::End();
	}
}

void StringTest3()
{
	SString test = SString::FromInt32(25);
	SLogger::Begin() << "Int test [25]: " << test << ", " << test.ToInt32() << SLogger::End();

	SString test2 = SString::FromInt64(-120);
	SLogger::Begin() << "Int test [-120]: " << test2 << ", " << test2.ToInt64() << SLogger::End();

	SString test3 = SString::FromDouble(158.215, 3);
	SLogger::Begin() << "Int test [158.215]: " << test3 << ", " << test3.ToDouble() << SLogger::End();
}

void StringTest4()
{
	SString test = TEXT("ccc:cc: :cc::cc:ccc");
	SString result = test.Replace(TEXT(':'), TEXT('|'));
	SString result2 = test.Replace(TEXT("cC"), TEXT("abc"), 2, false);
	SString result3 = test.Replace(TEXT("cC"), TEXT("abc"), 2);

	SLogger::Begin() << "Writing result for replace of array \"" << test << "\"" << SLogger::End();
	SLogger::Begin() << "Test 1 - Reference: \"ccc|cc| |cc||cc|ccc\", Result: \"" << result << "\"" << SLogger::End();
	SLogger::Begin() << "Test 2 - Reference: \"abcc:abc: :cc::cc:ccc\", Result: \"" << result2 << "\"" << SLogger::End();
	SLogger::Begin() << "Test 3 - Reference: \"ccc:cc: :cc::cc:ccc\", Result: \"" << result3 << "\"" << SLogger::End();
}

void ValidTest()
{
	SCustomData data(1,2);
	SLogger::Begin() << "[REF] Valid check test: " << (CHECK_VALID(data) ? "TRUE" : "FALSE") << SLogger::End();

	SCustomData* dataPtr = &data;
	SLogger::Begin() << "[REF] Valid check test: " << (CHECK_VALID(dataPtr) ? "TRUE" : "FALSE") << SLogger::End();

	CHECK(IsValid(data));
	CHECK(IsValid(dataPtr));
}

void OptionalTest()
{
	TOptional<SCustomData> myOpt;
	CHECK(!myOpt.IsSet());

	myOpt = SCustomData(1, 2);
	CHECK(myOpt.IsSet());

	SLogger::Begin() << "Optional test 1: " << myOpt->A << SLogger::End();

	TOptional<SCustomData> myOpt2(Move(myOpt));

	SLogger::Begin() << "Optional test 2: " << myOpt2->A << SLogger::End();

	SCustomData& testData = myOpt2.GetRef();

	SLogger::Begin() << "Optional test 3: " << testData.A << SLogger::End();

	myOpt2.Reset();
	SCustomData testData2 = myOpt2.Get(SCustomData(2, 2));

	SLogger::Begin() << "Optional test 4: " << testData2.A << SLogger::End();
}

void CheckAllocatedMemory()
{
	const double allocatedMemory = SMemory::GetAllocatedBytes();
	SLogger::Begin() << "Allocated Memory: " << allocatedMemory << SLogger::End();
	CHECKF(allocatedMemory < TINY_NUMBER);
}

int main()
{
	SLogger::EmptyLine();
	{	
		OptionalTest();
		CheckAllocatedMemory();
	}
	SLogger::EmptyLine();
	{	
		ValidTest();
		CheckAllocatedMemory();
	}
	SLogger::EmptyLine();
	{	
		StringTest1();
		CheckAllocatedMemory();
	}
	SLogger::EmptyLine();
	{	
		StringTest2();
		CheckAllocatedMemory();
	}
	SLogger::EmptyLine();
	{	
		StringTest3();
		CheckAllocatedMemory();
	}
	SLogger::EmptyLine();
	{
		StringTest4();
		CheckAllocatedMemory();
	}
	SLogger::EmptyLine();

	return 0;
}
