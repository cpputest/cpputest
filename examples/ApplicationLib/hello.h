
#ifndef HELLO_H_
#define HELLO_H_

extern void printHelloWorld();

struct helloWorldApi {
	int (*printHelloWorld_output) (const char*, ...);
};

#endif /*HELLO_H_*/
