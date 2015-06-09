
#include "CppUTest/TestHarness.h"
#include "MixInTest.h"

class ImplA : public SUT
{
public:
	const char* className() { return "ImplA"; }
};

class ImplB : public SUT
{
public:
	const char* className() { return "ImplB"; }
};

TEST_GROUP(ImplATestGroup)
{
	ImplA objA;
};

MIXIN_APPLY(ImplATestGroup, DemoMixInGroup, ImplA_test)
{
	params.obj = &objA;
	params.expectedName = "ImplA";
}

TEST_GROUP(ImplBTestGroup)
{
	ImplB objB;
};

MIXIN_APPLY(ImplBTestGroup, DemoMixInGroup, ImplB_test)
{
	params.obj = &objB;
	params.expectedName = "ImplB";
}
