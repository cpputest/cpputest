#include "CppUTest/TestHarness.h"
#include "CppUTest/SimpleStringExtensions.h"
#include "ClassName.h"
#include "MockClassName.h"

EXPORT_TEST_GROUP(ClassName);

namespace
  {
  ClassName* aClassName;
  MockClassName* mockClassName;

  void SetUp()
  {
    mockClassName = new MockClassName();
    aClassName = mockClassName;
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

