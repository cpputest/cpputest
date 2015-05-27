
#include "CppUTest/TestHarness.h"
#include "MixInTest.h"

class MyImplA : public MyInterface
{
public:
	const char* foo() { return "foo"; };
	const char* className() { return "MyImplA"; };
};

TEST_GROUP(ImplA)
{
	void setup() {}
	void teardown() {}
	MyImplA objA;
};

MIXIN_APPLY(ImplA, MixInTest, ImplA_test)
{
	params->obj = &objA;
}

class MyImplB : public MyInterface
{
public:
	const char* foo() { return "foo"; };
	const char* className() { return "MyImplB"; };
};

TEST_GROUP(ImplB)
{
	void setup() {}
	void teardown() {}
	MyImplB objB;
};

MIXIN_APPLY(ImplB, MixInTest, ImplB_test)
{
	params->obj = &objB;
}
