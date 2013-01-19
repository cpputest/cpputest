
#include "CppUTest/TestHarness_c.h"
#include "CppUTest/PlatformSpecificFunctions_c.h"

extern void functionWithUnusedParameter(void* PUNUSED(unlessParamater));

void functionWithUnusedParameter(void* PUNUSED(unlessParamater))
{

}
