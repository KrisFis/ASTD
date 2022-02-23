
#include <iostream>

#include "ASTD.h"

struct SCustomData
{
	SCustomData()
	{
		std::cout << "Default constructor" << std::endl;
	}

	SCustomData(const SCustomData& Other)
		: A(Other.A)
		, B(Other.B)
	{
		std::cout << "Copy constructor" << std::endl;
	}

	SCustomData(SCustomData&& Other)
		: A(Other.A)
		, B(Other.B)
	{
		std::cout << "Move constructor" << std::endl;
	}

	SCustomData(uint8 InA, uint8 InB)
		: A(InA)
		, B(InB)
	{
		std::cout << "Constructor with [A = " << (int)A << ", B = " << (int)B << "]" << std::endl;
	}

	~SCustomData()
	{
		std::cout << "Destructor" << std::endl;
	}

	uint8 A, B;
};

int main()
{
	std::cout << "Hello World" << std::endl;
	
	TArray<SCustomData> myArray;
	SCustomData& data = myArray.Add_GetRef({1, 3});
	data.A = 2;

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