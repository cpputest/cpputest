
extern "C" {
	#include "hello.h"
	extern helloWorldApi helloWorldApiInstance;
}

#include <stdio.h>
#include <stdarg.h>
#include "CppUTest/TestHarness.h"

static SimpleString buffer;

TEST_GROUP(HelloWorld)
{
	static int output_method(const char* output, ...)
	{
		va_list arguments;
		va_start(arguments, output);
		buffer = VStringFromFormat(output, arguments);
		va_end(arguments);
		return 1;
	}
	void setup()
  	{
	    UT_PTR_SET(helloWorldApiInstance.printHelloWorld_output, &output_method);
  	}
  	void teardown()
  	{
  	}
};


TEST(HelloWorld, PrintOk)
{
	printHelloWorld();
	STRCMP_EQUAL("Hello World!\n", buffer.asCharString());
}
