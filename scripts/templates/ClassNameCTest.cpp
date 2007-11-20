#include "CppUTest/TestHarness.h"
#include "CppUTest/SimpleStringExtensions.h"

extern "C"
{
#include "ClassName.h"
}

static struct ClassName* aClassName;

TEST_GROUP(ClassName)
{
  ClassName* aClassName;

  void setup()
  {
      aClassName = ClassName_create();
  }
  
  void teardown()
  {
     ClassName_destroy(aClassName);
  }
};

TEST(ClassName, Create)
{
  FAIL("Start here");
}

