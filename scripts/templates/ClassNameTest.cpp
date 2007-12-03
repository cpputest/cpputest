#include "CppUTest/TestHarness.h"
#include "ClassName.h"

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

