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
	TestData() = default;
	TestData(int a, int b) : _a(a), _b(b){}
	int _a, _b;
};

void PrintTestDataList(TDynamicList<TestData>& l, const char* desc)
{
	std::cout << "OP: " << desc << std::endl;
	std::cout << "  Size: " << l.Size() << " | Reserved Size: " << l.ReservedSize() << std::endl;
	for (size_t i = 0; i < l.Size(); ++i)
	{
		std::cout << "    " << i << ". " << l[i]._a << " | " << l[i]._b << std::endl;
	}
	std::cout << std::endl;
}



int main(int argc, char* argv[])
{
	TDynamicList<TestData> testList(3);

	testList.Emplace(10, 1.0f);
	testList.Emplace(20, 2.0f);
	testList.Emplace(30, 3.0f);
	testList.Emplace(40, 4.0f);
	testList.Emplace(50, 5.0f);

	PrintTestDataList(testList, "Initial");

	TestData d;
	bool result = testList.TryPop(d);
	std::cout << "Pop result " << result << " (Popped: " << d._a << " | " << d._b << ")" << std::endl;
	PrintTestDataList(testList, "Post Pop");

	TDynamicList<TestData> movedToList;

	movedToList = Util::Move(testList);

	PrintTestDataList(testList, "Post copy");
	PrintTestDataList(movedToList, "Copy into list");


	return 0;
}