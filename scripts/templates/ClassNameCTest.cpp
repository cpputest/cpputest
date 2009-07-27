extern "C"
{
#include "ClassName.h"
}

//Should be the last include
#include "CppUTest/TestHarness.h"

TEST_GROUP(ClassName)
{
    void setup()
    {
      ClassName_Create();
    }

    void teardown()
    {
       ClassName_Destroy();
    }
};

TEST(ClassName, Create)
{
  FAIL("Start here");
}

