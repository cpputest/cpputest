#include "ClassName.h"
#include "MockClassName.h"

//CppUTest includes should be after your and system includes
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
