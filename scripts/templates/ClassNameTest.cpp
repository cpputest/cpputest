#include "ClassName.h"

//Should be the last include
#include "CppUTest/TestHarness.h"

TEST_GROUP(ClassName)
{
  ClassName* aClassName;

  void setup()
  {
    aClassName = new ClassName();
  }
  void teardown()
  {
    delete aClassName;
  }
};

TEST(ClassName, Create)
{
  FAIL("Start here");
}

