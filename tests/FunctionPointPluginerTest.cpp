
#include "CppUTest/TestHarness.h"
#include "CppUTest/MockTestOutput.h"
#include "CppUTest/TestPlugin.h"

static TestRegistry* myRegistry;
static MockTestOutput* output;
static TestResult* result;

void orig_func1 () {};
void stub_func1 () {};
void orig_func2 () {};
void stub_func2 () {};

void (*fp1)() = orig_func1; 
void (*fp2)() = orig_func2; 

class FpUtest : public Utest
{
public:
	int numOfFpSets;
	FpUtest () : numOfFpSets(0) {};
	
	void setup()
	{
		for (int i = 0; i < numOfFpSets; ++i)
			UT_FPSET(fp1, stub_func1);
	}
	void testBody()
	{
		CHECK(fp1 == stub_func1);
    }
};


TEST_GROUP(FunctionPointPluginTest)
{
	FunctionPointerPlugin* plugin;
	FpUtest *tst;
	
	void setup() 
	{
		myRegistry = new TestRegistry();
    	plugin = new FunctionPointerPlugin();
		tst = new FpUtest();
		myRegistry->setCurrentRegistry(myRegistry);
		myRegistry->installPlugin(plugin);
		myRegistry->addTest(tst);
    	output = new MockTestOutput();
  		result = new TestResult(*output);
  		fp1 = orig_func1;
  		fp2 = orig_func2;
  	}
  
  	void teardown()
  	{
  		myRegistry->setCurrentRegistry(0);
		delete myRegistry;
		delete plugin;
		delete tst;
		delete output;
		delete result;
  	}
};

TEST(FunctionPointPluginTest, installOneFunctionPointer)
{
	UT_FPSET(fp1, stub_func1);
	myRegistry->runAllTests(*result);
	CHECK(fp1 == orig_func1);
	LONGS_EQUAL(0, result->getFailureCount());
}

TEST(FunctionPointPluginTest, installTwoFunctionPointer)
{
	UT_FPSET(fp1, stub_func1);
	UT_FPSET(fp2, stub_func2);
	myRegistry->runAllTests(*result);
	CHECK(fp1 == orig_func1);
	CHECK(fp2 == orig_func2);
	LONGS_EQUAL(0, result->getFailureCount());
}

TEST(FunctionPointPluginTest, installTooMuchFunctionPointer)
{
	tst->numOfFpSets = FunctionPointerPlugin::MAX_FPS + 1;
	myRegistry->runAllTests(*result);
	LONGS_EQUAL(1, result->getFailureCount());
}
