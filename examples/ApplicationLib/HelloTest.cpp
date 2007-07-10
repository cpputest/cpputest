
extern "C" {
	#include "hello.h"
	extern helloWorldApi helloWorldApiInstance;
}

#include <stdio.h>
#include <stdarg.h>
#include "CppUTest/TestHarness.h"

EXPORT_TEST_GROUP(HelloWorld);

#define BUFFER_SIZE 256
namespace 
{
	char buffer[BUFFER_SIZE];
	int output_method(const char* output, ...)
	{
		va_list arguments;
		va_start(arguments, output);
		vsnprintf(buffer, BUFFER_SIZE, output, arguments);
		va_end(arguments);
	}
	void SetUp()
  	{
  		helloWorldApiInstance.printHelloWorld_output = &output_method;
  	}
  	void TearDown()
  	{
  		helloWorldApiInstance.printHelloWorld_output = &printf;
  	}
}


TEST(HelloWorld, PrintOk) 
{
	printHelloWorld();
	STRCMP_EQUAL("Hello World!\n", buffer);	
}
