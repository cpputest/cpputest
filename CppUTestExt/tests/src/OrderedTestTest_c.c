#include "CppUTest/TestHarness_c.h"
#include "OrderedTestTest.h"

TEST_C(TestOrderedTestMacros, Test11)
{
    CHECK_C(orderedTestFixtureCWrapper() == 13);
}

TEST_C(TestOrderedTestMacros, Test7)
{
    CHECK_C(orderedTestFixtureCWrapper() == 9);
}

TEST_C(TestOrderedTestMacros, Test9)
{
    CHECK_C(orderedTestFixtureCWrapper() == 11);
}
