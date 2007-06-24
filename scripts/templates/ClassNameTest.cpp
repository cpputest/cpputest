#include "CppUTest/TestHarness.h"
#include "CppUTest/SimpleStringExtensions.h"
#include "ClassName.h"

EXPORT_TEST_GROUP(ClassName);

namespace
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
}

TEST(ClassName, Create)
{
  FAIL("Start here");
}

