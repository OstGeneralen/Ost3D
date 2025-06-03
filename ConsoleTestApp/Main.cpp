#pragma comment(lib, "Common_Debug")

#include <Common/Math/Mat3x3.h>
#include <Common/Math/Mat4x4.h>
#include <Common/Math/Vector4.h>
#include <Common/Math/Vector3.h>
#include <Common/Math/RotationUnits.h>
#include <Common/Utility/StringUtils.h>
#include <Common/Rendering/Color.h>
#include <Common/Container/DynamicList.h>
#include <iostream>

struct TestData
{
	TestData(int a, int b) : _a(a), _b(b){}
	int _a, _b;
};

template<typename ... TArgs>
void InstantiateTestData(TArgs&&... args)
{
	TestData t(args...);
}

int main(int argc, char* argv[])
{
	TDynamicList<TestData> testList(3);

	testList.Add(TestData(10, 1.0f));
	testList.Emplace(20, 2.0f);
	testList.Emplace(30, 3.0f);

	std::cout << "Normal:" << std::endl;
	std::cout << "Reserved Size " << testList.ReservedSize() << std::endl;
	for (size_t t = 0; t < testList.Size(); ++t)
	{
		std::cout << testList[t]._a << " | " << testList[t]._b << std::endl;
	}
	std::cout << "\nPost Remove:" << std::endl;
	std::cout << "Reserved Size " << testList.ReservedSize() << std::endl;
	testList.Remove(0);
	for (size_t t = 0; t < testList.Size(); ++t)
	{
		std::cout << testList[t]._a << " | " << testList[t]._b << std::endl;
	}


	testList.Emplace(40, 4.0f);
	testList.Emplace(50, 5.0f);
	testList.Emplace(60, 6.0f);

	std::cout << "\nPost More Adds:" << std::endl;
	std::cout << "Reserved Size " << testList.ReservedSize() << std::endl;
	for (size_t t = 0; t < testList.Size(); ++t)
	{
		std::cout << testList[t]._a << " | " << testList[t]._b << std::endl;
	}



	return 0;
}