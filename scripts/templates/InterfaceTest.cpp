#include "ClassName.h"
#include "MockClassName.h"

//Should be the last include
#include "CppUTest/TestHarness.h"

TEST_GROUP(ClassName)
{
  ClassName* aClassName;
  MockClassName* mockClassName;

  void setup()
  {
    mockClassName = new MockClassName();
    aClassName = mockClassName;
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
