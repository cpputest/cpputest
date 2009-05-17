
#include <stdio.h>
#include "hello.h"

void printHelloWorld()
{
	printHelloWorld_output("Hello World!\n");
}

int (*printHelloWorld_output) (const char*, ...) = printf;
