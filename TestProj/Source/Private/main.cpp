
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

int main()
{
	TArray<uint8> bitArray = {1,2,3,4,5,6};
	TArray<uint8> bitArray2 = {7,8,9,10,11,12};

	bitArray.Append(bitArray2);

	ReadArray(bitArray);

	return 0;
}
