
#include <stdio.h>
#include "hello.h"

void printHelloWorld()
{
	PrintFormated("Hello World!\n");
}

int (*PrintFormated) (const char*, ...) = printf;
