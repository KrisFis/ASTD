
#include <iostream>

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

int main()
{
	SLogger::Begin() << "Hello World" << SLogger::End();
	
	TArray<SCustomData> myArray;
	SCustomData& data = myArray.Add_GetRef({1, 3});
	data.A = 2;

	myArray.Add({2, 4});
	myArray.Add({2, 4});
	myArray.Add({2, 4});
	myArray.Add({2, 4});
	myArray.Add({2, 4});
	myArray.Add({2, 4});
	myArray.Add({2, 4});

	for(SCustomData& data : myArray)
	{
		SLogger::Begin() << "Read data [A = " << data.A << ", B = " << data.B << "]" << SLogger::End();
	}

	SCustomData data2 = myArray[2];

	SCustomData* a = myArray.FindByFunc(
		[&](const SCustomData& Data)
		{
			return Data.A == 1;
		}
	);

	int64 idx = myArray.FindIndex({2, 3});
	myArray.RemoveAt(idx);

	return 0;
}