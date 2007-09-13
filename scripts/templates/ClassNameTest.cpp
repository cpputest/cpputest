#include "CppUTest/TestHarness.h"
#include "CppUTest/SimpleStringExtensions.h"
#include "ClassName.h"

TEST_GROUP(ClassName)
{
  ClassName* aClassName;

  setup()
  {
    aClassName = new ClassName();
  }
  teardown()
  {
    delete aClassName;
  }
};

TEST(ClassName, Create)
{
  FAIL("Start here");
}

