#include "CppUTest/TestHarness.h"

extern "C"
{
#include "FakeClassName.h"
}

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
