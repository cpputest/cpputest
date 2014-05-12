extern "C"
{
#include "ClassName.h"
}

//CppUTest includes should be after your and system includes
#include "CppUTest/TestHarness.h"

TEST_GROUP(ClassName)
{
    void setup()
    {
    }

    void teardown()
    {
    }
};

TEST(ClassName, Create)
{
  FAIL("Start here");
}

