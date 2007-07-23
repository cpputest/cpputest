
extern "C" {
	#include "hello.h"
	extern helloWorldApi helloWorldApiInstance;
}

#include <stdio.h>
#include <stdarg.h>
#include "CppUTest/TestHarness.h"

#define BUFFER_SIZE 256

static char buffer[BUFFER_SIZE];

TEST_GROUP(HelloWorld)
{ 
	static int output_method(const char* output, ...)
	{
		va_list arguments;
		va_start(arguments, output);
		vsnprintf(buffer, BUFFER_SIZE, output, arguments);
		va_end(arguments);
		return 1;
	}
	void setup()
  	{
  		helloWorldApiInstance.printHelloWorld_output = &output_method;
  	}
  	void teardown()
  	{
  		helloWorldApiInstance.printHelloWorld_output = &printf;
  	}
};


TEST(HelloWorld, PrintOk) 
{
	printHelloWorld();
	STRCMP_EQUAL("Hello World!\n", buffer);	
}
