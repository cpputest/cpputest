#include "CppUTest/TestHarness.h"

static int fakeRan = 0;

extern "C"
{
#include "ClassName.h"
}

TEST_GROUP(ClassName)
{
    ClassName* aClassName;

    void setup()
    {
      aClassName = ClassName_Create();
      fakeRan = 0;
    }

    void teardown()
    {
       ClassName_Destroy(aClassName);
    }
};

TEST(ClassName, Create)
{
  FAIL("Start here");
}

