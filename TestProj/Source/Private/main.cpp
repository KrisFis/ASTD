
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

void TestVector()
{
	auto readData = [](const std::vector<SCustomData>& Datas, const char* message)
		{
			SLogger::Begin() << message << SLogger::End();

			for(const SCustomData& data : Datas)
			{
				SLogger::Begin() << "Read data [A = " << data.A << ", B = " << data.B << "]" << SLogger::End();
			}
		};

	SLogger::Begin() << "Hello Vector" << SLogger::End();
	
	std::vector<SCustomData> myArray;

	myArray.push_back({1, 3});
	myArray[0].A = 2;

	SCustomData data2 = SCustomData(8, 8);
	myArray.push_back(data2);

	myArray.push_back({2, 4});
	myArray.push_back({2, 5});
	myArray.push_back({2, 6});

	readData(myArray, "Vector 1");

	myArray.clear();

	readData(myArray, "Vector 2");

	myArray.push_back({2, 7});
	myArray.push_back({2, 8});
	myArray.push_back({2, 9});
	myArray.push_back({2, 10});
	myArray.pop_back();
	
	readData(myArray, "Vector 3");

	std::vector<SCustomData> newData = myArray;
	std::vector<SCustomData> newData2(myArray);
}

void TestArray()
{
	auto readData = [](const TArray<SCustomData>& Datas, const char* message)
	{
		SLogger::Begin() << message << SLogger::End();

		for(const SCustomData& data : Datas)
		{
			SLogger::Begin() << "Read data [A = " << data.A << ", B = " << data.B << "]" << SLogger::End();
		}
	};

	SLogger::Begin() << "Hello Array" << SLogger::End();
	
	TArray<SCustomData> myArray;

	SCustomData& data = myArray.Add_GetRef({1, 3});
	data.A = 2;

	SCustomData data2 = SCustomData(8, 8);
	myArray.Add(data2);

	myArray.Add({2, 4});
	myArray.Add({2, 5});
	myArray.Add({2, 6});

	readData(myArray, "Array 1");

	myArray.Empty();

	readData(myArray, "Array 2");

	myArray.Add({2, 7});
	myArray.Add({2, 8});
	myArray.Add({2, 9});
	myArray.Add({2, 10});
	myArray.RemoveAt(0);
	
	readData(myArray, "Array 3");

	TArray<SCustomData> newData = myArray;
	TArray<SCustomData> newData2(myArray);
}

void TestQueue()
{
	SLogger::Begin() << "Hello Queue" << SLogger::End();

	TQueue<SCustomData> myQueue;

	myQueue.Enqueue({1, 1});
	myQueue.Enqueue({2, 2});
	myQueue.Enqueue({3, 3});
	myQueue.Enqueue({4, 4});

	myQueue.Empty();

	myQueue.Enqueue({5, 5});
	myQueue.Enqueue({6, 6});
	myQueue.Enqueue({7, 7});

	{
		SCustomData data;
		if(myQueue.Peek(data))
		{
			SLogger::Begin() << "Peek data [A = " << data.A << ", B = " << data.B << "]" << SLogger::End();
		}

		SLogger::Begin() << "Invalid peek!" << SLogger::End();
	}

	{
		myQueue.Dequeue();

		SCustomData data;
		if(myQueue.Dequeue(data))
		{
			SLogger::Begin() << "Dequeue data [A = " << data.A << ", B = " << data.B << "]" << SLogger::End();
		}
	}

	myQueue.Enqueue({4, 4});

	{
		SCustomData data;
		while(myQueue.Dequeue(data))
		{
			SLogger::Begin() << "Dequeue data [A = " << data.A << ", B = " << data.B << "]" << SLogger::End();
		}
	}

	myQueue.Enqueue({11,11});
	myQueue.Enqueue({12,12});
	myQueue.Enqueue({13,13});
}

int main()
{
	TestArray();
	TestVector();
	//TestQueue();

	return 0;
}