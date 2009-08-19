extern "C"
{
#include "X.h"
}

//CppUTest includes should be after your and system includes
#include "CppUTest/TestHarness.h"

TEST_GROUP(X)
{
    void setup()
    {
    }

    void teardown()
    {
    }
};

TEST(X, Create)
{
  FAIL("Start here");
}

