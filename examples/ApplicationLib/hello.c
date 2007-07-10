
#include <stdio.h>
#include "hello.h"

struct helloWorldApi helloWorldApiInstance = {
	&printf
};

void printHelloWorld()
{
	helloWorldApiInstance.printHelloWorld_output("Hello World!\n");
}
