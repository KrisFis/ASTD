
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

template<typename ElementType>
void ReadArray(const TArray<ElementType>& Array)
{
	for(uint32 i = 0; i < Array.GetCount(); ++i)
	{
		SLogger::Begin() << "Value [" << i << "]: " << Array[i] << SLogger::End();
	}
}

void TestArray()
{
	TArray<uint8> bitArray = {1,2,3,4,5,6};
	TArray<uint8> bitArray2 = {7,8,9,10,11,12};

	bitArray.Append(bitArray2);

	ReadArray(bitArray);
}

template<typename CharType>
void CompareStrings(const CharType* Lhs, const CharType* Rhs)
{
	SLogger::Begin() << Lhs << " compare to " << Rhs << " is : " << SCString::Compare(Lhs, Rhs) << SLogger::End();
}

int main()
{
	double test = SCString::ToDouble("4.5");
	double test2 = SCString::ToDouble(TEXT("4.5"));

	//TestString();
	CompareStrings("abc", "abc");
	CompareStrings(TEXT("abc"), TEXT("abc"));
	CompareStrings("ab", "abc");
	CompareStrings(TEXT("ab"), TEXT("abc"));
	CompareStrings("abc", "ab");
	CompareStrings(TEXT("abc"), TEXT("ab"));
	CompareStrings("abd", "abc");
	CompareStrings(TEXT("abd"), TEXT("abc"));
	CompareStrings("abc", "abd");
	CompareStrings(TEXT("abc"), TEXT("abd"));

	return 0;
}
