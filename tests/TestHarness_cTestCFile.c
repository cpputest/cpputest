
#include "CppUTest/TestHarness_c.h"
#include "CppUTest/PlatformSpecificFunctions_c.h"

extern void functionWithUnusedParameter(void* PUNUSED(unlessParamater));

void functionWithUnusedParameter(void* PUNUSED(unlessParamater))
{

}

/* Declared in the cpp file */
extern int setup_was_called_in_test_group_in_C;

TEST_GROUP_C_SETUP(TestGroupInC)
{
	setup_was_called_in_test_group_in_C++;
}

TEST_GROUP_C_TEARDOWN(TestGroupInC)
{

}

TEST_C(TestGroupInC, one_test)
{

}
