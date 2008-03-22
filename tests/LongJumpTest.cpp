#include <stdio.h>
#include "CppUTest/TestHarness.h"

extern "C"
{
#include "LongJump.h"
}

TEST_GROUP(LongJump)
{
    void setup()
    {
    }
    
    void teardown()
    {
    }
};


TEST(LongJump, NormalReturn)
{
    if (0 == PLATFORM_SETJMP)
    {
        return;
    }
    FAIL("Should not have gotten here");
}

TEST(LongJump, LongErrorReturn)
{
    if (0 == PLATFORM_SETJMP)
    {
        PLATFORM_LONGJMP;
        FAIL("LongJump not implemented on this platform.  Multiple errors per one test is possible");
    }
}

