#include "CppUTest/TestHarness.h"
#include "CppUTest/SimpleStringExtensions.h"
#include "ClassName.h"

TEST_GROUP(ClassName)
{
  ClassName* aClassName;

  void SetUp()
  {
    aClassName = new ClassName();
  }
  void TearDown()
  {
    delete aClassName;
  }
};

TEST(ClassName, Create)
{
  FAIL("Start here");
}

