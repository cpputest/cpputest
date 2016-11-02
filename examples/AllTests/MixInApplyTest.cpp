
#include "CppUTest/TestHarness.h"
#include "MixInTest.h"

/*
	each implementation of the SUT interface should have:
	- a function className() returning the name of the class
	- a function foo(int val) returning an integer value greater than the given parameter val
*/
class ImplA : public FooInterface
{
public:

// implementing the FooInterface
	const char* className() { return "ImplA"; }
	int foo(int val) { return val + 1; }

// implementation specific
	bool bar() { return true; }
};

class ImplB : public FooInterface
{
public:
	ImplB() { dummyVariable = 1; }

// implementing the FooInterface
	const char* className() { return "ImplB"; }
	int foo(int val) { return val + 10; }

// implementation specific
	int dummyVariable;
};

//
// tests for ImplA implementation of the FooInterface
//
TEST_GROUP(ImplATestGroup)
{
	ImplA obj;
};

// add common tests of DemoMixInGroup to ImplATestGroup with the name prefix: MixInImplA
MIXIN_APPLY(ImplATestGroup, DemoMixInGroup, MixInImplA)
{
	params.sut = &obj;
	params.expectedName = "ImplA";
}

// implementation specific tests
TEST(ImplATestGroup, implementationSpecificTest)
{
	CHECK( obj.bar() );
}


//
// tests for ImplA implementation of the FooInterface
//
TEST_GROUP(ImplBTestGroup)
{
	ImplB obj;
};

// add common tests of DemoMixInGroup to ImplBTestGroup with the name prefix: MixInImplB
MIXIN_APPLY(ImplBTestGroup, DemoMixInGroup, MixInImplB)
{
	params.sut = &obj;
	params.expectedName = "ImplB";
}

// implementation specific tests
TEST(ImplBTestGroup, justADummyTest)
{
	LONGS_EQUAL( 1, obj.dummyVariable );
}

