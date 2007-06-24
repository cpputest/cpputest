#include "CppUTest/TestHarness.h"
#include "CppUTest/SimpleStringExtensions.h"

extern "C"
  {
#include "ClassName.h"
  }

EXPORT_TEST_GROUP(ClassName);

static struct ClassName* aClassName;

static void SetUp()
{
  aClassName = ClassName_create();
}
static void TearDown()
{
  ClassName_destroy(aClassName);
}


TEST(ClassName, Create)
{
  FAIL("Start here");
}

