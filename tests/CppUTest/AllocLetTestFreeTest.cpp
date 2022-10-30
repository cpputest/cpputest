
#include "CppUTest/StandardCLibrary.h"

extern "C"
{
#include "AllocLetTestFree.h"
}

#include "CppUTest/TestHarness.h"

/*
 * This test makes sure that memory leak malloc macros are forced into .cpp and .c files
 */
TEST_GROUP(AllocLetTestFree)
{
    AllocLetTestFree allocLetTestFree;

    void setup() _override
    {
        allocLetTestFree = AllocLetTestFree_Create();
    }

    void teardown() _override
    {
        AllocLetTestFree_Destroy(allocLetTestFree);
    }
};

TEST(AllocLetTestFree, Create)
{
    free(allocLetTestFree);
}
